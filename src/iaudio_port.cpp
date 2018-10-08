#include "iaudio_port.h"

static Queue<int, 4> *q;

pAudio::pAudio(const iParam* p)
    : iAudio(p)
    , t(osPriorityAboveNormal, 4096, NULL, "AUDIO")
{
    __DAC_Init();
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)cBuffer, CIR_BUFFER_SIZE, DAC_ALIGN_12B_L);
    t.start(callback(this, &pAudio::thandle));
    q = &queue;
}
void pAudio::thandle()
{
    osEvent evt;
    while (1)
    {
        evt = queue.get();
        handle(*(int*)(evt.value.p));
    }
}
void pAudio::handle(int sig)
{
    uint16_t *dest;
    FRESULT fres;
    UINT reserved_uint;
    FSIZE_t leftSize;
    int avaliableCnt = 0;
    int i;

    if (sig == 1) // half transmit compelete signal
        dest = cBuffer + BLOCK_BUFFER_SIZE;
    else if (sig == 2) // transmit compelete signal 
        dest = cBuffer;
    else // unknow sig
        return;

    memset(mirror_cBuffer, 0, sizeof(mirror_cBuffer));

    for (i = 0; i < MAX_TRACK_NUM; i++)
    {
        // check if file opened
        if (path[i][0] == '\0')
            continue;

        fres = f_read(&file[i], bBuffer[i], BLOCK_BUFFER_SIZE, &reserved_uint);
        if (fres != FR_OK)
            continue;

        avaliableCnt++;

        // output block buffer to mirror buffer
        int16_t *_d = mirror_cBuffer;
        int16_t *_s = bBuffer[i];
        for (int n = 0; n < BLOCK_BUFFER_SIZE; n++)
        {
            dataComb(_d++, _s++);
        }

        // check if should close file
        leftSize = f_size(&file[i]) - f_tell(&file[i]);
        if (leftSize < WAV_FIXED_DATA_DEPTH * BLOCK_BUFFER_SIZE)
        {
            // check if it is loop
            if (mode[i] == loop)
            {
                f_lseek(&file[i], 44);
            }
            else
            {
                dellocTrack(i);
            }
        }

    }

    dataConvert(dest, mirror_cBuffer, BLOCK_BUFFER_SIZE);

    if (avaliableCnt == 0)
    {
        // should mute
    }
}

int pAudio::allocTrack(int trackID, const char* filepath, mode_t _mode)
{
    FRESULT res;
    if (trackID >= MAX_TRACK_NUM || trackID < 0)
        return -1;
    if (path[trackID][0] != '\0')
        return -1;
    mutex[trackID].lock();
    res = f_open(&file[trackID], filepath, FA_READ);
    if (res != FR_OK)
        return -1;
    res = f_lseek(&file[trackID], 44);
    if (res != FR_OK)
        return -1;
    strncpy(path[trackID], filepath, 32);
    mode[trackID] = _mode;
    mutex[trackID].unlock();
    return 0;
}
void pAudio::dellocTrack(int trackID)
{
    if (trackID >= MAX_TRACK_NUM || trackID < 0)
        return;
    if (path[trackID][0] != '\0')
    {
        mutex[trackID].lock();
        f_close(&file[trackID]);
        path[trackID][0] = '\0';
        mutex[trackID].unlock();
    }
}

pAudio::trackId_t pAudio::_play(const char* path, mode_t mode)
{
    for (int i = 0; i < MAX_TRACK_NUM; i++)
    {
        if (allocTrack(i, path, mode) == 0)
            return i;
    }
    return -1;
}

bool pAudio::abort(trackId_t id)
{
    dellocTrack((int)id);
    return true;
}

uint32_t pAudio::getTrackDuration(trackId_t id)
{
    if (id >= MAX_TRACK_NUM || id < 0)
        return 0;
    FSIZE_t leftSize = f_size(&file[id]) - f_tell(&file[id]);
    return CONVERT_SIZE_2_TIME(leftSize);
}

void pAudio::dataConvert(uint16_t* dest, int16_t *src, int size)
{
    static const int offset = 0x10000/2;
    int t, i;

    for (i = 0; i < size; i++)
    {
        t = *src++ + offset;
        if (t > 0xFFFF)
            t = 0xFFFF;
        else if (t < 0)
            t = 0;
        *dest++ = t;
    }
}

void pAudio::dataComb(int16_t *dest, int16_t* src)
{
    int t = *dest + *src;
    if (t > INT16_MAX)
        t = INT16_MAX;
    else if (t < INT16_MIN)
        t = INT16_MIN;
    *dest = t;
}
extern "C" void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
    static int sig = 1;
    q->put(&sig);
}
extern "C" void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
    static int sig = 2;
    q->put(&sig);
}

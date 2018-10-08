#include "iaudio_port.h"

static Queue<int, 4> *q;

pAudio::pAudio(const iParam* p)
    : iAudio(p)
    , readThread(osPriorityAboveNormal, 4096, NULL, "AUDIO read")
    , fifoThread(osPriorityAboveNormal, 1024, NULL, "AUDIO fifo")
{
    q = &queue;
    readThread.start(callback(this, &pAudio::read_handle));
    fifoThread.start(callback(this, &pAudio::fifo_handle));
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)0, 1, DAC_ALIGN_12B_R);
}

void pAudio::fifo_handle()
{
    osEvent pre_evt;
    osEvent evt;

    static uint16_t *reserved = NULL;

    uint32_t* dest;
    uint32_t cnt;

    if (reserved == NULL)
    {
        reserved = (uint16_t*)malloc(sizeof(uint16_t) * BLOCK_BUFFER_SIZE / 2);
        for (int i = 0; i < BLOCK_BUFFER_SIZE / 2; i++)
        {
            reserved[i] = 0xFFF / 2;
        }
    }
    while (1)
    {
        queue.get();

        if (pre_evt.status == osEventMail)
        {
            mail.free((BMail*)pre_evt.value.p);
        }
        evt = mail.get();
        if (evt.status != osEventMail)
        {
            dest = (uint32_t*)reserved;
            cnt = BLOCK_BUFFER_SIZE / 2;
        }
        else
        {
            dest = (uint32_t*)evt.value.p;
            cnt = BLOCK_BUFFER_SIZE;
        }

        HAL_DAC_Start_DMA(&hdac,
                          DAC_CHANNEL_1,
                          dest,
                          cnt,
                          DAC_ALIGN_12B_R);
        pre_evt = evt;
    }
}
void pAudio::read_handle()
{
    uint16_t *dest;
    FRESULT fres;
    UINT reserved_uint;
    FSIZE_t leftSize;
    int i;

    while (1)
    {

        if (isIdle())
        {
            wait(0.01f);
            continue;
        }

        memset(ansBuffer, 0, sizeof(ansBuffer));

        for (i = 0; i < MAX_TRACK_NUM; i++)
        {
            if (track[i].busy == false)
                continue;

            fres = f_read(&file[i], tmpBuffer,
                          WAV_FIXED_DATA_DEPTH * BLOCK_BUFFER_SIZE,
                          &reserved_uint);

            if (fres != FR_OK)
                continue;

            // output to ansBuffer
            int *_d = ansBuffer;
            int16_t* _s = tmpBuffer;
            for (int n = 0; n < BLOCK_BUFFER_SIZE; n++)
            {
                *_d = *_d + *_s;
                _d++;
                _s++;
            }

            // check if should close file
            leftSize = f_size(&file[i]) - f_tell(&file[i]);
            if (leftSize < WAV_FIXED_DATA_DEPTH * BLOCK_BUFFER_SIZE)
            {
                // check if it is loop, reopen it
                if (track[i].mode == loop)
                {
                    f_lseek(&file[i], 44);
                }
                else
                {
                    dellocTrack(i);
                }
            }
        }

        BMail* pt = mail.alloc();
        while (pt == NULL)
        {
            wait(0.01f);
            pt = mail.alloc();
        }
        dest = pt->d;

        dataConvert(dest, ansBuffer, BLOCK_BUFFER_SIZE);
        mail.put(pt);
    }
}

int pAudio::allocTrack(int trackID, const char* filepath, mode_t _mode)
{
    FRESULT res;
    if (trackID >= MAX_TRACK_NUM || trackID < 0)
        return -1;
    if (track[trackID].busy)
        return -1;
    res = f_open(&file[trackID], filepath, FA_READ);
    if (res != FR_OK)
        return -1;
    res = f_lseek(&file[trackID], 44);
    if (res != FR_OK)
        return -1;
    track[trackID].path = filepath;
    track[trackID].mode = _mode;
    track[trackID].busy = true;
    return 0;
}
void pAudio::dellocTrack(int trackID)
{
    if (trackID >= MAX_TRACK_NUM || trackID < 0)
        return;
    if (track[trackID].busy)
    {
        f_close(&file[trackID]);
        track[trackID].busy = false;
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

void pAudio::dataConvert(uint16_t* dest, int *src, int size)
{
    static const int offset = 0xFFF/2;
    int t, i;

    for (i = 0; i < size; i++)
    {
        t = *src++ + offset;
        if (t > 0xFFF)
            t = 0xFFF;
        else if (t < 0)
            t = 0;
        *dest++ = t;
    }
}


bool pAudio::isIdle()
{
    for (int i = 0; i < MAX_TRACK_NUM; i++)
    {
        if (track[i].busy)
            return false;
    }
    return true;
}
extern "C" void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
    static int sig = 1;
    q->put(&sig);
}
extern "C" void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
//    static int sig = 2;
//    q->put(&sig);
}

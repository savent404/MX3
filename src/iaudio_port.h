#pragma once

#include "dac_init.h"
#include "ff.h"
#include "iAudio.h"
#include "mbed.h"
#include "rtos.h"

#ifndef MAX_TRACK_NUM
#define MAX_TRACK_NUM (3)
#endif

#ifndef BLOCK_BUFFER_SIZE
#define BLOCK_BUFFER_SIZE (512)
#endif

#define WAV_FIXED_SAMPLE_FREQ (220500)
#define WAV_FIXED_DATA_DEPTH  (sizeof(uint16_t))
#define CONVERT_TIME_2_SIZE(ms)   (WAV_FIXED_SAMPLE_FREQ * ms / 1000 * WAV_FIXED_DATA_DEPTH)
#define CONVERT_SIZE_2_TIME(size) (size * 1000 / WAV_FIXED_DATA_DEPTH / WAV_FIXED_SAMPLE_FREQ)

typedef struct BMail
{
    uint16_t d[BLOCK_BUFFER_SIZE];
} BMail;

class pAudio : public iAudio {
private:
    Queue<int, 4> queue;
    Mail<BMail, 3> mail;

    FIL file[MAX_TRACK_NUM];

    Thread readThread;
    Thread fifoThread;

    int ansBuffer[BLOCK_BUFFER_SIZE];
    int16_t tmpBuffer[BLOCK_BUFFER_SIZE];

protected:
    int allocTrack(int trackID, const char* filePath, mode_t mode);
    void dellocTrack(int trackID);
    inline void dataConvert(uint16_t* dest, int *src, int size);

    bool isIdle(void);
public:
    pAudio(const iParam* p);

    virtual trackId_t _play(const char* wavPath, mode_t mode = oneShout);
    virtual bool abort(trackId_t id);
    virtual uint32_t getTrackDuration(trackId_t id);

    void read_handle();
    void fifo_handle();
};

extern "C" void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac);
extern "C" void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac);

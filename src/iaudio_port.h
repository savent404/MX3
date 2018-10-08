#pragma once

#include "dac_init.h"
#include "ff.h"
#include "iAudio.h"
#include "mbed.h"

#ifndef MAX_TRACK_NUM
#define MAX_TRACK_NUM (3)
#endif

#ifndef CIR_BUFFER_SIZE
#define CIR_BUFFER_SIZE (512)
#endif

#ifndef BLOCK_BUFFER_SIZE
#define BLOCK_BUFFER_SIZE (CIR_BUFFER_SIZE / 2)
#endif

#define WAV_FIXED_SAMPLE_FREQ (220500)
#define WAV_FIXED_DATA_DEPTH  (sizeof(uint16_t))
#define CONVERT_TIME_2_SIZE(ms)   (WAV_FIXED_SAMPLE_FREQ * ms / 1000 * WAV_FIXED_DATA_DEPTH)
#define CONVERT_SIZE_2_TIME(size) (size * 1000 / WAV_FIXED_DATA_DEPTH / WAV_FIXED_SAMPLE_FREQ)

class pAudio : public iAudio {
private:
    Queue<int, 4> queue;
    Thread t;
    char path[MAX_TRACK_NUM][32];
    FIL  file[MAX_TRACK_NUM];
    mode_t mode[MAX_TRACK_NUM];
    Mutex mutex[MAX_TRACK_NUM];

    uint16_t cBuffer[CIR_BUFFER_SIZE];
    int16_t bBuffer[MAX_TRACK_NUM][BLOCK_BUFFER_SIZE];
    int16_t mirror_cBuffer[BLOCK_BUFFER_SIZE];

protected:
    int allocTrack(int trackID, const char* filePath, mode_t mode);
    void dellocTrack(int trackID);
    inline void dataConvert(uint16_t* dest, int16_t *src, int size);
    inline void dataComb(int16_t* dest, int16_t* src);
    void thandle();
public:
    pAudio(const iParam* p);

    virtual trackId_t _play(const char* wavPath, mode_t mode = oneShout);
    virtual bool abort(trackId_t id);
    virtual uint32_t getTrackDuration(trackId_t id);

    void handle(int sig);
};

extern "C" void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac);
extern "C" void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac);

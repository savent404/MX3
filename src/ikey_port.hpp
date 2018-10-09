#pragma once

#include "iKey.h"
#include "mbed.h"
#include "systemSupport.h"

class pKey : public iKey
{
private:
    InterruptIn k1;
    InterruptIn k2;

    uint32_t tRiseK1;
    uint32_t tFallK1;
    uint32_t tRiseK2;
    uint32_t tFallK2;
    uint32_t duration[2];
    void rise_key1()
    {
        tRiseK1 = mGetCPUTime();
        duration[key1] = tRiseK1 - tFallK1;
        sendEvent(KEY_1_RELEASE | KEY_1_CLICK);
    }
    void rise_key2()
    {
        tRiseK2 = mGetCPUTime();
        duration[key2] = tRiseK2 - tFallK2;
        sendEvent(KEY_2_RELEASE | KEY_2_CLICK);
    }
    void fall_key1()
    {
        tFallK1 = mGetCPUTime();
        sendEvent(KEY_1_PRESS);
    }
    void fall_key2()
    {
        tFallK2 = mGetCPUTime();
        sendEvent(KEY_2_PRESS);
    }
public:
    pKey() : k1(PA_1), k2(PA_2)
    {
        tRiseK1 =0;
        tFallK1 = 0;
        tRiseK2 = 0;
        tFallK2 = 0;
        duration[0] = 0;
        duration[1] = 0;
        k1.rise(callback(this, &pKey::rise_key1));
        k1.fall(callback(this, &pKey::fall_key1));
        k2.rise(callback(this, &pKey::rise_key2));
        k2.fall(callback(this, &pKey::fall_key2));
    }

    virtual int getStatus(Key_t id)
    {
        if (id == key1)
        {
            return tFallK1 > tRiseK1 ? 0 : 1;
        }
        else
        {
            return tFallK2 > tRiseK2 ? 0 : 1;
        }
    }

    virtual uint32_t lastClickDuration(Key_t id)
    {
        return duration[int(id)];
    }
};

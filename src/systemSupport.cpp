#include "MXSchedule/Interface/systemSupport.h"
#include "mbed.h"
#include "cmsis_os.h"

uint32_t mGetCPUTime(void)
{
    return osKernelGetSysTimerCount();
}

void mDelay(uint32_t time)
{
    wait_ms(time);
}

void mDebug(int level, const char* str, ...)
{
    static char buffer[512];
    va_list aptr;
    int cnt;

    va_start(aptr, str);
    cnt = sprintf(buffer, ":%d ", level);
    cnt += vsprintf(buffer + cnt, str, aptr);
    cnt += sprintf(buffer, "\r\n", buffer + cnt);
    va_end(aptr);

    debug(buffer);
}

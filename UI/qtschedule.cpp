#include "qtschedule.h"
#include <QSemaphore>
#include <ctime>
#include <sys/time.h>
#include <queue>

#include <QDebug>
static std::queue<uint32_t> q;
static QSemaphore sem;

void mDelay(uint32_t time)
{
    QThread::msleep(time);
}

uint32_t mGetCPUTime()
{
    static bool inited = false;
    static uint32_t stamp = 0;
    if (inited == false)
    {
        inited = true;
        stamp = mGetCPUTime();
    }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint32_t res = tv.tv_sec * 1000 + tv.tv_usec / 1000 - stamp;
    return res;
}

void defaultEventSender(uint32_t message)
{
    q.push(message);
    sem.release();
}
bool defaultEventReciver(uint32_t& message, uint32_t timeout)
{
    if (sem.tryAcquire(1, timeout))
    {
        message = q.front();
        q.pop();
        return true;
    }
    return false;
}

void mDebug(int level, const char *str, ...)
{
    char buffer[1024];
    const char* ptr = nullptr;
    switch(level)
    {
    case DEBUG_LEVEL_ERROR:
        ptr = "E:";
        break;
    case DEBUG_LEVEL_INFO:
        ptr = "I:";
        break;
    case DEBUG_LEVEL_VERBOSS:
        ptr = "V:";
        break;
    default:
        ptr = "";
        break;
    }

    va_list p;
    va_start(p, str);
    vsprintf(buffer, str, p);
    va_end(p);

    qDebug() << ptr << " " << buffer;

}

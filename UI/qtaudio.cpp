#include "qtaudio.h"
#include "iAudio.h"
#include "common.h"
#include <string>
#include <QDebug>

#include "systemSupport.h"

#include <QDebug>

using namespace  std;

#define INTERVAL (20)

qtAudio::qtAudio(QObject *parent, iParam *p)
    : QObject(parent)

    , iAudio(p)
{
    aduioIOFormat.frequence = 44100;
    aduioIOFormat.bits = 8;
    aduioIOFormat.channels = 1;

    QAudioFormat format;
    format.setSampleRate(aduioIOFormat.frequence);
    format.setChannelCount(aduioIOFormat.channels);
    format.setSampleSize(aduioIOFormat.bits);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qDebug() << "Raw audio format not support";
        return;
    }

    audio = new QAudioOutput(format, this);
    audioIO = audio->start();
    timer = new QTimer(this);
    timer->setInterval(INTERVAL);
    audio->setBufferSize(44100);
    connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    timer->start();
}

qtAudio::~qtAudio()
{
    size_t n = sizeof(track) / sizeof(track[0]);
    for (size_t i = 0; i < n ;i++)
        abort(i);
}
iAudio::trackId_t qtAudio::_play(const char* wavPath, mode_t mode)
{
    // search any useful track
    size_t n = sizeof(track) / sizeof(track[0]);
    for (size_t i = 0; i < n; i++)
    {
        QString path = wavPath;
        if (track[i].busy)
            continue;
        track[i].path = wavPath;
        track[i].mode = mode;

        sourceFile[i].setFileName(QString(wavPath));
        if (!sourceFile[i].open(QFile::ReadOnly))
            return -1;
        QByteArray head = sourceFile[i].read(WAV_FIFF_SIZE);
        readWavFormat(sourceFormat[i], head.constData());

        sourcePos[i] = 0;
        track[i].T = wavSize2Time(sourceFormat[i], sourceFormat[i].totalSize);
        track[i].busy = true;
        return i;
    }
    return -1;
}
bool qtAudio::abort(qtAudio::trackId_t id)
{
    if (id < 0)
        return true;
    if (track[id].busy)
    {
        sourceFile[id].close();
        track[id].busy = false;
        sendEvent(end | (track1 << id));
    }
    return true;
}

uint32_t qtAudio::getTrackDuration(iAudio::trackId_t id)
{
    if (track[id].busy == false)
        return 0;
    return 500;
}

void qtAudio::handleTimeout()
{
    int chunks = audio->bytesFree() / audio->periodSize();
    while (chunks--)
    {
        while (tempBuffer.size() < audio->periodSize())
        {
            generateData();
        }
        audioIO->write(tempBuffer, audio->periodSize());
        tempBuffer = tempBuffer.data() + audio->periodSize();
    }
}

void qtAudio::generateData()
{
    int buffSize = wavTime2Size(aduioIOFormat, INTERVAL);

    memset(t, 0, buffSize);

    /** Read 100ms size from each file */
    for (int i = 0; i < 3; i++)
    {
        if (track[i].busy == false)
            continue;
        /** Calculate 100Ms needed size */
        int needSize = wavTime2Size(sourceFormat[i], INTERVAL);
        bool isEnd = false;
        if (needSize > sourceFormat[i].totalSize - sourcePos[i])
        {
            isEnd = true;
            needSize = sourceFormat[i].totalSize - sourcePos[i];
        }

        sourceFile[i].seek(WAV_FIFF_SIZE + sourcePos[i]);
        QByteArray source = sourceFile[i].read(needSize);

        if (convertWavFormat(sourceFormat[i], source.constData(), needSize,
                         aduioIOFormat, _t, buffSize) == false)
            qDebug("Convert format error");

        for (int j = 0; j < buffSize; j++)
        {
            int a = t[j] + _t[j];
            if (a > INT8_MAX)
                a = INT8_MAX;
            if (a < INT8_MIN)
                a = INT8_MIN;
            t[j] = a;
        }

        sourcePos[i] += needSize;
        if (isEnd)
        {
            if (i == mainTrackId)
                sourcePos[i] = 0;
            else
                abort(i);
        }

    }

    for (int i = 0; i < buffSize; i++)
    {
        int a = t[i] + 128;
        if (a > INT8_MAX)
            a = INT8_MAX;
        if (a < INT8_MIN)
            a = INT8_MIN;
        tempBuffer.append(a);
    }
}

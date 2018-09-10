#ifndef QTAUDIO_H
#define QTAUDIO_H

#include "iAudio.h"
#include "iParam.h"
#include <QFile>
#include <QAudioOutput>
#include <QTimer>
#include <QAudioDeviceInfo>
#include "AudioFormat.h"
#include <bits/stdc++.h>

class qtAudio : public QObject, public iAudio
{
    Q_OBJECT
private:
    QFile sourceFile[3];
    wavFormat_t sourceFormat[3];
    size_t sourcePos[3];
    audioFormat_t aduioIOFormat;
    QAudioOutput* audio;
    QIODevice* audioIO;
    QTimer *timer;

    QByteArray tempBuffer;
    char t[10000];
    char _t[10000];
public:
    explicit qtAudio(QObject *parent = nullptr, iParam* p = nullptr);
    virtual ~qtAudio();
    virtual trackId_t _play(const char* wavPath, mode_t mode = oneShout);
    virtual bool abort(trackId_t id);
    virtual uint32_t getTrackDuration(trackId_t id) override;
    void generateData();
public slots:
    void handleTimeout();
};

#endif // QTAUDIO_H

#ifndef QTSCHEDULE_H
#define QTSCHEDULE_H

#include <QThread>
#include "iSchedule.h"

class qtSchedule : public QThread, public iShechdule
{
public:
    qtSchedule(iDriverList l) : iShechdule(l){}
protected:
    virtual void run() override {
        iShechdule::run();
    }
signals:

public slots:
};

#endif // QTSCHEDULE_H

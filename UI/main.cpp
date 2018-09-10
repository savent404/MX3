#include "mainwindow.h"
#include <QApplication>

#include "qtschedule.h"


#include "qtkey.h"
#include "qtparameter.h"
#include "qtaudio.h"
#include "powerlevel.h"
#include "qtblade.h"
#include "qthand.h"

iDriverList list;
qtSchedule* s;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    iDriverList list;

    // iParam
    qtParameter *param = new qtParameter(0);
    list.param = param;

    // Blade
    RGB c(255), d(0, 255);
    qtBlade *blade = new qtBlade(param);
    blade->show();
    list.blade = blade;

    // iAudio
    qtAudio *audio = new qtAudio(nullptr, param);
    list.audio = audio;

    // iPower
    powerLevel* power = new powerLevel();
    power->show();
    list.power = power;

    // iHand
    qtHand *hand = new qtHand;
    hand->show();
    list.hand = hand;

    // iKey
    qtkey *key = new qtkey();
    key->show();
    list.key = key;

    s = new qtSchedule(list);
    s->start();
    return a.exec();
}

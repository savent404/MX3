#include "qtkey.h"
#include "ui_qtkey.h"

#include "QtDebug"
qtkey::qtkey(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qtkey),
    iKey(),
    isKey1Press(false),
    isKey2Press(false)
{
    ui->setupUi(this);
    ui->pushButton->setFocusPolicy(Qt::StrongFocus);
    ui->pushButton_2->setFocusPolicy(Qt::StrongFocus);
}


qtkey::~qtkey()
{
    delete ui;
}

uint32_t qtkey::lastClickDuration(iKey::Key_t id)
{
    if (id == key1)
    {
        return key1Duration;
    }
    else if (id == key2)
    {
        return key2Duration;
    }
    return 0;
}

int qtkey::getStatus(iKey::Key_t id)
{
    switch(id)
    {
    case key1: return isKey1Press ? 1 : 0; break;
    case key2: return isKey2Press ? 1 : 0; break;
    }
    return 0;
}

void qtkey::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    if (event->key() == Qt::Key_Z)
    {
        on_pushButton_pressed();
    }
    if (event->key() == Qt::Key_X)
    {
        on_pushButton_2_pressed();
    }
}

void qtkey::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
        return;
    if (event->key() == Qt::Key_Z)
    {
        on_pushButton_released();
    }
    if (event->key() == Qt::Key_X)
    {
        on_pushButton_2_released();
    }
}

void qtkey::on_pushButton_pressed()
{
    key1Stamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    isKey1Press = true;
    sendEvent(KEY_1_PRESS);
}

void qtkey::on_pushButton_released()
{
    qint64 buf = QDateTime::currentDateTime().toMSecsSinceEpoch();
    key1Duration = uint32_t(buf - key1Stamp);
    qDebug() << "KEY1: " << key1Duration;
    isKey1Press = false;
    sendEvent(KEY_1_CLICK | KEY_1_RELEASE);
}

void qtkey::on_pushButton_2_pressed()
{
    key2Stamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    isKey2Press = true;
    sendEvent(KEY_2_PRESS);
}

void qtkey::on_pushButton_2_released()
{
    qint64 buf = QDateTime::currentDateTime().toMSecsSinceEpoch();
    key2Duration = uint32_t(buf - key2Stamp);
    qDebug() << "KEY2: " << key2Duration;
    isKey2Press = false;
    sendEvent(KEY_2_CLICK | KEY_2_RELEASE);
}

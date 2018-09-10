#ifndef QTKEY_H
#define QTKEY_H

#include <QDialog>
#include <QDate>
#include <QKeyEvent>
#include "iKey.h"

namespace Ui {
class qtkey;
}

class qtkey : public QDialog, public iKey
{
    Q_OBJECT
private:
    qint64 key1Stamp;
    qint64 key2Stamp;

    uint32_t key1Duration;
    uint32_t key2Duration;

    bool isKey1Press;
    bool isKey2Press;
public:
    explicit qtkey(QWidget *parent = nullptr);
    virtual ~qtkey();
    virtual uint32_t lastClickDuration(Key_t id);
    virtual int getStatus(Key_t id);
protected slots:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
private slots:
    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_pushButton_2_pressed();

    void on_pushButton_2_released();

private:
    Ui::qtkey *ui;
};

#endif // QTKEY_H

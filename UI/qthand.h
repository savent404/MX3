#ifndef QTHAND_H
#define QTHAND_H

#include <QDialog>
#include "iHand.h"

namespace Ui {
class qtHand;
}

class qtHand : public QDialog, public iHand
{
    Q_OBJECT

public:
    explicit qtHand(QWidget *parent = 0);
    ~qtHand();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::qtHand *ui;
};

#endif // QTHAND_H

#include "qthand.h"
#include "ui_qthand.h"

qtHand::qtHand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qtHand),
    iHand()
{
    ui->setupUi(this);
}

qtHand::~qtHand()
{
    delete ui;
}

void qtHand::on_pushButton_clicked()
{
    sendEvent(handSwing);
}

void qtHand::on_pushButton_2_clicked()
{
    sendEvent(handStab);
}

void qtHand::on_pushButton_3_clicked()
{
    sendEvent(handSlash);
}

void qtHand::on_pushButton_4_clicked()
{
    sendEvent(handSpin);
}

void qtHand::on_pushButton_5_clicked()
{
    sendEvent(handClash);
}

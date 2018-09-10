#include "powerlevel.h"
#include "ui_powerlevel.h"

powerLevel::powerLevel(float level1, float level2, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::powerLevel),
    iPower(level1, level2)
{
    ui->setupUi(this);
    ui->verticalSlider->connect(ui->verticalSlider, SIGNAL(sliderMoved(int)),
                                this, SLOT(change(int)));
}

iPower::stage_t powerLevel::getStatus()
{
    int a = ui->verticalSlider->value();
    float v = (a) * (5.2f - 2.9f) / 100.0f + 2.9f;
    if (v > 5.1)
        stage = stage_full;
    else if (v > L1)
        stage = stage_aboveLevel1;
    else if (v > L2)
        stage = stage_aboveLevel2;
    else
        stage = stage_underLevel2;
    return stage;
}

powerLevel::~powerLevel()
{
    delete ui;
}

void powerLevel::change(int a)
{
    float v = (a) * (5.2f - 2.9f) / 100.0f + 2.9f;
    QString text = QString::number(v) + "V";
    ui->label_2->setText(text);
    measurement(v);
}

void powerLevel::on_checkBox_stateChanged(int arg1)
{
    sendEvent(arg1 ? event_plugin : event_plugout);
}

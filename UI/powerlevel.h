#ifndef POWERLEVEL_H
#define POWERLEVEL_H

#include <QDialog>
#include "iPower.h"

namespace Ui {
class powerLevel;
}

class powerLevel : public QDialog, public iPower
{
    Q_OBJECT

public:
    explicit powerLevel(float level1 = 3.5f, float level2 = 3.0f, QWidget *parent = nullptr);
    virtual iPower::stage_t getStatus();
    ~powerLevel();

private:
    Ui::powerLevel *ui;
    iPower::stage_t stage;
public slots:
    void change(int);
private slots:
    void on_checkBox_stateChanged(int arg1);
};

#endif // POWERLEVEL_H

#ifndef QTBLADE_H
#define QTBLADE_H

#include "iBlade.h"
#include <QWidget>
#include <QOpenGLWidget>
#include <QTimer>
#include <GL/gl.h>

class qtBlade : public QOpenGLWidget, public iBlade
{
    Q_OBJECT
private:
    QTimer *timer;
public:
    qtBlade(iParam *param, QWidget *parent = 0, size_t pixnum = BLADE_PIXEL);

    virtual void update() override
    {
        paintGL();
    }
    virtual ~qtBlade()
    {
        delete vector;
    }
public slots:
    void handle();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    virtual bool play(triggerID_t id, uint32_t duration = 0) override;
    virtual bool abort(triggerID_t id) override;
};

#endif // QTBLADE_H

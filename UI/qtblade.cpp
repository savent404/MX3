#include "qtblade.h"

qtBlade::qtBlade(iParam* param, QWidget* parent, size_t pixnum)
    : QOpenGLWidget(parent)
    , iBlade(param)
{
    timer = new QTimer();
    timer->setInterval(17);
    connect(timer, SIGNAL(timeout()), this, SLOT(handle()));
    timer->start();
    this->resize(50, 600);
}

void qtBlade::handle()
{
    iBlade::hanlde();
    if (isActive)
        repaint();
}


void qtBlade::initializeGL()
{
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void qtBlade::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float interval = 2.0f / getPixelNum();

    RGB *_p = ptr();
    for (size_t i = 0; i < getPixelNum(); i++)
    {

        float x_bot = i*interval - 1;
        float x_top = x_bot + interval;
        glBegin(GL_TRIANGLES);
        glColor3f(_p->wR()/255.0f, _p->wG()/255.0f, _p->wB()/255.0f);
        glVertex2f(-1.0, x_bot);
        glVertex2f(1.0, x_bot);
        glVertex2f(-1.0, x_top);
        glEnd();
        glBegin(GL_TRIANGLES);
        glColor3f(_p->wR()/255.0f, _p->wG()/255.0f, _p->wB()/255.0f);
        glVertex2f(-1.0, x_top);
        glVertex2f(1.0, x_top);
        glVertex2f(1.0, x_bot);
        glEnd();
        _p++;
    }
}

bool qtBlade::play(triggerID_t id, uint32_t duration)
{
    return iBlade::play(id, duration);
}

bool qtBlade::abort(triggerID_t id)
{
    return false;
}

void qtBlade::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

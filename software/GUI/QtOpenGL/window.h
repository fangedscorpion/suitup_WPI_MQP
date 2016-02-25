#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include "glwidget.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private:
    QSlider *createSlider();

    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *zSlider;
};

#endif

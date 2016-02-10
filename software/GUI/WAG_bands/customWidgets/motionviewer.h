#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "playbackcontroller.h"
#include "editingcontroller.h"
#include "superslider.h"
#include <QStackedWidget>
#include <QOpenGLWidget>
#include <visualization/glwidget.h>
#include "user.h"

class MotionViewer : public QWidget {
    Q_OBJECT
public:
    MotionViewer(QWidget *parent);
    ~MotionViewer();

    const QPushButton* getPlayPauseBtn() {return playPause;}
    const SuperSlider* getSlider() {return videoSlider;}

private:

    GLWidget *viewer;
    SuperSlider *videoSlider;
    QIcon playIcon;
    QIcon pauseIcon;

    QLabel *handle1Time;
    QLabel *handle2Time;

    QPushButton *playPause;


public slots:
    void playToggled(bool playing);
    void displayNewTime(int newMillis);
};

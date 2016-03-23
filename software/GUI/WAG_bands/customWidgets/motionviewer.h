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

#define SLIDER_DIVIDE_FACTOR 100

class MotionViewer : public QWidget {
    Q_OBJECT
public:
    MotionViewer(QPointer<QWidget> parent, QPointer<Model> m);
    ~MotionViewer();

    const QPushButton* getPlayPauseBtn() {return playPause;}
    const QPointer<SuperSlider> getSlider() {return videoSlider;}

private:

    GLWidget *viewer;
    Model *model;
    SuperSlider *videoSlider;
    QIcon playIcon;
    QIcon pauseIcon;

    QLabel *handle1Time;
    QLabel *handle2Time;

    QPushButton *playPause;
    QString convertTimeToString(qint32 convertTime);


public slots:
    void playToggled(bool playing);
    void updateFirstLabel(qint32 newTime);
    void updateLastLabel(qint32 newTime);
    void changeSliderRange(qint32 newSliderLen);

};

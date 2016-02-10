#include "motionviewer.h"
#include <QGroupBox>
#include <QBoxLayout>


MotionViewer::MotionViewer(QWidget *parent) : QWidget(parent){
    playIcon = QIcon(QPixmap(":/icons/play.png"));
    pauseIcon = QIcon(QPixmap(":/icons/pause.png"));

    // viewer window
    viewer = new GLWidget;
    // video controls
    QHBoxLayout *controls = new QHBoxLayout;
    playPause = new QPushButton;
    videoSlider = new SuperSlider;
    handle1Time = new QLabel("00:00.00");
    handle2Time = new QLabel("00:10.00");
    playPause->setIcon(playIcon);
    playPause->setIconSize(QSize(15,15));
    controls->addWidget(playPause);
    controls->addWidget(handle1Time);
    controls->addWidget(videoSlider);
    controls->addWidget(handle2Time);

    // add everything
    QVBoxLayout *viewerPane = new QVBoxLayout;
    viewerPane->addWidget(viewer,1);
    viewerPane->addLayout(controls);
    this->setLayout(viewerPane);
}

MotionViewer::~MotionViewer() {}

void MotionViewer::playToggled(bool playing) {
    qDebug("HERE");
    if (playing) {
        playPause->setIcon(pauseIcon);
    }
    else {
        playPause->setIcon(playIcon);
    }
}

// display stopwatch timer time
void MotionViewer::displayNewTime(int newMillis) {
    int numSeconds = newMillis/1000;
    int hundredths = (newMillis - numSeconds*1000)/10;
    int numMinutes = numSeconds/60;
    numSeconds = numSeconds - numMinutes*60;
    // set label
    QString minNum = QString("%1").arg(numMinutes, 2, 10, QChar('0'));
    QString secNum = QString("%1").arg(numSeconds, 2, 10, QChar('0'));
    QString hundNum = QString("%1").arg(hundredths, 2, 10, QChar('0'));
    handle1Time->setText(minNum + ":" + secNum + "." + hundNum);
}

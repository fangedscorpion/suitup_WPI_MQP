#include "motionviewer.h"
#include <QGroupBox>
#include <QBoxLayout>


MotionViewer::MotionViewer(QWidget *parent, Model *m) : QWidget(parent){
    playIcon = QIcon(QPixmap(":/icons/play.png"));
    pauseIcon = QIcon(QPixmap(":/icons/pause.png"));

    // viewer window
    viewer = new GLWidget(m);
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
    if (playing) {
        playPause->setIcon(pauseIcon);
    }
    else {
        playPause->setIcon(playIcon);
    }
}

void MotionViewer::changeSliderRange(qint32 newSliderLen) {
    qDebug()<<"MotionViewer: NEW slider len: "<<newSliderLen/SLIDER_DIVIDE_FACTOR;
    //videoSlider->setMaximum(newSliderLen/SLIDER_DIVIDE_FACTOR);
}

QString MotionViewer::convertTimeToString(qint32 convertTime) {
    // round number to nearest 10th of millisecond
    int modVal = convertTime%10;
    if (modVal >= 5) {
        convertTime += (10-modVal);
    } else {
        convertTime  -= modVal;
    }

    QTime time = QTime(0, 0, 0, 0);
    time = time.addMSecs(convertTime);

    QString timeString = time.toString("mm:ss.zzz");
    timeString.remove(timeString.length() - 1, 1);
    return timeString;
}

void MotionViewer::updateFirstLabel(qint32 newTime) {
    handle1Time->setText(convertTimeToString(newTime));
}

void MotionViewer::updateLastLabel(qint32 newTime) {
    handle2Time->setText(convertTimeToString(newTime));
}

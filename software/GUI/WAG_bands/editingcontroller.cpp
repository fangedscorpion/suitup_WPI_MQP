#include "editingcontroller.h"
#include <QDebug>


#define MILLISECONDS_PER_FRAME 25

EditingController::EditingController() : QObject()
{
    playing = false;
    currentFrame = 0;
    lastFrameNum = 0;
    beginningPointer = 0;
    endPointer = 100;
    connect(this, SIGNAL(endOfTimeRange()), this, SLOT(togglePlay()));
    connect(this, SIGNAL(frameChanged(qint32)), this, SLOT(catchFrameUpdate(qint32)));

}

void EditingController::beginningSliderChanged(int sliderVal) {
    qDebug()<<"EditingController: beginning slider val: "<<sliderVal;
    beginningPointer = sliderVal;
    if (currentFrame < (beginningPointer*lastFrameNum/100)) {
        currentFrame = beginningPointer *lastFrameNum/100;
        emit frameChanged(currentFrame);
    }
}

void EditingController::endSliderChanged(int sliderVal) {
    qDebug()<<"EditingController: end slider val: "<<sliderVal;
    endPointer = sliderVal;
    if (currentFrame > (endPointer*lastFrameNum/100)) {
        currentFrame = endPointer *lastFrameNum/100;
        emit frameChanged(currentFrame);
    }
}

void EditingController::togglePlay() {
    if (!((!playing) && (currentFrame >= (std::min(lastFrameNum, (endPointer*lastFrameNum/100)))))) {
        playing = !playing;
        qDebug()<<"PlaybackController: Play status: "<<playing;
        if (playing) {
            startPlaying();
        }
        else {
            stopPlaying();
        }
        emit editingPlayStateChanged(playing);
    }
    // play motion

    qDebug()<<"EditingController: playing: "<<playing;
    emit editingPlayStateChanged(playing);
}

void EditingController::setActiveMotion(WAGFile *newMotion) {
    activeMotion = newMotion;
    qint32 sliderMax = activeMotion->getFrameNums();
    lastFrameNum = sliderMax;
    emit changeSliderMax(sliderMax);
    connect(newMotion, SIGNAL(framesChanged(qint32)), this, SLOT(catchFrameNumsChanged(qint32)));
}

void EditingController::startPlaying() {
    timerId = startTimer(MILLISECONDS_PER_FRAME);
    emit startPlayback();
}

void EditingController::timerEvent(QTimerEvent *event) {
    currentFrame += MILLISECONDS_PER_FRAME;
    if (currentFrame < (std::min(lastFrameNum, (endPointer*lastFrameNum/100)))) {
        emit frameChanged(currentFrame);
    }
    else {
        reachedEndOfTimeRange();
    }
}

void EditingController::stopPlaying() {
    killTimer(timerId);
    emit stopPlayback();
}


void EditingController::catchFrameNumsChanged(qint32 newLastNum) {
    lastFrameNum = newLastNum;
}

void EditingController::reachedEndOfTimeRange() {
    currentFrame = beginningPointer*lastFrameNum/100;
    emit frameChanged(currentFrame);
    emit endOfTimeRange();
}


void EditingController::catchFrameUpdate(qint32 newFrame) {
    if (lastFrameNum == 0) {
        emit changeSliderVal(0);
    } else {
        int newCurrentFrame = newFrame * 1000/lastFrameNum;
        emit changeSliderVal(newCurrentFrame);
    }
    PositionSnapshot desiredPos = activeMotion->getSnapshot(newFrame, CLOSEST);
    // should probably figure out how to handle null snapshots
    // TODO
    emit goToSnapshot(desiredPos);
}

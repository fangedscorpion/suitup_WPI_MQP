#include "editingcontroller.h"
#include <QDebug>


#define MILLISECONDS_PER_FRAME 25

EditingController::EditingController(QWidget *parent) : QObject(parent)
{
    playing = false;
    //currentFrame = 0;
    updater = new FrameUpdater(MILLISECONDS_PER_FRAME);
    lastFrameNum = 0;
    beginningPointer = 0;
    endPointer = 100;
    connect(this, SIGNAL(endOfTimeRange()), this, SLOT(togglePlay()));
    connect(this, SIGNAL(frameChanged(qint32)), this, SLOT(catchFrameUpdate(qint32)));
}

EditingController::~EditingController() {
    // activeMotion will be deleted by TabContent
    delete updater;
}

void EditingController::beginningSliderChanged(int sliderVal) {
    qDebug()<<"EditingController: beginning slider val: "<<sliderVal;
    beginningPointer = sliderVal;

    if (updater->getCurrentFrameNum() < (beginningPointer*lastFrameNum/100)) {
        updater->setCurrentFrameNum(beginningPointer *lastFrameNum/100);
        emit frameChanged(updater->getCurrentFrameNum());
    }
    emit beginningTimeChanged(beginningPointer*lastFrameNum/100);
}

void EditingController::endSliderChanged(int sliderVal) {
    qDebug()<<"EditingController: end slider val: "<<sliderVal;
    endPointer = sliderVal;

    if (updater->getCurrentFrameNum() > (endPointer*lastFrameNum/100)) {
        updater->setCurrentFrameNum(endPointer *lastFrameNum/100);
        emit frameChanged(updater->getCurrentFrameNum());
    }
    emit endTimeChanged(endPointer*lastFrameNum/100);
}

void EditingController::togglePlay() {
    if (!((!playing) && (updater->getCurrentFrameNum() >= (std::min(lastFrameNum, (endPointer*lastFrameNum/100)))))) {
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
    emit totalTimeChanged(lastFrameNum);
    emit changeSliderMax(sliderMax);
    connect(newMotion, SIGNAL(framesChanged(qint32)), this, SLOT(catchFrameNumsChanged(qint32)));
}

void EditingController::startPlaying() {
    timerId = startTimer(MILLISECONDS_PER_FRAME);
    updater->startFrameUpdates(MILLISECONDS_PER_FRAME);
    emit startPlayback();
}

void EditingController::timerEvent(QTimerEvent *) {
    //currentFrame += MILLISECONDS_PER_FRAME;
    qint32 frame = updater->getCurrentFrameNum();
    if (frame < (std::min(lastFrameNum, (endPointer*lastFrameNum/100)))) {
        emit frameChanged(frame);
    }
    else {
        reachedEndOfTimeRange();
    }
}

void EditingController::stopPlaying() {
    killTimer(timerId);
    updater->stopFrameUpdates();
    emit stopPlayback();
}


void EditingController::catchFrameNumsChanged(qint32 newLastNum) {
    lastFrameNum = newLastNum;
    emit totalTimeChanged(lastFrameNum);
}

void EditingController::reachedEndOfTimeRange() {
    updater->setCurrentFrameNum(beginningPointer*lastFrameNum/100);
    emit frameChanged(updater->getCurrentFrameNum());
    emit endOfTimeRange();
}


void EditingController::catchFrameUpdate(qint32 newFrame) {
    if (lastFrameNum == 0) {
        emit changeSliderVal(0);
    } else {
        int newCurrentFrame = newFrame * 1000/lastFrameNum;
        emit changeSliderVal(newCurrentFrame);
    }
    float approxPercentThroughFile = 0;
    if (lastFrameNum != 0) {
        approxPercentThroughFile = ((float) newFrame)/lastFrameNum;
    }
    PositionSnapshot *desiredPos = activeMotion->getSnapshot(approxPercentThroughFile, newFrame, CLOSEST);
    // should probably figure out how to handle null snapshots
    // TODO
    emit goToSnapshot(desiredPos);
}

// currentFrameSliderPos ranges from 0 to 1000
void EditingController::currentFrameChanged(int currentFrameSliderPos) {
    // may have to check here to see if frame in bounds (not sure where we want those checks)
    qDebug()<<"PlaybackController: currentFrame slider moved to "<<currentFrameSliderPos;
    updater->setCurrentFrameNum(currentFrameSliderPos*lastFrameNum/1000);

    //emit frameChanged(currentFrame);
    emit frameChanged(updater->getCurrentFrameNum());
}

qint32 EditingController::getBeginningHandleTime() {
    return beginningPointer*lastFrameNum/100;
}

qint32 EditingController::getEndingHandleTime() {
    return endPointer*lastFrameNum/100;
}

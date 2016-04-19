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
    emit frameChanged(updater->getCurrentFrameNum());
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
    int oldLastFrameNum = lastFrameNum;
    int newBeginningPointer;
    int newEndPointer;
    if (oldLastFrameNum != 0) {
        qint32 beginningTime = beginningPointer*oldLastFrameNum/100;
        qint32 endTime = endPointer*oldLastFrameNum/100;

        if (newLastNum != 0) {
            newBeginningPointer = std::min(beginningTime*100/newLastNum, 99);
            newEndPointer = std::min(endTime*100/newLastNum, 99);
        } else {
            // nothing?
            newEndPointer = 99;
            newBeginningPointer = 99;
        }
    } else {
        newEndPointer = 99;
        newBeginningPointer = 99;
    }

    emit beginningSliderPointerChanged(0);
    emit endSliderPointerChanged(99);

    lastFrameNum = newLastNum;
    emit totalTimeChanged(lastFrameNum);
    // need to change beginning pointer and end pointer here
    updater->setCurrentFrameNum(std::min(lastFrameNum, updater->getCurrentFrameNum()));
    emit frameChanged(updater->getCurrentFrameNum());
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
    qDebug()<<"frame emitted";
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
    if (beginningPointer == 0)
        return 0;
    return beginningPointer*lastFrameNum/100;
}

qint32 EditingController::getEndingHandleTime() {
    if (endPointer == 99)
        return lastFrameNum;
    return endPointer*lastFrameNum/100;
}

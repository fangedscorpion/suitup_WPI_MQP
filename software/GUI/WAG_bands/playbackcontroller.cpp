#include "playbackcontroller.h"
#include <QDebug>
#include <QString>
#include <math.h>
#include "wagfile.h"

#define HOLD_POSE_DEFAULT_MILLIS 500 // change later
#define STEP_THROUGH_INTERVAL_DEFAULT 30 // can change later,
// with this, user should match 1 frame
// out of every half second moved if motion recorded at 60fps
#define DEFAULT_FRAME_RATE 1 // how are we expressing frame rate? Multiplier by actual time progression (0.5 -> half speed)
#define DEFAULT_TOLERANCE 1 // how are we expressing tolerance
#define RECORDING_RATE 60 // 60 fps

#define MILLISECONDS_PER_FRAME 75


PlaybackController::PlaybackController(Suit *newSuitObj) {
    suitObj = newSuitObj;
    playing = false;
    stepThrough = false;
    frameRate = 1;
    voiceControl = false;
    suitActive = false;
    currentFrame = 0;
    timeToHoldFrameMillis = HOLD_POSE_DEFAULT_MILLIS;
    stepThroughInterval = STEP_THROUGH_INTERVAL_DEFAULT;
    stepThroughTolerance = DEFAULT_TOLERANCE; // how are we expressing this
    lastFrameNum = 1000; // TODO get from motion
    beginningPointer = 0;
    endPointer = 100;
    connect(this, SIGNAL(endOfTimeRange()), this, SLOT(togglePlay()));
    connect(suitObj, SIGNAL(voiceControlCommandReady(MessageType)), this, SLOT(catchVoiceControlCommand(MessageType)));
    connect(this, SIGNAL(frameChanged(qint32)), this, SLOT(catchFrameUpdate(qint32)));
    connect(this, SIGNAL(toleranceChanged(int)), suitObj, SLOT(catchToleranceChange(int)));
}

void PlaybackController::togglePlay() {
    //qDebug("PlaybackController: Toggling play");
    //qDebug()<<"PlaybackController: Last frame num: "<<lastFrameNum;
    //qDebug()<<"PlaybackController: Current frame num: "<<currentFrame;
    if (!((!playing) && (currentFrame >= (std::min(lastFrameNum, (endPointer*lastFrameNum/100)))))) {
        playing = !playing;
        //qDebug()<<"PlaybackController: Play status: "<<playing;
        if (playing) {
            startPlaying();
        }
        else {
            stopPlaying();
        }
        emit playbackStateChanged(playing);
        emit playingOnSuit(playing && suitActive);
    }
}

void PlaybackController::setStepThroughMode(bool steppingThrough) {
    stepThrough = steppingThrough;
}

void PlaybackController::changeFrameRate(float newFrameRate) {
    frameRate = newFrameRate;
}

void PlaybackController::toggleVoiceControl() {
    voiceControl = !voiceControl;
}

void PlaybackController::toggleSuitActive(bool active) {
    if (active != suitActive) {
        if (active) {
            connect(this, SIGNAL(goToSnapshot(PositionSnapshot)), suitObj, SLOT(playSnapshot(PositionSnapshot)));
            connect(this, SIGNAL(startPlayback()), suitObj, SLOT(catchStartPlayback()));
            connect(this, SIGNAL(stopPlayback()), suitObj, SLOT(catchStopPlayback()));
            connect(suitObj, SIGNAL(positionMet()), this, SLOT(positionMet()));
        } else {
            // disconnect everything from suit obj
            disconnect(this, SIGNAL(goToSnapshot(PositionSnapshot)), suitObj, 0);
            disconnect(this, SIGNAL(startPlayback()), suitObj, 0);
            disconnect(this, SIGNAL(stopPlayback()), suitObj, 0);
            disconnect(suitObj, SIGNAL(positionMet()), this, 0);
        }
    }
    suitActive = active;
    //qDebug()<<"PlaybackController: Suit activated: "<<suitActive;
}

// currentFrameSliderPos ranges from 0 to 1000
void PlaybackController::currentFrameChanged(int currentFrameSliderPos) {
    // may have to check here to see if frame in bounds (not sure where we want those checks)
    //qDebug()<<"PlaybackController: currentFrame slider moved to "<<currentFrameSliderPos;
    updateFrameWithoutSuitNotification(currentFrameSliderPos*lastFrameNum/1000);
}

void PlaybackController::modifyHoldTime(double holdSeconds) {
    // again, may have to add bounds checking here
    timeToHoldFrameMillis = holdSeconds*1000;
    qDebug()<<"PlaybackController: Holding last pose for "<<timeToHoldFrameMillis<<" milliseconds";
}

void PlaybackController::setStepThroughInterval(int newInterval) {
    // bounds checking...?
    stepThroughInterval = newInterval;
}

void PlaybackController::setStepThroughTolerance(float newTolerance) {
    // bounds checking
    stepThroughTolerance = newTolerance/10 + 1;
    qDebug()<<"PlaybackController: New tolerance "<<newTolerance;
    emit toleranceChanged(stepThroughTolerance);
}

// sliderPosition seems to be 0-99
void PlaybackController::updateStepThroughTolerance(int sliderPosition) {
    // scale tolerance
    setStepThroughTolerance((float) sliderPosition);
}

void PlaybackController::speedChanged(int sliderPosition) {
    if (stepThrough) {
        // modify slider position into interval value somehow
        float positionsToHoldPerSecond = pow(2, ((100.0 - sliderPosition)/25) - 1);
        // may want to round
        setStepThroughInterval(((float) RECORDING_RATE)/positionsToHoldPerSecond);
        qDebug()<<"PlaybackController: Step through interval "<<stepThroughInterval;
    }
    else {
        float exponent = sliderPosition - 50.0;
        // may want to round this result somehow
        changeFrameRate(pow(2, (exponent/25)));
        //qDebug()<<"PlaybackController: Playback speed is now "<<frameRate;
    }
}

void PlaybackController::startPlaying() {
    if (stepThrough) {
        emit startPlayback();
        if (suitActive) {
            qDebug("PlaybackController: start stepping through");
            emit frameChanged(currentFrame);
        } else {
            qDebug("PlaybackController: starting timer");
            // TODO: should probably slow this down
            int tempFrameRate = MILLISECONDS_PER_FRAME*stepThroughInterval;
            timerId = startTimer(tempFrameRate);
        }
    } else {
        timerId = startTimer(MILLISECONDS_PER_FRAME/frameRate);
        emit startPlayback();
    }
}

void PlaybackController::timerEvent(QTimerEvent *event) {
    if (stepThrough) {

        // TO DO remove once we can actually do position met
        emit metPosition();
    } else {
        currentFrame += MILLISECONDS_PER_FRAME;
        if (currentFrame < (std::min(lastFrameNum, (endPointer*lastFrameNum/100)))) {
            //qDebug()<<"PlaybackController: Current frame "<<currentFrame;
            emit frameChanged(currentFrame);
        }
        else {
            reachedEndOfTimeRange();
        }
    }
}

void PlaybackController::positionMet() {
    if (playing && stepThrough) {
        currentFrame += stepThroughInterval*MILLISECONDS_PER_FRAME;
        if (currentFrame < std::min(lastFrameNum, lastFrameNum*endPointer/100)) {
            qDebug()<<"PlaybackController: Position met Frame: "<<currentFrame;
            emit frameChanged(currentFrame);
        }
        else {
            qDebug()<<"PlaybackController: Reached end of time range in step through mode";
            reachedEndOfTimeRange();
        }
    }
}

void PlaybackController::stopPlaying() {
    if (stepThrough) {
        if (suitActive) {

        } else {
            killTimer(timerId);
        }
    }
    else {
        killTimer(timerId);
    }
    emit stopPlayback();
}

void PlaybackController::setActiveMotion(WAGFile *newMotion) {
    activeMotion = newMotion;
    qint32 sliderMax = activeMotion->getFrameNums();
    lastFrameNum = sliderMax;
    emit totalTimeChanged(lastFrameNum);
    emit changeSliderMax(sliderMax);
    connect(newMotion, SIGNAL(framesChanged(qint32)), this, SLOT(catchFrameNumsChanged(qint32)));
}

void PlaybackController::beginningSliderChanged(int sliderVal) {
    qDebug()<<"PlaybackController: beginning slider val: "<<sliderVal;
    beginningPointer = sliderVal;
    if (currentFrame < (beginningPointer*lastFrameNum/100)) {
        updateFrameWithoutSuitNotification(beginningPointer *lastFrameNum/100);
    }
    emit beginningTimeChanged(beginningPointer*lastFrameNum/100);
}

void PlaybackController::endSliderChanged(int sliderVal) {
    endPointer = sliderVal;
    if (currentFrame > (endPointer*lastFrameNum/100)) {
        updateFrameWithoutSuitNotification(endPointer *lastFrameNum/100);
    }
    emit endTimeChanged(endPointer*lastFrameNum/100);
}

void PlaybackController::catchFrameUpdate(qint32 newFrame) {
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
    PositionSnapshot desiredPos = activeMotion->getSnapshot(approxPercentThroughFile, newFrame, CLOSEST);
    // should probably figure out how to handle null snapshots
    // TODO
    emit goToSnapshot(desiredPos);
}

void PlaybackController::catchVoiceControlCommand(MessageType vcCommandInstruction) {
    if (voiceControl) {
        if (vcCommandInstruction == START_PLAYBACK) {
            if (!playing) {
                togglePlay();
            }
        } else if (vcCommandInstruction == STOP_PLAYBACK) {
            if (playing) {
                togglePlay();
            }
        }
    }
}

void PlaybackController::catchFrameNumsChanged(qint32 newLastNum) {
    lastFrameNum = newLastNum;
    emit totalTimeChanged(lastFrameNum);
}

void PlaybackController::reachedEndOfTimeRange() {
    updateFrameWithoutSuitNotification(beginningPointer*lastFrameNum/100);
    emit endOfTimeRange();
}

void PlaybackController::updateFrameWithoutSuitNotification(int newFrame) {
    bool actualSuitActive = suitActive;

    // if the suit is on, don't send this last frame update (which resets to the beginning)
    toggleSuitActive(false);
    // reset the pointer and the visualization to the beginning
    currentFrame = newFrame;
    emit frameChanged(currentFrame);
    // restore the suit's active state to what it was before we manually toggled it
    toggleSuitActive(actualSuitActive);
}

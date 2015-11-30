#include "playbackcontroller.h"
#include <QDebug>
#include <QString>
#include <math.h>
#include "motion.h"

#define HOLD_POSE_DEFAULT_MILLIS 500 // change later
#define STEP_THROUGH_INTERVAL_DEFAULT 30 // can change later,
// with this, user should match 1 frame
// out of every half second moved if motion recorded at 60fps
#define DEFAULT_FRAME_RATE 1 // how are we expressing frame rate? Multiplier by actual time progression (0.5 -> half speed)
#define DEFAULT_TOLERANCE 1 // how are we expressing tolerance
#define RECORDING_RATE 60 // 60 fps

#define MILLISECONDS_PER_FRAME 16


PlaybackController::PlaybackController()
{
    playing = false;
    stepThrough = true;
    frameRate = 1;
    voiceControl = false;
    suitActive = true;
    currentFrame = 0;
    timeToHoldFrameMillis = HOLD_POSE_DEFAULT_MILLIS;
    stepThroughInterval = STEP_THROUGH_INTERVAL_DEFAULT;
    stepThroughTolerance = DEFAULT_TOLERANCE; // how are we expressing this
    lastFrameNum = 1000;
    connect(this, SIGNAL(endOfTimeRange()), this, SLOT(togglePlay()));
}

void PlaybackController::togglePlay() {
    if (!((!playing) && (currentFrame >= lastFrameNum))) {
        playing = !playing;
        qDebug("toggle play");
        qDebug()<<"Play status: "<<playing;
        if (playing) {
            startPlaying();
        }
        else {
            stopPlaying();
        }
    }
}

void PlaybackController::setStepThroughMode(QString desiredPlaybackMode) {
    if (desiredPlaybackMode == "Step through mode") {
        stepThrough = true;
        qDebug("stepThroughMode");
    }
    else if (desiredPlaybackMode == "Timed mode") {
        stepThrough = false;
        qDebug("Timed mode");
    }
    else {
        qDebug("Neither option matched, please examine strings to match");
    }
}

void PlaybackController::changeFrameRate(float newFrameRate) {
    frameRate = newFrameRate;
}

void PlaybackController::toggleVoiceControl() {
    voiceControl = !voiceControl;
}

void PlaybackController::toggleSuitActive(bool active) {
    suitActive = active;
    qDebug()<<"Suit activated: "<<suitActive;
}

void PlaybackController::moveFramePointer(int newFrame) {
    // may have to check here to see if frame in bounds (not sure where we want those checks)
    if (newFrame >=0) {
        currentFrame = newFrame;
    }
}

void PlaybackController::modifyHoldTime(double holdSeconds) {
    // again, may have to add bounds checking here
    timeToHoldFrameMillis = holdSeconds*1000;
    qDebug()<<"Holding last pose for "<<timeToHoldFrameMillis<<" milliseconds";
}

void PlaybackController::setStepThroughInterval(int newInterval) {
    // bounds checking...?
    stepThroughInterval = newInterval;
}

void PlaybackController::setStepThroughTolerance(float newTolerance) {
    // bounds checking
    stepThroughTolerance = newTolerance;
}

// sliderPosition seems to be 0-99
void PlaybackController::updateStepThroughTolerance(int sliderPosition) {
    // scale tolerance
    qDebug()<<"Tolerance on slider: "<<sliderPosition;
    setStepThroughTolerance((float) sliderPosition);
}

void PlaybackController::speedChanged(int sliderPosition) {
    if (stepThrough) {
        // modify slider position into interval value somehow
        float positionsToHoldPerSecond = pow(2, ((100.0 - sliderPosition)/25) - 1);
        // may want to round
        setStepThroughInterval(((float) RECORDING_RATE)/positionsToHoldPerSecond);
        qDebug()<<"Step through interval "<<stepThroughInterval;
    }
    else {
        qDebug()<<"Slider pos "<<sliderPosition;
        float exponent = sliderPosition - 50.0;
        // may want to round this result somehow
        changeFrameRate(pow(2, (exponent/25)));
        qDebug()<<"Playback speed is now "<<frameRate;
    }
}

void PlaybackController::startPlaying() {
    if (stepThrough && !suitActive) {
        qDebug("Step through suit off");
    }
    else {
        timerId = startTimer(MILLISECONDS_PER_FRAME/frameRate);
    }
    emit startPlayback();
}

void PlaybackController::timerEvent(QTimerEvent *event) {
    currentFrame += 1;
    qDebug()<<"Current frame: "<<currentFrame<<", event: "<<event;
    if (currentFrame < lastFrameNum) {
        emit frameChanged(currentFrame);
    }
    else {
        emit endOfTimeRange();
    }
}

void PlaybackController::positionMet() {
    currentFrame += stepThroughInterval;
    if (currentFrame < lastFrameNum) {
        emit frameChanged(currentFrame);
    }
    else {
        emit endOfTimeRange();
    }
}

void PlaybackController::stopPlaying() {
    if (stepThrough && !suitActive) {

    }
    else {
        killTimer(timerId);
    }
    currentFrame = 0;
    emit stopPlayback();
}


Motion PlaybackController::loadMotionFrom(QString fileLocation) {
    return Motion();
}

bool playMotion(Motion motionToPlay) { // play from currentFrame to end
    return true;
}
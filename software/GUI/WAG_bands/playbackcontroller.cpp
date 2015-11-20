#include "playbackcontroller.h"
#include <QDebug>

#define HOLD_POSE_DEFAULT_MILLIS 500 // change later
#define STEP_THROUGH_INTERVAL_DEFAULT 30 // can change later,
// with this, user should match 1 frame
// out of every half second moved if motion recorded at 60fps
#define DEFAULT_FRAME_RATE 1 // how are we expressing frame rate? Multiplier by actual time progression (0.5 -> half speed)
#define DEFAULT_TOLERANCE 1 // how are we expressing tolerance


PlaybackController::PlaybackController()
{
    playing = false;
    stepThrough = false;
    frameRate = 1;
    voiceControl = false;
    suitActive = true;
    currentFrame = 0;
    timeToHoldFrameMillis = HOLD_POSE_DEFAULT_MILLIS;
    stepThroughInterval = STEP_THROUGH_INTERVAL_DEFAULT;
    stepThroughTolerance = DEFAULT_TOLERANCE; // how are we expressing this

}

void PlaybackController::togglePlay() {
    playing = !playing;
    qDebug("toggle play");
    qDebug()<<"Play status: "<<playing;
}

void PlaybackController::setStepThrough(bool shouldStepThrough) {
    stepThrough = shouldStepThrough;
}

void PlaybackController::changeFrameRate(float newFrameRate) {
    frameRate = newFrameRate;
}

void PlaybackController::toggleVoiceControl() {
    voiceControl = !voiceControl;
}

void PlaybackController::toggleSuitActive() {
    suitActive = !suitActive;
}

void PlaybackController::moveFramePointer(int newFrame) {
    // may have to check here to see if frame in bounds (not sure where we want those checks)
    if (newFrame >=0) {
        currentFrame = newFrame;
    }
}

void PlaybackController::modifyHoldTime(int newHoldTimeMillis) {
    // again, may have to add bounds checking here
    timeToHoldFrameMillis = newHoldTimeMillis;
}

void PlaybackController::setStepThroughInterval(int newInterval) {
    // bounds checking...?
    stepThroughInterval = newInterval;
}

void PlaybackController::setStepThroughTolerance(float newTolerance) {
    // bounds checking
    stepThroughTolerance = newTolerance;
}


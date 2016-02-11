#include "recordingcontroller.h"

// takes in record commands (stop/start recording), talks to suit,
RecordingController::RecordingController(Suit *newSuitObj) : QObject()
{
    suitObj = newSuitObj;
    connect(suitObj, SIGNAL(positionSnapshotReady(qint64,PositionSnapshot)), this, SLOT(addSnapshotToMotion(qint64, PositionSnapshot)));
}

void RecordingController::stopRecording() {
    // stop recording
    suitObj->startOrStopMode(STOP_RECORDING);
    activeMotion->setMotionData(currentMotionData);
}

void RecordingController::startRecording() {
    currentMotionData.clear();
    suitObj->startOrStopMode(START_RECORDING);
}

void RecordingController::addSnapshotToMotion(qint64 snapTime, PositionSnapshot snap) {
    qint32 newSnapTime = (qint32) snapTime;
    currentMotionData[newSnapTime] = snap;
}

void RecordingController::setActiveMotion(WAGFile *motion) {
    activeMotion = motion;
}



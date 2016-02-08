#include "recordingcontroller.h"

// takes in record commands (stop/start recording), talks to suit,
RecordingController::RecordingController() : QObject()
{

}

void RecordingController::setSuit(Suit *newSuitObj) {
    suitObj = newSuitObj;
    connect(suitObj, SIGNAL(positionSnapshotReady(qint64,PositionSnapshot)), this, SLOT(addSnapshotToMotion(qint64, PositionSnapshot)));
}

Motion RecordingController::stopRecording() {
    // start recording
}

void RecordingController::startRecording() {
    currentMotion.clear();
    suitObj->startOrStopMode(START_RECORDING);
}

void RecordingController::addSnapshotToMotion(qint64 snapTime, PositionSnapshot snap) {
    currentMotion[snapTime] = snap;
}



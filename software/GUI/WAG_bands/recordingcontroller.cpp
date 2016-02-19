#include "recordingcontroller.h"

// takes in record commands (stop/start recording), talks to suit,
RecordingController::RecordingController(Suit *newSuitObj) : QObject()
{
    suitObj = newSuitObj;
    connect(suitObj, SIGNAL(positionSnapshotReady(qint32,PositionSnapshot)), this, SLOT(addSnapshotToMotion(qint32, PositionSnapshot)));
    connect(suitObj, SIGNAL(voiceControlCommandReady(MessageType)), this, SLOT(catchVoiceControlCommand(MessageType)));
    voiceEnabled = false;
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

void RecordingController::addSnapshotToMotion(qint32 snapTime, PositionSnapshot snap) {
    qint32 newSnapTime = (qint32) snapTime;
    currentMotionData[newSnapTime] = snap;
}

void RecordingController::catchVoiceControlCommand(MessageType vcCommandInstruction) {
    if (voiceEnabled) {
        if (vcCommandInstruction == START_RECORDING) {
            startRecording();
        } else if (vcCommandInstruction == STOP_RECORDING) {
            stopRecording();
        }
    }
}

void RecordingController::setActiveMotion(WAGFile *motion) {
    activeMotion = motion;
}

void RecordingController::toggleVoiceControl(bool voiceControlOn) {
    voiceEnabled = voiceControlOn;
    qDebug()<<"Voice control enabled: "<<voiceEnabled;
}



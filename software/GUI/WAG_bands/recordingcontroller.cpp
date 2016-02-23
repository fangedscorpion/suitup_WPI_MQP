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
    qDebug("Setting motion data");
    qDebug()<<currentMotionData.keys().size();
    activeMotion->setMotionData(currentMotionData);
    qDebug()<<activeMotion->getFrameNums();
}

void RecordingController::startRecording() {
    currentMotionData.clear();
    suitObj->startOrStopMode(START_RECORDING);
}

void RecordingController::addSnapshotToMotion(qint32 snapTime, PositionSnapshot snap) {
    qint32 newSnapTime = snapTime;
    qDebug()<<newSnapTime<<"Adding snpashot to motion data";
    currentMotionData[newSnapTime] = snap;
    qDebug()<<currentMotionData.keys().size();
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



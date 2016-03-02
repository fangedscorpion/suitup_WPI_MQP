#include "recordingcontroller.h"

// takes in record commands (stop/start recording), talks to suit,
RecordingController::RecordingController(Suit *newSuitObj) : QObject()
{
    suitObj = newSuitObj;
    connect(suitObj, SIGNAL(positionSnapshotReady(qint32,PositionSnapshot)), this, SLOT(addSnapshotToMotion(qint32, PositionSnapshot)));
    connect(suitObj, SIGNAL(voiceControlCommandReady(MessageType)), this, SLOT(catchVoiceControlCommand(MessageType)));
    voiceEnabled = false;
    recording = false;
}

void RecordingController::stopRecording() {
    // stop recording
    suitObj->startOrStopMode(STOP_RECORDING);
    qDebug("RecordingController: Setting motion data");

    activeMotion->setMotionData(currentMotionData);
    qDebug()<<"RecordingController: totalFrames"<<activeMotion->getFrameNums();
    recording = false;
}

void RecordingController::startRecording() {
    recording = true;
    currentMotionData.clear();
    suitObj->startOrStopMode(START_RECORDING);
}

void RecordingController::addSnapshotToMotion(qint32 snapTime, PositionSnapshot snap) {
    if (recording) {
        qint32 newSnapTime = snapTime;
        //qDebug()<<"RecordingController: at time "<<newSnapTime<<" Adding snpashot to motion data";
        currentMotionData[newSnapTime] = snap;
    }
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
    qDebug()<<"RecordingController:Voice control enabled: "<<voiceEnabled;
}



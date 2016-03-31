#include "recordingcontroller.h"

// max file length is 10 minutes (give 1 extra second so we make sure we satisfy 10 minute requirement)
#define MAX_FILE_LENGTH 601000

// takes in record commands (stop/start recording), talks to suit,
RecordingController::RecordingController(QWidget *parent, Suit *newSuitObj) : QObject(parent)
{
    suitObj = newSuitObj;
    connect(suitObj, SIGNAL(positionSnapshotReady(qint32,PositionSnapshot*)), this, SLOT(addSnapshotToMotion(qint32, PositionSnapshot*)));
    connect(suitObj, SIGNAL(voiceControlCommandReady(MessageType)), this, SLOT(catchVoiceControlCommand(MessageType)));
    voiceEnabled = false;
    recording = false;
    maxRecordedTime = 0;
}

void RecordingController::stopRecording() {
    // stop recording
    suitObj->startOrStopMode(STOP_RECORDING_MODE);
    qDebug("RecordingController: Setting motion data");

    activeMotion->updateMotionData(currentMotionData);
    qDebug()<<"RecordingController: totalFrames"<<activeMotion->getFrameNums();
    recording = false;
}

void RecordingController::startRecording() {
    recording = true;
    maxRecordedTime = 0;
    currentMotionData.clear();
    suitObj->startOrStopMode(START_RECORDING_MODE);
}

void RecordingController::addSnapshotToMotion(qint32 snapTime, PositionSnapshot *snap) {
    if (recording) {
        qint32 newSnapTime = snapTime;
        maxRecordedTime = snapTime;        
        currentMotionData[newSnapTime] = snap;

        if (newSnapTime > MAX_FILE_LENGTH) {
            // stop recording (use voice control signalling protocol)
            emit vcChangeState(false);
        }
    }
}

void RecordingController::catchVoiceControlCommand(MessageType vcCommandInstruction) {
    if (voiceEnabled) {
        if (vcCommandInstruction == START_RECORDING) {
            emit vcChangeState(true);
            //startRecording();
        } else if (vcCommandInstruction == STOP_RECORDING) {
            emit vcChangeState(false);
            //stopRecording();
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

qint32 RecordingController::getMaxRecordedTime() {
    return maxRecordedTime;
}

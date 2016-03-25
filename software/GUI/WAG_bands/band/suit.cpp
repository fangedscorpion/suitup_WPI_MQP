#include "suit.h"
#include <QHash>
#include <stdexcept>

#define PING_INTERVAL 5000

Suit::Suit(WifiManager *comms, Model *suitModel):QObject() {

    model = suitModel;
    wifiMan = comms;
    startTime = QElapsedTimer();
    startTime.start();

    try {
        bands[CHEST] = new ChestBand();
        bands[LEFT_SHOULDER] = new ShoulderBand(LEFT_SHOULDER);
        bands[RIGHT_SHOULDER] = new ShoulderBand(RIGHT_SHOULDER);
        bands[LEFT_UPPER_ARM] = new ArmBand(LEFT_UPPER_ARM);
        bands[RIGHT_UPPER_ARM] = new ArmBand(RIGHT_UPPER_ARM);
        bands[LEFT_LOWER_ARM] = new ArmBand(LEFT_LOWER_ARM);
        bands[RIGHT_LOWER_ARM] = new ArmBand(RIGHT_LOWER_ARM);
        /* bands.insert(std::make_pair<BandType, AbsBand*>(RIGHT_HAND,
                                                     new Glove(RIGHT_HAND)));
        bands.insert(std::make_pair<BandType, AbsBand*>(LEFT_HAND,
                                                     new Glove(LEFT_HAND))); */

    } catch (const std::invalid_argument& e) {
        char str[100];
        strcpy(str, "Error in Suit's band: ");
        strcpy(str, e.what());
        throw std::logic_error(str);
    }

    connect(wifiMan, SIGNAL(dataAvailable(BandType,BandMessage*, QElapsedTimer)), this, SLOT(getRecvdData(BandType,BandMessage*,QElapsedTimer)));
    connect(wifiMan, SIGNAL(connectionStatusChanged(BandType,ConnectionStatus)), this, SLOT(handleConnectionStatusChange(BandType, ConnectionStatus)));

    QList<BandType> allBands = bands.keys();
    for (int i = 0; i < allBands.length(); i++) {
        bands[allBands[i]]->addNode(model->getNodeByName(AbsBand::bandTypeToModelName(allBands[i])).data());

        connect(bands[allBands[i]], SIGNAL(dataToSend(BandType,BandMessage*)), this, SLOT(sendData(BandType, BandMessage*)));
        connect(bands[allBands[i]], SIGNAL(poseRecvd(AbsState *,BandType,qint32)), this, SLOT(catchNewPose(AbsState *, BandType, qint32)));
        connect(this, SIGNAL(toleranceChanged(int)), bands[allBands[i]], SLOT(catchTolChange(int)));
        connect(bands[allBands[i]], SIGNAL(connectionProblem(BandType)), this, SLOT(catchConnectionProblem(BandType)));
        connect(bands[allBands[i]], SIGNAL(lowBattery(BandType, bool)), this, SLOT(propagateLowBatteryUpdate(BandType, bool)));
    }

    collectingData = true;
    toggleCollecting(false);
    activeSnapshot = new PositionSnapshot();
}


void Suit::getRecvdData(BandType band, BandMessage *data, QElapsedTimer dataTimestamp) {

    AbsBand *targetBand = getBand(band);
    // send data to target band
    qint64 elapsedTime = startTime.msecsTo(dataTimestamp);
    if ((data->getMessageType() == VOICE_CONTROL_LOW_BATT) || (data->getMessageType() == VOICE_CONTROL)) {
        processVoiceControlMessage(data);
    }
    targetBand->handleMessage((qint32) elapsedTime, data);

}

AbsBand* Suit::getBand(BandType bt) {
    return bands[bt];
}

void Suit::handleConnectionStatusChange(BandType band, ConnectionStatus newStatus) {
    bands[band]->handleConnectionStatusChange(newStatus);
}

void Suit::sendData(BandType destBand, BandMessage* sendMsg) {
    if (sendMsg->getMessageType() == POSITION_ERROR) {
        qDebug()<<"Sending error message";
    }
    wifiMan->sendMessageToBand(destBand, sendMsg);
}

void Suit::toggleCollecting(bool shouldCollectData) {
    if (collectingData != shouldCollectData) {
        collectingData = shouldCollectData;
        if (collectingData) {
            // stop timer
            //qDebug("Suit: Killing ping timer");
            killTimer(pingTimerID);
            activeSnapshot = new PositionSnapshot();
            activeSnapTimes.clear();
        } else {
            // start timer
            //qDebug("Suit: Starting ping timer");
            pingTimerID = startTimer(PING_INTERVAL);
        }
    }
}

void Suit::timerEvent(QTimerEvent *event) {
    BandMessage *newMsg = new BandMessage(COMPUTER_PING, QByteArray(""));
    qDebug("Suit: Ping timer event");
    sendToConnectedBands(newMsg);
}

void Suit::sendToConnectedBands(BandMessage *sendMsg) {
    QList<BandType> allBands = bands.keys();
    for (int i = 0; i < allBands.length(); i++) {
        bands[allBands[i]]->sendIfConnected(sendMsg);
    }
}

/*
 * starts or stops playback or recording, depending on the parameters
 * only messagetypes of Start/stop recording and start/stop playback should be used
 */
void Suit::startOrStopMode(MessageType commandType) {
    BandMessage *newMsg;
    switch (commandType) {
    case START_RECORDING:
        qDebug("Suit: starting record");
        toggleCollecting(true);
        startTime = QElapsedTimer();
        startTime.start();
        newMsg = new BandMessage(START_RECORDING, QByteArray());
        sendToConnectedBands(newMsg);
        break;
    case STOP_RECORDING:
        newMsg = new BandMessage(STOP_RECORDING, QByteArray());
        sendToConnectedBands(newMsg);
        qDebug("Suit: Stopping record");
        toggleCollecting(false);
        break;
    case START_PLAYBACK:
        qDebug("Suit: Starting playback");
        startTime = QElapsedTimer();
        startTime.start();
        toggleCollecting(true);
        newMsg = new BandMessage(START_PLAYBACK, QByteArray());
        sendToConnectedBands(newMsg);
        break;
    case STOP_PLAYBACK:
        newMsg = new BandMessage(STOP_PLAYBACK, QByteArray());
        sendToConnectedBands(newMsg);
        toggleCollecting(false);
        qDebug("Suit: stop playback");
        break;
    default:
        // shouldn't give any other message types
        break;
    }
}

void Suit::catchStartPlayback() {
    startOrStopMode(START_PLAYBACK);

}

void Suit::playSnapshot(PositionSnapshot *goToSnap) {
    //qDebug("Suit: Received snap to play\n");
    if (collectingData) {
        // TODO
        // probably want to set a snapshot to match, and then when we receive a full snapshot, we can compare
        // and send back error
        QList<BandType> connected = getConnectedBands().toList();
        QHash<BandType, AbsState*> snapshotData = goToSnap->getSnapshot();
        bool posWithinTol = true;
        for (int i = 0; i < connected.size(); i++){
            BandType getBand = connected[i];
            if (snapshotData.contains(getBand)) {
                //qDebug()<<"Suit: Sending error to band "<<getBand;
                posWithinTol &= bands[getBand]->moveTo(snapshotData[getBand]);
                //qDebug()<<"Suit: Position for band "<<getBand<<" within tolerance "<<posWithinTol;
            }
        }

        if (posWithinTol) {
            emit positionMet();
        }
    }
}

void Suit::catchStopPlayback() {
    startOrStopMode(STOP_PLAYBACK);
}

void Suit::catchModeChanged(ACTION_TYPE newMode) {
    qDebug()<<"Suit: mode changed to "<<newMode;
    currentMode = newMode;
}

void Suit::processVoiceControlMessage(BandMessage *msg) {
    qDebug()<<"Received voice control message";
    qDebug()<<"Suit: voice contrl data"<<msg->getMessageData();
    if (currentMode == PLAYBACK) {
        switch (msg->parseVoiceControlMsg()) {
        case VC_START:
            if (!collectingData) {
                // start playback
                qDebug()<<"Voice c ontrol start playback";
                emit voiceControlCommandReady(START_PLAYBACK);
            } else {
                // user error!
                qDebug()<<"User tried to start playback while playing";
            }
            break;
        case VC_STOP:
            if (collectingData) {
                // stop playback
                qDebug()<<"Voice control stop playback";
                emit voiceControlCommandReady(STOP_PLAYBACK);
            } else {
                // user error!
                qDebug()<<"User tried to stop playback while stopped";
            }
            break;
        default:
            // do nothing
            // error (user said wrong word or data wasn't properly transmitted)
            qDebug()<<"Invalid message type transmitted";
            break;
        }
    } else if (currentMode == RECORD) {
        switch (msg->parseVoiceControlMsg()) {
        case VC_START:
            if (!collectingData) {
                // start recording
                qDebug()<<"Voice control start recording";
                emit voiceControlCommandReady(START_RECORDING);
            } else {
                // user error!
                qDebug()<<"User tried to start recording while already recording";
            }
            break;
        case VC_STOP:
            if (collectingData) {
                qDebug()<<"VC stop recroding";
                emit voiceControlCommandReady(STOP_RECORDING);
                // start playback
            } else {
                // user error!
                qDebug()<<"User tried to stop recording while not recording";
            }
            break;
        default:
            // do nothing
            qDebug()<<"Invalid voice command type";
            break;
        }
    }
    // currently shouldn't do anything in edit mode
}

void Suit::propagateLowBatteryUpdate(BandType chargeBand, bool hasLowBattery) {
    emit bandHasLowBattery(chargeBand, hasLowBattery);
}


void Suit::catchNewPose(AbsState* newPose, BandType bandForPose, qint32 poseTime) {
    /* AbsState *copiedPose = (AbsState*) malloc(newPose->objectSize()); // not sure if can do this for abs
    // TODO figure out where to free this
    *copiedPose = *newPose; */

    // NOTE: may have to make sure changes to this absstate later are not reflected in position snapshot

    activeSnapshot->addMapping(bandForPose, newPose);

    activeSnapTimes.append(poseTime);

    // maybe just want to make it so it's all bands, not just the connected ones
    if (getConnectedBands() == activeSnapshot->getRecordedBands()) {
        // full snapshot!

        qint64 totalTime = 0;

        for (int i = 0; i < activeSnapTimes.length(); i++) {
            totalTime += activeSnapTimes[i];
        }
        qint32 avgReadingTime;
        if (activeSnapTimes.length() != 0) {
            avgReadingTime = (qint32) (totalTime/activeSnapTimes.length());
        }
        else {
            avgReadingTime = 0;
        }
        emit positionSnapshotReady(avgReadingTime, activeSnapshot);

        activeSnapshot = new PositionSnapshot();
        activeSnapTimes.clear();
    }
}


QSet<BandType> Suit::getConnectedBands() {
    QSet<BandType> connectedBands = QSet<BandType>();
    QList<BandType> possibleBands = bands.keys();
    for (int i = 0; i < possibleBands.length(); i++) {
        if (bands[possibleBands[i]]->isConnected()) {
            connectedBands += possibleBands[i];
        }
    }

    return connectedBands;
}

void Suit::catchToleranceChange(int newTol) {
    emit toleranceChanged(newTol);
}

void Suit::catchConnectionProblem(BandType bandWithProblem) {
    bands[bandWithProblem]->handleConnectionStatusChange(DISCONNECTED);
    emit bandConnectionStatusChanged(bandWithProblem, DISCONNECTED);
}

void Suit::calibrate() {
    QList<AbsBand*> bandArr = bands.values();
    for (int i = 0; i < bandArr.size(); ++i){
        if (bandArr[i]->isConnected()){
            bandArr[i]->calibrate();
        }
    }
}

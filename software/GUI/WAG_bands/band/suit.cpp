#include "suit.h"
#include <QHash>
#include <stdexcept>

#define PING_INTERVAL 5000

Suit::Suit(WifiManager *comms):QObject() {

    wifiMan = comms;
    startTime = QElapsedTimer();

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

    refBand = bands[CHEST];

    bands[CHEST]->addDependentBand(bands[RIGHT_SHOULDER]);
    bands[CHEST]->addDependentBand(bands[LEFT_SHOULDER]);
    bands[LEFT_SHOULDER]->addDependentBand(bands[LEFT_UPPER_ARM]);
    bands[RIGHT_SHOULDER]->addDependentBand(bands[RIGHT_UPPER_ARM]);
    bands[LEFT_UPPER_ARM]->addDependentBand(bands[LEFT_LOWER_ARM]);
    bands[RIGHT_UPPER_ARM]->addDependentBand(bands[RIGHT_LOWER_ARM]);
    /*bands[LEFT_LOWER_ARM]->addDependentBand(bands[LEFT_HAND]);
    bands[RIGHT_LOWER_ARM]->addDependentBand(bands[RIGHT_HAND]);*/

    connect(wifiMan, SIGNAL(dataAvailable(BandType,BandMessage*, QElapsedTimer)), this, SLOT(getRecvdData(BandType,BandMessage*,QElapsedTimer)));
    connect(wifiMan, SIGNAL(connectionStatusChanged(BandType,ConnectionStatus)), this, SLOT(handleConnectionStatusChange(BandType, ConnectionStatus)));

    QList<BandType> allBands = bands.keys();
    for (int i = 0; i < allBands.length(); i++) {
        connect(bands[allBands[i]], SIGNAL(dataToSend(BandType,BandMessage*)), this, SLOT(sendData(BandType, BandMessage*)));
    }

    collectingData = true;
    toggleCollecting(false);
}


void Suit::getRecvdData(BandType band, BandMessage *data, QElapsedTimer dataTimestamp) {
    // process QByteArray here or let individual band do it?
    // pass data to band
    AbsBand *targetBand = getBand(band);
    // send data to target band
    qDebug()<<band;

    qDebug()<<data->getMessageType();
    qint64 elapsedTime = startTime.msecsTo(dataTimestamp);
    targetBand->handleMessage(elapsedTime, data);

    //wifiMan->sendMessageToBand(band, newMsg);

}

QByteArray Suit::trimNewLineAtEnd(QByteArray trimFrom) {
    bool keepTrimming = true;
    while (keepTrimming) {
        if (trimFrom[trimFrom.length() - 1] == '\n') {
            trimFrom.remove(trimFrom.length() - 1, 1);
        }
        // uncomment if we should also remove trailing spaces
        //else if (trimFrom[trimFrom.length() - 1] == ' '){
        //    trimFrom.remove(trimFrom.length() - 1, 1);
        //}
        else  {
            keepTrimming = false;
        }
    }
    qDebug()<<trimFrom;
    return trimFrom;
}

QByteArray Suit::reverseByteArray(QByteArray reverseThis) {
    qDebug()<<"Reversing "<<reverseThis;
    for (int i = 0; i < reverseThis.length()/2; i++) {
        char tmp = reverseThis[i];
        reverseThis[i] = reverseThis[reverseThis.length() - 1 -i];
        reverseThis[reverseThis.length() - 1 -i] = tmp;
    }
    qDebug()<<"Reversed: "<<reverseThis;
    return reverseThis;
}


AbsBand* Suit::getBand(BandType bt) {
    return bands[bt];
}
// bool playback(vector<PositionSnapshot> motion)
// PositionSnapshot takeSnapshot( )
// void calibrate( )
// map<enum, BandCalibration> getCalibrationData( )

void Suit::handleConnectionStatusChange(BandType band, ConnectionStatus newStatus) {
    bands[band]->handleConnectionStatusChange(newStatus);
}
void Suit::sendData(BandType destBand, BandMessage* sendMsg) {
    wifiMan->sendMessageToBand(destBand, sendMsg);
}

void Suit::toggleCollecting(bool shouldCollectData) {
    if (collectingData != shouldCollectData) {
        collectingData = shouldCollectData;
        if (collectingData) {
            // stop timer
            qDebug("Killing timer");
            killTimer(pingTimerID);
        } else {
            // start timer
            qDebug("Starting timer");
            pingTimerID = startTimer(PING_INTERVAL);
        }
    }
}

void Suit::timerEvent(QTimerEvent *event) {
    BandMessage *newMsg = new BandMessage(COMPUTER_PING, QByteArray());
    qDebug("Timer event");
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
        toggleCollecting(true);
        startTime = QElapsedTimer();
        newMsg = new BandMessage(START_RECORDING, QByteArray());
        sendToConnectedBands(newMsg);
        break;
    case STOP_RECORDING:
        newMsg = new BandMessage(STOP_RECORDING, QByteArray());
        sendToConnectedBands(newMsg);
        toggleCollecting(false);
        break;
    case START_PLAYBACK:
        startTime = QElapsedTimer();
        toggleCollecting(true);
        newMsg = new BandMessage(START_PLAYBACK, QByteArray());
        sendToConnectedBands(newMsg);
        break;
    case STOP_PLAYBACK:
        newMsg = new BandMessage(STOP_PLAYBACK, QByteArray());
        sendToConnectedBands(newMsg);
        toggleCollecting(false);
        break;
    default:
        // shouldn't give any other message types
        break;
    }
}

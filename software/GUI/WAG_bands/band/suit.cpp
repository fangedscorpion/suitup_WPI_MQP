#include "suit.h"
#include <QHash>
#include <stdexcept>

Suit::Suit(WifiManager *comms):QObject() {

    wifiMan = comms;

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

    connect(wifiMan, SIGNAL(dataAvailable(BandType,BandMessage*, QTime)), this, SLOT(getRecvdData(BandType,BandMessage*,QTime)));
    connect(wifiMan, SIGNAL(connectionStatusChanged(BandType,ConnectionStatus)), this, SLOT(handleConnectionStatusChange(BandType, ConnectionStatus)));

    QList<BandType> allBands = bands.keys();
    for (int i = 0; i < allBands.length(); i++) {
        connect(bands[allBands[i]], SIGNAL(dataToSend(BandType,BandMessage*)), this, SLOT(sendData(BandType, BandMessage*)));
    }
}


void Suit::getRecvdData(BandType band, BandMessage *data, QTime dataTimestamp) {
    // process QByteArray here or let individual band do it?
    // pass data to band
    AbsBand *targetBand = getBand(band);
    // send data to target band
    qDebug()<<band;

    qDebug()<<data->getMessageType();
    qDebug()<<data->getMessageData();

    QByteArray newData = reverseByteArray(trimNewLineAtEnd(data->getMessageData()));
    qDebug()<<newData;

    BandMessage *newMsg = new BandMessage(data->getMessageType(), newData);

    wifiMan->sendMessageToBand(band, newMsg);

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

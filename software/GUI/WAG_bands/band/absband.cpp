#include "absband.h"
#include <cstddef>
#include <QDebug>

AbsBand::AbsBand(BandType bt):QObject() {
    type = bt;
    active = true;
    commsSetUp = false;
}

void AbsBand::handleConnectionStatusChange(ConnectionStatus newStatus) {
    if (newStatus == CONNECTED) {
        // send hi there message
        BandMessage *newMsg = new BandMessage(COMPUTER_INITIATE_CONNECTION, QByteArray());
        qDebug()<<"SENDING CONNECT MSG";
        qDebug()<<"BandT type: "<<type;
        emit dataToSend(type, newMsg);
    }
    else {
        // do we try to reconnect??
        commsSetUp = false;
    }
}

void AbsBand::handleMessage(qint64 msgTimestamp, BandMessage *recvdMessage) {
    switch(recvdMessage->getMessageType()) {
    case BAND_CONNECTING:
        if (!commsSetUp) {
            commsSetUp = true;
        }
        qDebug("recvd band connecting");
        break;
    case BAND_PING:
        if (pendingBandPing) {
            pendingBandPing = false;
        }
        qDebug("Recvd band ping");
        break;
    case BAND_POSITION_UPDATE:
        // should probably handle in subclass
        break;
    default:
        // should never receive a message that isn't one of these types
        break;
    }
}

void AbsBand::sendIfConnected(BandMessage *sendMsg) {
    if (commsSetUp) {
        if (sendMsg->getMessageType() == COMPUTER_PING) {
            pendingBandPing = true;
            qDebug("Sending band ping");
        }
        emit dataToSend(type, sendMsg);
    }
}

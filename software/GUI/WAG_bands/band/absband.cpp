#include "absband.h"
#include <cstddef>
#include <QTime>

AbsBand::AbsBand(BandType bt):QObject() {
    type = bt;
    active = true;
    commsSetUp = false;
}

void AbsBand::handleConnectionStatusChange(ConnectionStatus newStatus) {
    if (newStatus == CONNECTED) {
        // send hi there message
        BandMessage *newMsg = new BandMessage(COMPUTER_INITIATE_CONNECTION, QByteArray());
        emit dataToSend(type, newMsg);
    }
    else {
        // do we try to reconnect??
        commsSetUp = false;
    }
}

void AbsBand::handleMessage(QTime msgTimestamp, BandMessage *recvdMessage) {
    switch(recvdMessage->getMessageType()) {
    case BAND_CONNECTING:
        if (!commsSetUp) {
            commsSetUp = true;
        }
        break;
    case BAND_PING:
        if (pendingBandPing) {
            pendingBandPing = false;
        }
        break;
    case BAND_POSITION_UPDATE:
        // should probably handle in subclass
        break;
    default:
        // should never receive a message that isn't one of these types
        break;
    }
}




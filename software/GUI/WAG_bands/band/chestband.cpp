#include "absband.h"
#include "abspose.h"
#include <stdio.h>
#include <QDebug>
#include <QQuaternion>

// ChestBand class should probably not be extended

ChestBand::ChestBand() : AbsBand(CHEST) {}

bool ChestBand::moveTo(AbsPose* x) const {
    if (!isActive())
        return true;

    //qDebug()<<(static_cast<QQuaternion*>(x)->x)<<" \n";
    return true;
}

AbsPose ChestBand::getPose() const {
    // query for IMU position
    // parse value into Quaternion
}

void ChestBand::handleMessage(QTime msgTimestamp, BandMessage *recvdMessage) {
    switch(recvdMessage->getMessageType()) {
        case BAND_POSITION_UPDATE:
        // do something
        break;
    case VOICE_CONTROL:
        // do something else
        break;
    default:
        AbsBand::handleMessage(msgTimestamp, recvdMessage);
    }
}

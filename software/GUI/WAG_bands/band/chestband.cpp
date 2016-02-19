#include "absband.h"
#include "abspose.h"
#include <stdio.h>
#include <QDebug>
#include <QQuaternion>

// ChestBand class should probably not be extended

ChestBand::ChestBand() : AbsBand(CHEST) {}

bool ChestBand::moveTo(AbsState* x) const {
    if (!isActive())
        return true;

    //qDebug()<<(static_cast<QQuaternion*>(x)->x)<<" \n";
    return true;
}

AbsState *ChestBand::getStateUpdate() const {
    // query for IMU position
    // parse value into Quaternion
    return static_cast<QuatState*>(new QQuaternion());
}

void ChestBand::handleMessage(qint32 msgTimestamp, BandMessage *recvdMessage) {
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

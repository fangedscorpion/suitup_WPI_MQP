#include "absband.h"
#include "abspose.h"
#include <stdio.h>
#include <stdexcept>
#include <QQuaternion>
// ShoulderBand class should ONLY be extended into either:
// RIGHT_SHOULDER
// LEFT_SHOULDER

ShoulderBand::ShoulderBand(BandType b) : AbsBand(b) {
    if (b != LEFT_SHOULDER && b != RIGHT_SHOULDER) {
        throw std::invalid_argument("Created ShoulderBand with bad BandType");
    }
}

bool ShoulderBand::moveTo(AbsState* x) {
    if (!isActive())
        return true;

    return AbsBand::moveTo(x);
}

AbsState *ShoulderBand::getStateUpdate() const {
    // query for IMU position
    // parse value into Quaternion
    return static_cast<QuatState*>(new QQuaternion());
}

void ShoulderBand::handleMessage(qint32 msgTimestamp, BandMessage *recvdMessage) {
    switch(recvdMessage->getMessageType()) {
       // case BAND_POSITION_UPDATE:
        // do something
        //break;
    default:
        AbsBand::handleMessage(msgTimestamp, recvdMessage);
    }
}

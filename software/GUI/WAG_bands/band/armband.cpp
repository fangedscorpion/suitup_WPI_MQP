#include "absband.h"
#include "abspose.h"
#include <stdio.h>
#include <stdexcept>
// ArmBand class should ONLY be extended into either:
// LEFT_UPPER_ARM
// RIGHT_UPPER_ARM
// LEFT_LOWER_ARM or
// RIGHT_LOWER_ARM

ArmBand::ArmBand(BandType b) : AbsBand(b) {
    if (b != LEFT_LOWER_ARM && b != LEFT_UPPER_ARM &&
        b != RIGHT_LOWER_ARM && b != RIGHT_UPPER_ARM) {
        throw std::invalid_argument("Created ArmBand with bad BandType");
    }
}

bool ArmBand::moveTo(AbsState* x) const {
    if (!isActive())
        return true;

    //printf("%f \n", static_cast<Quaternion*>(x)->x * static_cast<Quaternion*>(getCalibrationPose())->x);
    return true;
}

AbsState *ArmBand::getStateUpdate() const {
    // query for IMU position
    // parse value into Quaternion
    return static_cast<QuatState*>(new QQuaternion());
}


void ArmBand::handleMessage(qint32 msgTimestamp, BandMessage *recvdMessage) {
    switch(recvdMessage->getMessageType()) {
        case BAND_POSITION_UPDATE:
        // do something
        break;
    default:
        AbsBand::handleMessage(msgTimestamp, recvdMessage);
    }
}


#include "absband.h"
#include "abspose.h"
#include <stdio.h>
#include <stdexcept>
#include "abserror.h"
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
    positionRep = QUAT_REP;

    pose = new QuatPose(QVector3D(1, 0, 0), QVector3D(0, 0, 1));
}

bool ArmBand::moveTo(AbsState* x) {
    if (!isActive())
        return true;
    return AbsBand::moveTo(x);
}

AbsState *ArmBand::getStateUpdate() const {
    // query for IMU position
    // parse value into Quaternion
    return static_cast<QuatState*>(new QQuaternion());
}


void ArmBand::handleMessage(qint32 msgTimestamp, BandMessage *recvdMessage) {
    AbsBand::handleMessage(msgTimestamp, recvdMessage);
}


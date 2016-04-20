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
    positionRep = QUAT_REP;

    switch (b) {
    case LEFT_SHOULDER:
        pose = new QuatPose(QVector3D(-1,0,0), QVector3D(0,-1,0), b);
        break;
    case RIGHT_SHOULDER:
        pose = new QuatPose(QVector3D(1, 0, 0), QVector3D(0, 0, 1), b); // X
        break;
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
    return new QuatState(QQuaternion());
}

void ShoulderBand::handleMessage(qint32 msgTimestamp, BandMessage *recvdMessage) {
    AbsBand::handleMessage(msgTimestamp, recvdMessage);
}

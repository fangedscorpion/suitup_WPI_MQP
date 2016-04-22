#include "absband.h"
#include "abspose.h"
#include <stdio.h>
#include <QDebug>
#include <QQuaternion>

// ChestBand class should probably not be extended

ChestBand::ChestBand() : AbsBand(CHEST) {
    positionRep = QUAT_REP;
    pose = new QuatPose(QVector3D(0,-1,0), QVector3D(0,0,-1), CHEST);
}

bool ChestBand::moveTo(AbsState* x) {
    if (!isActive())
        return true;

    return AbsBand::moveTo(x);
}

AbsState *ChestBand::getStateUpdate() const {
    // query for IMU position
    // parse value into Quaternion
    return new QuatState(QQuaternion());
}

void ChestBand::handleMessage(qint32 msgTimestamp, BandMessage *recvdMessage) {
    AbsBand::handleMessage(msgTimestamp, recvdMessage);
}

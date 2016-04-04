#include "absband.h"
#include "abspose.h"
#include <stdio.h>
#include <QDebug>
#include <QQuaternion>

// ChestBand class should probably not be extended

ChestBand::ChestBand() : AbsBand(CHEST) {
    positionRep = QUAT_REP;
    pose = new QuatPose(QVector3D(1, 0, 0), QVector3D(0, 0, 1));
}

bool ChestBand::moveTo(AbsState* x) {
    if (!isActive())
        return true;

    return AbsBand::moveTo(x);
}

AbsState *ChestBand::getStateUpdate() const {
    // query for IMU position
    // parse value into Quaternion
    return static_cast<QuatState*>(new QQuaternion());
}

void ChestBand::handleMessage(qint32 msgTimestamp, BandMessage *recvdMessage) {
    AbsBand::handleMessage(msgTimestamp, recvdMessage);
}

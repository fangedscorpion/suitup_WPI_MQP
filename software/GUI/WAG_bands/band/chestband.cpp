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

#include "absband.h"
#include "abspose.h"
#include <stdio.h>
// ChestBand class should probably not be extended

ChestBand::ChestBand() : AbsBand(CHEST) {}

bool ChestBand::moveTo(AbsPose* x) const {
    if (!isActive())
        return true;

    printf("%f \n", static_cast<Quaternion*>(x)->x);
    return true;
}

AbsPose ChestBand::getPose() const {
    // query for IMU position
    // parse value into Quaternion
}

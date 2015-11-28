#include "Band.h"
#include <stdio.h>
// ChestBand class should ONLY be extended into either:
// RIGHT_SHOULDER
// LEFT_SHOULDER

ChestBand::ChestBand() : Band(CHEST) {}

bool ChestBand::moveTo(Pose* x) const {
	if (!isActive())
		return true;

	printf("%f \n", static_cast<Quaternion*>(x)->x);
	return true;
}

Pose ChestBand::getPosition() const {
	// query for IMU position
	// parse value into Quaternion
}

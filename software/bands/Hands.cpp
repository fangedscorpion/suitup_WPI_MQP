#include "Band.h"
#include <stdio.h>
// Glove class should ONLY be extended into either:
// RIGHT_HAND
// LEFT_HAND

Glove::Glove(BandType b) : Band(b) {}

bool Glove::moveTo(Pose* x) const {
	if (!isActive())
		return true;

	printf("%f \n", static_cast<Quaternion*>(x)->x);
	return true;
}

Pose Glove::getPosition() const {
	// query for IMU position
	// parse value into Quaternion
}

#include "Band.h"
#include <stdio.h>
// ShoulderBand class should ONLY be extended into either:
// RIGHT_SHOULDER
// LEFT_SHOULDER

ShoulderBand::ShoulderBand(BandType b) : Band(b) {}

bool ShoulderBand::moveTo(Pose& x) const {
	if (!isActive())
		return true;

	printf("%f \n", static_cast<Quaternion&>(x).x);
	return true;
}

Pose ShoulderBand::getPosition() const {
	// query for IMU position
	// parse value into Quaternion
}

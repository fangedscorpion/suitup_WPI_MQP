#include "Band.h"
#include <stdio.h>
#include <stdexcept>
// ShoulderBand class should ONLY be extended into either:
// RIGHT_SHOULDER
// LEFT_SHOULDER

ShoulderBand::ShoulderBand(BandType b) : Band(b) {
	if (b != LEFT_SHOULDER && b != RIGHT_SHOULDER) {
		throw std::invalid_argument("Created ShoulderBand with bad BandType");
	}
}

bool ShoulderBand::moveTo(Pose* x) const {
	if (!isActive())
		return true;

	printf("%f \n", static_cast<Quaternion*>(x)->x);
	return true;
}

Pose ShoulderBand::getPose() const {
	// query for IMU position
	// parse value into Quaternion
}

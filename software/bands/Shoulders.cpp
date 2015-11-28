#include "Band.h"
#include <stdio.h>
// ShoulderBand class should ONLY be extended into either:
// RIGHT_SHOULDER
// LEFT_SHOULDER

ShoulderBand::ShoulderBand(BandType b) : Band(b) {
	if (b != LEFT_SHOULDER && b != RIGHT_SHOULDER) {
		printf("Created ShoulderBand with bad BandType\n");
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

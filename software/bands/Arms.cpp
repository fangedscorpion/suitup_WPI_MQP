#include "Band.h"
#include <stdio.h>
// ArmBand class should ONLY be extended into either:
// LEFT_UPPER_ARM
// RIGHT_UPPER_ARM
// LEFT_LOWER_ARM or
// RIGHT_LOWER_ARM

ArmBand::ArmBand(BandType b) : Band(b) {
	if (b != LEFT_LOWER_ARM && b != LEFT_UPPER_ARM &&
	    b != RIGHT_LOWER_ARM && b != RIGHT_UPPER_ARM) {
		printf("Created ArmBand with bad BandType\n");
	}
}

bool ArmBand::moveTo(Pose* x) const {
	if (!isActive())
		return true;

	printf("%f \n", static_cast<Quaternion*>(x)->x * static_cast<Quaternion*>(getCalibrationPose())->x);
	return true;
}

Pose ArmBand::getPose() const {
	// query for IMU position
	// parse value into Quaternion
	return Pose();
}

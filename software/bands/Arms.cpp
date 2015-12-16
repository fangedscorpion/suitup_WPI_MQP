#include "Band.h"
#include <stdio.h>
// ArmBand class should ONLY be extended into either:
// LEFT_UPPER_ARM
// RIGHT_UPPER_ARM
// LEFT_LOWER_ARM or
// RIGHT_LOWER_ARM

ArmBand::ArmBand(BandType b) : Band(b) {}

bool ArmBand::moveTo(Pose& x) const {
	if (!isActive())
		return true;

	printf("%f \n", static_cast<Quaternion&>(x).x);
	return true;
}

Pose ArmBand::getPosition() const {
	// query for IMU position
	// parse value into Quaternion
}

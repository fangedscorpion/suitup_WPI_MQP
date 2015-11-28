#include "Band.h"
#include <stdio.h>
// Glove class should ONLY be extended into either:
// RIGHT_HAND
// LEFT_HAND

Glove::Glove(BandType b) : Band(b) {
	if (b != LEFT_HAND && b != RIGHT_HAND) {
		printf("Created Glove with bad BandType\n");
	}
}

bool Glove::moveTo(Pose* x) const {
	if (!isActive())
		return true;

	printf("%f \n", static_cast<Quaternion*>(x)->x);
	return true;
}

Pose Glove::getPose() const {
	// query for IMU position
	// parse value into Quaternion
}

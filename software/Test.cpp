#include "bands/Band.h"
#include "stdio.h"
#include "Pose.h"
#include "Suit.h"

int main(int argc, char** argv) {
	Quaternion *p = new Quaternion(2.0, 3.0, 4.0, 5.0);
	Suit s = Suit();
	Band* b = s.getBand(LEFT_UPPER_ARM);
	b->setCalibrationPose(p);
	s.getBand(LEFT_SHOULDER)->getDependentBand(0)->moveTo(p);
	// printf("Pose x: %f\n", static_cast<Quaternion*>(b->getCalibrationPose())->x);
	// printf("Pose x: %f\n", static_cast<Quaternion*>(s.getBand(LEFT_LOWER_ARM)->getCalibrationPose())->x);

	return 1;
}

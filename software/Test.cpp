#include "bands/Band.h"
#include "stdio.h"
#include "Pose.h"
#include "Suit.h"

int main(int argc, char** argv) {
	Quaternion p = Quaternion(2.1, 3.1, 4.1, 5.1);
	// Band* b = new LeftLowerArm();
	Suit s = Suit();
	Band* b = s.getBand(LEFT_LOWER_ARM);
	b->moveTo(p);
	printf("test\n");
	return 1;
}

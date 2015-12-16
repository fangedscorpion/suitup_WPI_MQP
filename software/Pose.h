#pragma once

class Pose {
};

class Quaternion : public Pose {
public:
	Quaternion(float in_x, float in_y, float in_z, float in_w);
	float x, y, z, w;
};

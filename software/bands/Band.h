#include <vector>

#include "../Pose.h"
#pragma once

enum BandType {LEFT_HAND, RIGHT_HAND, LEFT_LOWER_ARM, RIGHT_LOWER_ARM,
				LEFT_UPPER_ARM, RIGHT_UPPER_ARM, LEFT_SHOULDER, RIGHT_SHOULDER,
				CHEST};

class Band {
private:
	BandType type;
	bool active;
	// the bands that rely on this band. ie) lower arm relies on upper arm.
	std::vector<Band*> dependentBands;
	// This is used to zero this band around the calibration pose
	Pose* calibrationPose;

public:
	Band(BandType bt);
	void addDependentBand(Band *band) {dependentBands.push_back(band); }
	Band* getDependentBand(int index) const {return dependentBands[index]; }
	int numDependentBands() {return dependentBands.size();}
	void setCalibrationPose(Pose* p) {calibrationPose = p; }
	Pose* getCalibrationPose() const {return calibrationPose; }
	BandType getType() const {return type;}
	virtual bool moveTo(Pose* x) const = 0;
	virtual Pose getPose() const = 0;
	void setActive(bool a) {active = a;}
	bool isActive() const {return active;}
};

class ArmBand : public Band {
public:
	ArmBand(BandType b);
	bool moveTo(Pose* x) const;
	Pose getPose() const;
};

class Glove : public Band {
public:
	Glove(BandType b);
	bool moveTo(Pose* x) const;
	Pose getPose() const;
};

class ShoulderBand : public Band {
public:
	ShoulderBand(BandType b);
	bool moveTo(Pose* x) const;
	Pose getPose() const;
};

class ChestBand : public Band {
public:
	ChestBand();
	bool moveTo(Pose* x) const;
	Pose getPose() const;
};

#include "../Pose.h"
#pragma once

enum BandType {LEFT_HAND, RIGHT_HAND, LEFT_LOWER_ARM, RIGHT_LOWER_ARM,
				LEFT_UPPER_ARM, RIGHT_UPPER_ARM, LEFT_SHOULDER, RIGHT_SHOULDER,
				CHEST};

class Band {
private:
	BandType type;
	bool active;
	// the band that this relies on. ie) lower arm relies on upper arm.
	// Band *previousBand;
	//BandCalibration data

public:
	Band(BandType bt);
	BandType getType() const {return type;}
	virtual bool moveTo(Pose& x) const = 0;
	virtual Pose getPosition() const = 0;
	void setActive(bool a) {active = a;}
	bool isActive() const {return active;}
};

class ArmBand : public Band {
public:
	ArmBand(BandType b);
	bool moveTo(Pose& x) const;
	Pose getPosition() const;
};

class Glove : public Band {
public:
	Glove(BandType b);
	bool moveTo(Pose& x) const;
	Pose getPosition() const;
};

class ShoulderBand : public Band {
public:
	ShoulderBand(BandType b);
	bool moveTo(Pose& x) const;
	Pose getPosition() const;
};

class ChestBand : public Band {
public:
	ChestBand();
	bool moveTo(Pose& x) const;
	Pose getPosition() const;
};

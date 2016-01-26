#ifndef ABSBAND_H
#define ABSBAND_H

#include <vector>

#include "abspose.h"


enum BandType {LEFT_HAND, RIGHT_HAND, LEFT_LOWER_ARM, RIGHT_LOWER_ARM,
                LEFT_UPPER_ARM, RIGHT_UPPER_ARM, LEFT_SHOULDER, RIGHT_SHOULDER,
                CHEST};

class AbsBand
{
public:
    AbsBand(BandType);
    void addDependentBand(AbsBand *band) {dependentBands.push_back(band); }
    AbsBand* getDependentBand(int index) const {return dependentBands[index]; }
    int numDependentBands() {return dependentBands.size();}
    void setCalibrationPose(AbsPose* p) {calibrationPose = p; }
    AbsPose* getCalibrationPose() const {return calibrationPose; }
    BandType getType() const {return type;}
    virtual bool moveTo(AbsPose* x) const = 0;
    virtual AbsPose getPose() const = 0;
    void setActive(bool a) {active = a;}
    bool isActive() const {return active;}

private:
    BandType type;
    bool active;
    // the bands that rely on this band. ie) lower arm relies on upper arm.
    std::vector<AbsBand*> dependentBands;
    // This is used to zero this band around the calibration pose
    AbsPose* calibrationPose;
};


class ArmBand : public AbsBand {
public:
    ArmBand(BandType b);
    bool moveTo(AbsPose* x) const;
    AbsPose getPose() const;
};

class Glove : public AbsBand {
public:
    Glove(BandType b);
    bool moveTo(AbsPose* x) const;
    AbsPose getPose() const;
};

class ShoulderBand : public AbsBand {
public:
    ShoulderBand(BandType b);
    bool moveTo(AbsPose* x) const;
    AbsPose getPose() const;
};

class ChestBand : public AbsBand {
public:
    ChestBand();
    bool moveTo(AbsPose* x) const;
    AbsPose getPose() const;
};


#endif // ABSBAND_H

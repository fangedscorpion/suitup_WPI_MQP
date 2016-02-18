#ifndef ABSBAND_H
#define ABSBAND_H

#include <vector>

#include "abspose.h"
#include "communications/bandmessage.h"
#include <QObject>


enum BandType {LEFT_HAND, RIGHT_HAND, LEFT_LOWER_ARM, RIGHT_LOWER_ARM,
                LEFT_UPPER_ARM, RIGHT_UPPER_ARM, LEFT_SHOULDER, RIGHT_SHOULDER,
                CHEST,_NULL};

class AbsBand:public QObject
{
    Q_OBJECT
public:
    AbsBand(BandType);

    // child and parent bands
    void addChildBand(AbsBand *band) {childBands.push_back(band); band->setParent(this);}
    AbsBand* getChildBand(int index) const {return childBands[index]; }
    int numDependentBands() {return childBands.size();}
    void setParentBand(AbsBand* band) {parentBand = band;}
    AbsBand* getParentBand() {return parentBand;}

    // updating the pose
    void updateState(AbsState* state);
    void updatePoints();
    void setCalibrationState(AbsState* refState) {pose->calibrate(refState);}
    virtual QVector3D* getEndpoint() const {return pose->getEndpoint();}
    AbsState* getCalibrationState() const {return pose->getCalibrationState(); }
    virtual AbsState* getState() const {return pose->getState();}
    virtual AbsState* getStateUpdate() const = 0;

    BandType getType() const {return type;}
    virtual bool moveTo(AbsState* x) const = 0;
    void setActive(bool a) {active = a;}
    bool isActive() const {return active;}
    void handleConnectionStatusChange(ConnectionStatus);
    void handleMessage(qint64, BandMessage *);
    void sendIfConnected(BandMessage *sendMsg);

private:
    BandType type;
    bool active;

    AbsBand* parentBand;
    std::vector<AbsBand*> childBands;
    AbsPose* pose;

    bool commsSetUp;
    bool pendingBandPing;
signals:
    void dataToSend(BandType, BandMessage *);
    void lowBattery(BandType);
};


class ArmBand : public AbsBand {
public:
    ArmBand(BandType b);
    void handleMessage(qint64, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x) const;
};

class Glove : public AbsBand {
public:
    Glove(BandType b);
    void handleMessage(qint64, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x) const;
};

class ShoulderBand : public AbsBand {
public:
    ShoulderBand(BandType b);
    void handleMessage(qint64, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x) const;
};

class ChestBand : public AbsBand {
public:
    ChestBand();
    void handleMessage(qint64, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x) const;
};

class NullBand : public AbsBand {
public:
    NullBand();
    AbsState* getStateUpdate() const {return NULL;}
    bool moveTo(AbsState* x) const {return false;}
    AbsState* getState();
    QVector3D getEndpoint();
};

#endif // ABSBAND_H

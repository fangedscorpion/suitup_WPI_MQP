#ifndef ABSBAND_H
#define ABSBAND_H

#include <vector>

#include "abspose.h"
#include "communications/bandmessage.h"
#include <QObject>
#include "abserror.h"


enum BandType {LEFT_HAND, RIGHT_HAND, LEFT_LOWER_ARM, RIGHT_LOWER_ARM,
                LEFT_UPPER_ARM, RIGHT_UPPER_ARM, LEFT_SHOULDER, RIGHT_SHOULDER,
                CHEST,_NULL};
QDataStream & operator>>(QDataStream & str, BandType & v);

enum PositionRepresentation {QUATERNION};

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
    void updateState(AbsState* state, qint32 msgTime);
    void updatePoints();
    void setCalibrationState(AbsState* refState) {pose->calibrate(refState);}
    virtual QVector3D* getEndpoint() const {return pose->getEndpoint();}
    AbsState* getCalibrationState() const {return pose->getCalibrationState(); }
    virtual AbsState* getState() const {return pose->getState();}
    virtual AbsState* getStateUpdate() const = 0;

    BandType getType() const {return type;}
    bool moveTo(AbsState* x);
    void setActive(bool a) {active = a;}
    bool isActive() const {return active;}
    void handleConnectionStatusChange(ConnectionStatus);
    void handleMessage(qint32, BandMessage *);
    void sendIfConnected(BandMessage *sendMsg);
    bool isConnected();
    PositionRepresentation getPositionRepresentation() { return positionRep; }

    static AbsState *deserialize(QByteArray byteRep, PositionRepresentation positionRep);

public slots:
    void catchTolChange(int newTol);

protected:
    AbsPose *pose;
private:
    BandType type;
    PositionRepresentation positionRep;
    bool active;

    AbsBand* parentBand;
    std::vector<AbsBand*> childBands;
    qint32 poseRecvdTime;

    bool commsSetUp;
    bool pendingBandPing;
    int tolerance;
signals:
    void dataToSend(BandType, BandMessage *);
    void lowBattery(BandType);
    void poseRecvd(AbsState *, BandType, qint32);
    void withinTolerance(BandType);
};


class ArmBand : public AbsBand {
public:
    ArmBand(BandType b);
    void handleMessage(qint32, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x);
};

/* class Glove : public AbsBand {
public:
    Glove(BandType b);
    void handleMessage(qint32, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x);
}; */

class ShoulderBand : public AbsBand {
public:
    ShoulderBand(BandType b);
    void handleMessage(qint32, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x);
};

class ChestBand : public AbsBand {
public:
    ChestBand();
    void handleMessage(qint32, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x);
};

class NullBand : public AbsBand {
public:
    NullBand();
    AbsState* getStateUpdate() const {return NULL;}
    bool moveTo(AbsState* x) {return false;}
    AbsState* getState();
    QVector3D getEndpoint();
};

#endif // ABSBAND_H

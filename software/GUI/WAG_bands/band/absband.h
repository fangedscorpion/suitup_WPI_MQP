#ifndef ABSBAND_H
#define ABSBAND_H

#include <vector>

#include "abspose.h"
#include "communications/bandmessage.h"
#include <QObject>
#include "abserror.h"

#define PING_PROBLEMS_THRESHOLD 5

QDataStream & operator>>(QDataStream & str, BandType & v);

class AbsBand:public QObject
{
    Q_OBJECT
public:
    AbsBand(BandType);
    ~AbsBand();
    // updating the pose
    void updateState(AbsState* state, qint32 msgTime);
    void calibrate() {pose->calibrate();}
    virtual AbsState* getState() const {return pose->getState();}
    virtual AbsState* getStateUpdate() const = 0;

    void addNode(Node* node) {pose->addNode(node);}

    BandType getType() const {return type;}
    bool moveTo(AbsState* x);
    void setActive(bool a) {active = a;}
    bool isActive() const {return active;}
    void handleConnectionStatusChange(ConnectionStatus);
    void handleMessage(qint32, BandMessage *);
    bool sendIfConnected(BandMessage *sendMsg);
    bool isConnected();
    PositionRepresentation getPositionRepresentation() { return positionRep; }

    static AbsState *deserialize(QByteArray byteRep, PositionRepresentation positionRep);
    static QString bandTypeToString(BandType stringifyThis);
    static QString bandTypeToModelName(BandType b);
    void invalidateData();

public slots:
    void catchTolChange(int newTol);

protected:
    AbsPose *pose;
    PositionRepresentation positionRep;
private:
    BandType type;
    bool active;
    bool hasLowBattery;

    qint32 poseRecvdTime;

    bool commsSetUp;
    bool pendingBandPing;
    int tolerance;
    int pingProblems;
    bool validData;
signals:
    void dataToSend(BandType, BandMessage *);
    void lowBattery(BandType, bool);
    void poseRecvd(AbsState *, BandType, qint32);
    void withinTolerance(BandType);
    void connectionProblem(BandType);
};


class ArmBand : public AbsBand {
public:
    ArmBand(BandType b);
    ~ArmBand(){}
    void handleMessage(qint32, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x);
};

class ShoulderBand : public AbsBand {
public:
    ShoulderBand(BandType b);
    ~ShoulderBand(){}
    void handleMessage(qint32, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x);
};

class ChestBand : public AbsBand {
public:
    ChestBand();
    ~ChestBand(){}
    void handleMessage(qint32, BandMessage *);
    AbsState* getStateUpdate() const;
    bool moveTo(AbsState* x);
};

#endif // ABSBAND_H

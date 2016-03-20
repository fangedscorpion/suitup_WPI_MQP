#ifndef SUIT_H
#define SUIT_H

#include "communications/wifimanager.h"
#include <map>
#include "band/absband.h"
#include <QObject>
#include <QHash>
#include "communications/bandmessage.h"
#include "positionsnapshot.h"
#include "user.h"
#include "model/model.h"

class Suit:public QObject
{
    Q_OBJECT
public:
    Suit(WifiManager* comms, Model *suitModel);
    AbsBand* getBand(BandType bt);
    // bool playback(vector<PositionSnapshot> motion)
    // PositionSnapshot takeSnapshot( )
    // void calibrate( )
    // map<enum, BandCalibration> getCalibrationData( )
    void startOrStopMode(MessageType);
    Model* getModel() {return model;}

private:
    QHash<BandType, AbsBand*> bands;
    WifiManager *wifiMan;
    ACTION_TYPE currentMode;
    bool collectingData;
    int pingTimerID;
    void toggleCollecting(bool);
    void sendToConnectedBands(BandMessage*);
    QElapsedTimer startTime;
    void processVoiceControlMessage(BandMessage *msg);
    QList<qint32> activeSnapTimes;
    PositionSnapshot *activeSnapshot;
    QSet<BandType> getConnectedBands();
    Model* model;

public slots:
    void catchStartPlayback();
    void catchStopPlayback();
    void playSnapshot(PositionSnapshot *);
    void propagateLowBatteryUpdate(BandType, bool);

    void catchNewPose(AbsState* newPose, BandType bandForPose, qint32 poseTime);
    void catchToleranceChange(int);
    void catchConnectionProblem(BandType bandWithProblem);

    void calibrate();

signals:
    void positionSnapshotReady(qint32, PositionSnapshot*);
    void voiceControlCommandReady(MessageType);
    void bandHasLowBattery(BandType, bool);
    void toleranceChanged(int);
    void positionMet();
    void bandConnectionStatusChanged(BandType, ConnectionStatus);
protected:
    void timerEvent(QTimerEvent *);

private slots:
    void sendData(BandType destBand, BandMessage* sendMsg);
    void getRecvdData(BandType band, BandMessage *data, QElapsedTimer dataTimestamp);
    void handleConnectionStatusChange(BandType band, ConnectionStatus newStatus);
    void catchModeChanged(ACTION_TYPE);

};

#endif // SUIT_H

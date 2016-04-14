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

enum StartStopModeType {START_PLAYBACK_MODE, STOP_PLAYBACK_MODE, START_RECORDING_MODE, STOP_RECORDING_MODE, START_CALIBRATION_MODE, STOP_CALIBRATION_MODE};

class Suit:public QObject
{
    Q_OBJECT
public:
    Suit(WifiManager* comms, Model *suitModel);
    ~Suit();
    AbsBand* getBand(BandType bt);

    QSet<BandType> getConnectedBands();
    // bool playback(vector<PositionSnapshot> motion)
    // PositionSnapshot takeSnapshot( )
    // void calibrate( )
    // map<enum, BandCalibration> getCalibrationData( )
    void startOrStopMode(StartStopModeType);
    Model* getModel() {return model;}
    void startCollecting();
    void stopCollecting();

private:
    QHash<BandType, AbsBand*> bands;
    WifiManager *wifiMan;
    DISPLAY_TYPE currentMode;
    bool collectingData;
    int pingTimerID;
    void toggleCollecting(bool);
    void sendToConnectedBands(BandMessage*);
    QElapsedTimer startTime;
    void processVoiceControlMessage(BandMessage *msg);
    QList<qint32> activeSnapTimes;
    PositionSnapshot *activeSnapshot;
    Model* model;
    StartStopModeType lastActiveMode;
    qint32 lastTimeEmitted;

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
    void voiceActionCommandRecvd();
protected:
    void timerEvent(QTimerEvent *);

private slots:
    void sendData(BandType destBand, BandMessage* sendMsg);
    void getRecvdData(BandType band, BandMessage *data, QElapsedTimer dataTimestamp);
    void handleConnectionStatusChange(BandType band, ConnectionStatus newStatus);
    void catchModeChanged(DISPLAY_TYPE);

};

#endif // SUIT_H

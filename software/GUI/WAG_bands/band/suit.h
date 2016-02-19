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

class Suit:public QObject
{
    Q_OBJECT
public:
    Suit(WifiManager* comms);
    AbsBand* getBand(BandType bt);
    // bool playback(vector<PositionSnapshot> motion)
    // PositionSnapshot takeSnapshot( )
    // void calibrate( )
    // map<enum, BandCalibration> getCalibrationData( )
    AbsBand* getRefBand() const {return refBand;};
    void startOrStopMode(MessageType);

private:
    QHash<BandType, AbsBand*> bands;
    AbsBand* refBand;
    WifiManager *wifiMan;
    ACTION_TYPE currentMode;

    QByteArray trimNewLineAtEnd(QByteArray trimFrom);

    QByteArray reverseByteArray(QByteArray reverseThis);
    bool collectingData;
    int pingTimerID;
    void toggleCollecting(bool);
    void sendToConnectedBands(BandMessage*);
    QElapsedTimer startTime;
    void processVoiceControlMessage(BandMessage *msg);
    QList<qint32> activeSnapTimes;
    PositionSnapshot activeSnapshot;
    QSet<BandType> getConnectedBands();

public slots:
    void catchStartPlayback();
    void catchStopPlayback();
    void playSnapshot(PositionSnapshot);
    void propagateLowBattery(BandType);

    void catchNewPose(AbsState* newPose, BandType bandForPose, qint32 poseTime);
    void catchToleranceChange(int);
signals:
    void positionSnapshotReady(qint32, PositionSnapshot);
    void voiceControlCommandReady(MessageType);
    void bandHasLowBattery(BandType);
    void toleranceChanged(int);
    void positionMet();
protected:
    void timerEvent(QTimerEvent *);

private slots:
    void sendData(BandType destBand, BandMessage* sendMsg);
    void getRecvdData(BandType band, BandMessage *data, QElapsedTimer dataTimestamp);
    void handleConnectionStatusChange(BandType band, ConnectionStatus newStatus);
    void catchModeChanged(ACTION_TYPE);

};

#endif // SUIT_H

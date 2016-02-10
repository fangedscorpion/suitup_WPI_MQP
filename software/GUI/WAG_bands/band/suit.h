#ifndef SUIT_H
#define SUIT_H

#include "communications/wifimanager.h"
#include <map>
#include "band/absband.h"
#include <QObject>
#include <QHash>
#include "communications/bandmessage.h"
#include "positionsnapshot.h"

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

    QByteArray trimNewLineAtEnd(QByteArray trimFrom);

    QByteArray reverseByteArray(QByteArray reverseThis);
    bool collectingData;
    int pingTimerID;
    void toggleCollecting(bool);
    void sendToConnectedBands(BandMessage*);
    QElapsedTimer startTime;

public slots:
    void catchStartPlayback();
    void catchStopPlayback();
    void playSnapshot(PositionSnapshot);
signals:
    void positionSnapshotReady(qint64, PositionSnapshot *);
protected:
    void timerEvent(QTimerEvent *);

private slots:
    void sendData(BandType destBand, BandMessage* sendMsg);
    void getRecvdData(BandType band, BandMessage *data, QElapsedTimer dataTimestamp);
    void handleConnectionStatusChange(BandType band, ConnectionStatus newStatus);

};

#endif // SUIT_H

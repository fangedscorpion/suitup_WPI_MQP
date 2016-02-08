#ifndef SUIT_H
#define SUIT_H

#include "communications/wifimanager.h"
#include <map>
#include "band/absband.h"
#include <QObject>
#include <QHash>
#include "communications/bandmessage.h"

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
protected:
    void timerEvent(QTimerEvent *);

private slots:
    void sendData(BandType destBand, BandMessage* sendMsg);
    void getRecvdData(BandType band, BandMessage *data, QTime dataTimestamp);
    void handleConnectionStatusChange(BandType band, ConnectionStatus newStatus);

};

#endif // SUIT_H

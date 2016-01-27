#ifndef SUIT_H
#define SUIT_H

#include "communications/wifimanager.h"
#include <map>
#include "band/absband.h"
#include <QObject>
#include <QHash>

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

private slots:
    void getRecvdData(BandType band, QByteArray data, QTime dataTimestamp);
};

#endif // SUIT_H

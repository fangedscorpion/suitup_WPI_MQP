#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H


#define CHEST_BAND_PORT 12930
#define RIGHT_SHOULDER_PORT 12992
#define RIGHT_UPPER_ARM_PORT 13028
#define RIGHT_FOREARM_PORT 12949
#define LEFT_SHOULDER_PORT 12941
#define LEFT_UPPER_ARM_PORT 13491
#define LEFT_FOREARM_PORT 13291

#define SERVER_PORT 14273

#include <QObject>
#include <QtNetwork>
#include "band/absband.h"

class WifiManager:public QObject
{
    Q_OBJECT
public:
    // consider having wifimanager take in the suit object here
    WifiManager();
    void startListening();
    void sendToBand(BandType destBand, QByteArray data);
    void sendToBand(BandType destBand, char * bandData);

private:
    QTcpServer *serv;
    QHash<BandType, QTcpSocket*> socketMap;
    void routeToBandObject(BandType bandWithData);
    QByteArray trimNewLineAtEnd(QByteArray trimFrom);
    QByteArray reverseByteArray(QByteArray reverseThis);
private slots:
    void connectToNewDevice();
    void checkForData();
};

#endif // WIFIMANAGER_H

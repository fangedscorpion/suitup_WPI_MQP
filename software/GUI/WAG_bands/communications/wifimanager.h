#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H


#define CHEST_BAND_PORT 12930
#define RIGHT_SHOULDER_PORT 12992
#define RIGHT_UPPER_ARM_PORT 13028
#define RIGHT_FOREARM_PORT 12949
#define LEFT_SHOULDER_PORT 12941
#define LEFT_UPPER_ARM_PORT 13491
#define LEFT_FOREARM_PORT 13291

#define CHEST_IP_END 200
#define RIGHT_SHOULDER_IP_END 205
#define RIGHT_UPPER_ARM_IP_END 207
#define RIGHT_FOREARM_IP_END 209
#define LEFT_SHOULDER_IP_END 222
#define LEFT_UPPER_ARM_IP_END 224
#define LEFT_FOREARM_IP_END 226


#define SERVER_PORT 14273

#include <QObject>
#include <QtNetwork>
#include "band/absband.h"
#include <QSignalMapper>

class WifiManager:public QObject
{
    Q_OBJECT
public:
    // consider having wifimanager take in the suit object here
    WifiManager();
    void initiateConnection();
    void sendToBand(BandType destBand, QByteArray data);
    void sendToBand(BandType destBand, char * bandData);

signals:
    void dataAvailable(BandType recvdFrom, QByteArray dataRecvd, QTime timestamp);

private:
    QTcpServer *serv;
    QHash<BandType, QTcpSocket*> socketMap;
    void routeToBandObject(BandType bandWithData);
    QByteArray trimNewLineAtEnd(QByteArray trimFrom);
    QByteArray reverseByteArray(QByteArray reverseThis);
    void startSingleConnection(BandType bandToConnect);
    QSignalMapper *connectedMapper;
    QSignalMapper *recvdMapper;
    QHash<BandType, QString> ipMap;
    QHash<BandType, quint16> portMap;

private slots:
   // void connectToNewDevice();
    void checkForData(int);
    void socketConnected(int);
    void socketError(int);
};

#endif // WIFIMANAGER_H

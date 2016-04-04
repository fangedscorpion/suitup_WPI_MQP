#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H



// for using with command line, or if we revert from static IPs

#include "localtesting.h"
#include "incorrectdatalengthexception.h"

#ifdef TESTING
#define CHEST_BAND_PORT 12930
#define RIGHT_SHOULDER_PORT 12992
#define RIGHT_UPPER_ARM_PORT 13028
#define RIGHT_FOREARM_PORT 12949
#define LEFT_SHOULDER_PORT 12941
#define LEFT_UPPER_ARM_PORT 13491
#define LEFT_FOREARM_PORT 13291


#else

// for using with physical bands

#define CHEST_BAND_PORT 12941
#define RIGHT_SHOULDER_PORT 12941
#define RIGHT_UPPER_ARM_PORT 12941
#define RIGHT_FOREARM_PORT 12941
#define LEFT_SHOULDER_PORT 12941
#define LEFT_UPPER_ARM_PORT 12941
#define LEFT_FOREARM_PORT 12941
#endif

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
#include "communications/bandmessage.h"

class WifiManager:public QObject
{
    Q_OBJECT
public:
    // consider having wifimanager take in the suit object here
    WifiManager();
    ~WifiManager();
    void initiateConnection(QList<BandType> bandsToConnect);
    void sendRawDataToBand(BandType destBand, QByteArray data);
    void sendMessageToBand(BandType destBand, QByteArray msgData, MessageType msgType);
    void sendMessageToBand(BandType destBand, BandMessage *fullMsg);
    void closeAllConnections();

signals:
    void dataAvailable(BandType recvdFrom, BandMessage *dataRecvd, QElapsedTimer timestamp);
    void connectionStatusChanged(BandType affectedBand, ConnectionStatus status);

private:
    QTcpSocket *newSocket;
    QTcpServer *serv;
    QHash<BandType, QTcpSocket*> socketMap;
    void routeToBandObject(BandType bandWithData);
    void startSingleConnection(BandType bandToConnect);
    QSignalMapper *connectedMapper;
    QSignalMapper *recvdMapper;
    QSignalMapper *disconnectedMapper;
    QHash<BandType, QString> ipMap;
    QHash<BandType, quint16> portMap;


private slots:
    void checkForData(int);
    void socketConnected(int);

    void socketDisconnected(int);
    void socketError(int);
};

#endif // WIFIMANAGER_H

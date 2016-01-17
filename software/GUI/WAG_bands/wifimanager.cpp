#include "wifimanager.h"
#include <QtNetwork>

WifiManager::WifiManager():QObject()
{
    serv = new QTcpServer(this);
    connect(serv, SIGNAL(newConnection()), this, SLOT(connectToNewDevice()));
}

void WifiManager::startListening()
{
    serv->listen(QHostAddress::Any, SERVER_PORT);
}



void WifiManager::connectToNewDevice() {
    QTcpSocket *socket;
    socket = serv->nextPendingConnection();
    if (socket != 0) {
        // associate socket with band given port number
        // check src port
        quint16 srcPort;
        srcPort = socket->peerPort();

        BandType connectingBandType;


        switch(srcPort) {
        case  CHEST_BAND_PORT:
            connectingBandType = CHEST;
            break;
        case  RIGHT_SHOULDER_PORT:
            connectingBandType = RIGHT_SHOULDER;
            break;
        case  RIGHT_UPPER_ARM_PORT:
            connectingBandType = RIGHT_UPPER_ARM;
            break;
        case  RIGHT_FOREARM_PORT:
            connectingBandType = RIGHT_LOWER_ARM;
            break;
        case  LEFT_SHOULDER_PORT:
            connectingBandType = LEFT_SHOULDER;
            break;
        case  LEFT_UPPER_ARM_PORT:
            connectingBandType = LEFT_UPPER_ARM;
            break;
        case  LEFT_FOREARM_PORT:
            connectingBandType = LEFT_LOWER_ARM;
            break;
        default:
            // something going wrong
            qDebug("ERRROR");
            qDebug()<<"Srcport is "<<srcPort;
            return;
        }
        qDebug()<<"Src port is "<<srcPort;
        qDebug()<<"BandType is "<<connectingBandType;
        socketMap[connectingBandType] = socket;

        // TODO connect readyRead to slot
    } else {
        // error!
    }
}

void WifiManager::sendToBand(BandType destBand, QByteArray bandData) {
    // write to socket
    QTcpSocket *bandSocket = socketMap[destBand];


    if (bandSocket->bytesAvailable() != 0) {
        // read data from bandSocket
        // TODO
    }

    if (bandSocket->write(bandData) < bandData.length()) {
        qDebug("ERROR SENDING");
    }
}

void WifiManager::sendToBand(BandType destBand, char * bandData) {
    // write to socket
    QTcpSocket *bandSocket = socketMap[destBand];


    if (bandSocket->bytesAvailable() != 0) {
        // read data from bandSocket
        // TODO
    }

    if (bandSocket->write(bandData, strlen(bandData)) < strlen(bandData)) {
        qDebug("ERROR SENDING");
    }
}

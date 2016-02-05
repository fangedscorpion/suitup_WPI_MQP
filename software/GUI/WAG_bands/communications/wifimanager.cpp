#include "wifimanager.h"
#include <QtNetwork>

WifiManager::WifiManager():QObject()
{
    connectedMapper = new QSignalMapper(this);
    disconnectedMapper = new QSignalMapper(this);
    recvdMapper = new QSignalMapper(this);

    QList<QHostAddress> addrlist = QNetworkInterface::allAddresses();
    qDebug()<<addrlist;

    QHostAddress myIp;

    for (int i=0;i < addrlist.length(); i++) {
        if (addrlist[i].protocol() == QAbstractSocket::IPv4Protocol) {
            if (addrlist[i] != QHostAddress("127.0.0.1")) {
                myIp = addrlist[i];
                break;
            }
        }
    }

    qDebug()<<myIp;
    // TODO check if no IP found
    QString ipString = myIp.toString();
    int lastPd = ipString.lastIndexOf(".");

    ipMap[CHEST] = ipString.replace(lastPd+1, 3, QString::number(CHEST_IP_END));
    ipMap[RIGHT_SHOULDER] = ipString.replace(lastPd+1, 3, QString::number(RIGHT_SHOULDER_IP_END));
    ipMap[LEFT_SHOULDER] = ipString.replace(lastPd+1, 3, QString::number(LEFT_SHOULDER_IP_END));
    ipMap[RIGHT_UPPER_ARM] = ipString.replace(lastPd+1, 3, QString::number(RIGHT_UPPER_ARM_IP_END));
    ipMap[LEFT_UPPER_ARM] = ipString.replace(lastPd+1, 3, QString::number(LEFT_UPPER_ARM_IP_END));
    ipMap[RIGHT_LOWER_ARM] = ipString.replace(lastPd+1, 3, QString::number(RIGHT_FOREARM_IP_END));
    ipMap[LEFT_LOWER_ARM] = ipString.replace(lastPd+1, 3, QString::number(LEFT_FOREARM_IP_END));

    ipMap[CHEST] = "127.0.0.1";// for testing purposes only, remove later
    ipMap[RIGHT_SHOULDER] = "127.0.0.1";
    ipMap[LEFT_SHOULDER] = "127.0.0.1";
    ipMap[RIGHT_UPPER_ARM] = "127.0.0.1";
    ipMap[LEFT_UPPER_ARM] = "127.0.0.1";
    ipMap[RIGHT_LOWER_ARM] = "127.0.0.1";
    ipMap[LEFT_LOWER_ARM] = "127.0.0.1";
    // TODO

    portMap[CHEST] = CHEST_BAND_PORT;
    portMap[LEFT_SHOULDER] = LEFT_SHOULDER_PORT;
    portMap[RIGHT_SHOULDER] = RIGHT_SHOULDER_PORT;
    portMap[LEFT_UPPER_ARM] = LEFT_UPPER_ARM_PORT;
    portMap[RIGHT_UPPER_ARM] = RIGHT_UPPER_ARM_PORT;
    portMap[LEFT_LOWER_ARM] = LEFT_FOREARM_PORT;
    portMap[RIGHT_LOWER_ARM] = RIGHT_FOREARM_PORT;

}

void WifiManager::initiateConnection(QList<BandType> bandsToConnect)
{

    // Don't know if we want to do this
    closeAllConnections();

    qDebug("Initiating conection");

    // need to check for duplicates (don't know why there would be)
    for (int i = 0; i < bandsToConnect.length(); i++) {
        qDebug()<<"connecting"<<bandsToConnect[i];
        qDebug()<<CHEST;
        startSingleConnection(bandsToConnect[i]);
    }


}

void WifiManager::startSingleConnection(BandType bandToConnect) {
    QString ipAddr = ipMap[bandToConnect];
    qDebug()<<"Connecting to "<<ipAddr;
    quint16 portNum = portMap[bandToConnect];
    qDebug()<<portNum;

    // TODO: make sure portnum and ip addr actually exist
    QTcpSocket *newSocket = new QTcpSocket();

    connectedMapper->setMapping(newSocket, bandToConnect);
    recvdMapper->setMapping(newSocket, bandToConnect);
    disconnectedMapper->setMapping(newSocket, bandToConnect);

    socketMap[bandToConnect] = newSocket;

    connect(newSocket, SIGNAL(connected()), connectedMapper, SLOT(map()));
    //connect(newSocket, SIGNAL(error()), errorMapper, SLOT(map()));
    connect(newSocket, SIGNAL(readyRead()), recvdMapper, SLOT(map()));
    connect(newSocket, SIGNAL(disconnected()), disconnectedMapper, SLOT(map()));

    connect(connectedMapper, SIGNAL(mapped(int)), this, SLOT(socketConnected(int)));
    connect(disconnectedMapper, SIGNAL(mapped(int)), this ,SLOT(socketDisconnected(int)));

    connect(recvdMapper, SIGNAL(mapped(int)), this, SLOT(checkForData(int)));

    newSocket->connectToHost(ipAddr, portNum);
}

void WifiManager::socketConnected(int connectedBand) {
    BandType bandEnum = (BandType) connectedBand;
    qDebug("Socket conencted");

    emit connectionStatusChanged(bandEnum, CONNECTED);

}

void WifiManager::socketError(int bandWithError) {

    BandType bandEnum = (BandType) bandWithError;
    qDebug("Socket error");
}

void WifiManager::socketDisconnected(int disconnectedBand) {
    BandType bandEnum = (BandType) disconnectedBand;
    emit connectionStatusChanged(bandEnum, DISCONNECTED);
}



/* void WifiManager::connectToNewDevice() {
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

        connect(socket, SIGNAL(readyRead()), this, SLOT(checkForData()));

        // TODO connect readyRead to slot
    } else {
        // error!
    }
} */

// should stay same after convert computer to client
void WifiManager::checkForData(int checkBand) {
    BandType bandWithData= (BandType) checkBand;
    if (socketMap[bandWithData]->bytesAvailable() >= 0) {

        routeToBandObject(bandWithData);
    } else {
        // error most likely
    }

}

// should stay same after convert to client
void WifiManager::sendRawDataToBand(BandType destBand, QByteArray bandData) {
    // write to socket
    if (socketMap.contains(destBand)) {
        qDebug("Sending");
        QTcpSocket *bandSocket = socketMap[destBand];

        qDebug("Socket retrieved");
        if (bandSocket->bytesAvailable() != 0) {
            // read data from bandSocket
            qDebug("Bytes available before send");
            routeToBandObject(destBand);
        }

        qDebug("writing data");

        if (bandSocket->write(bandData) < bandData.length()) {
            qDebug("ERROR SENDING");
        }
    } else {
        qDebug()<<"Band "<<destBand<<" is not connected ";
    }

}

void WifiManager::sendRawDataToBand(BandType destBand, char * bandData) {
    if (socketMap.contains(destBand)) {
        // write to socket

        qDebug()<<"Data "<<bandData;
        QTcpSocket *bandSocket = socketMap[destBand];


        if (bandSocket->bytesAvailable() != 0) {
            // if there is data to be read, route it ot the band object to be interpreted
            routeToBandObject(destBand);
        }

        if (bandSocket->write(bandData, strlen(bandData)) < strlen(bandData)) {
            qDebug("ERROR SENDING");
        }
    } else {
        qDebug()<<"Band "<<destBand<<" is not connected ";
    }
}

// do not call unless data is available
void WifiManager::routeToBandObject(BandType bandWithData) {
    QTcpSocket *bandSocket = socketMap[bandWithData];
    qDebug()<<"Bytes available: "<<bandSocket->bytesAvailable();

    // read all data from the socket
    QByteArray readData = bandSocket->readAll();

    if (readData.length() == 0) {
        // Error probably occured
    } else {
        qDebug()<<"READ " <<readData<<" from band number " <<bandWithData;

        // consider timestamping (get current time and pass with data)
        BandMessage *msg = new BandMessage(readData);

        emit dataAvailable(bandWithData, msg, QTime::currentTime());


        // actually send data to software band (need suit class here)
        //QByteArray returnData = trimNewLineAtEnd(readData);
        //qDebug()<<returnData;
        //returnData = reverseByteArray(returnData);
        //qDebug()<<returnData;
        //sendRawDataToBand(bandWithData, returnData);
    }
}

QByteArray WifiManager::trimNewLineAtEnd(QByteArray trimFrom) {
    bool keepTrimming = true;
    while (keepTrimming) {
        if (trimFrom[trimFrom.length() - 1] == '\n') {
            trimFrom.remove(trimFrom.length() - 1, 1);
        }
        // uncomment if we should also remove trailing spaces
        //else if (trimFrom[trimFrom.length() - 1] == ' '){
        //    trimFrom.remove(trimFrom.length() - 1, 1);
        //}
        else  {
            keepTrimming = false;
        }
    }
    qDebug()<<trimFrom;
    return trimFrom;
}

QByteArray WifiManager::reverseByteArray(QByteArray reverseThis) {
    qDebug()<<"Reversing "<<reverseThis;
    for (int i = 0; i < reverseThis.length()/2; i++) {
        char tmp = reverseThis[i];
        reverseThis[i] = reverseThis[reverseThis.length() - 1 -i];
        reverseThis[reverseThis.length() - 1 -i] = tmp;
    }
    qDebug()<<"Reversed: "<<reverseThis;
    return reverseThis;
}

void WifiManager::closeAllConnections() {
    QList<QTcpSocket *> openConnections = socketMap.values();

    for (int i = 0; i < openConnections.length(); i++) {
        openConnections[i]->disconnectFromHost();

    }
}



void WifiManager::sendMessageToBand(BandType destBand, QByteArray msgData, MessageType msgType) {
    BandMessage *newMsg = new BandMessage(msgType, msgData);
    sendMessageToBand(destBand, newMsg);
}

void WifiManager::sendMessageToBand(BandType destBand, BandMessage *fullMsg) {
    sendRawDataToBand(destBand, fullMsg->getSerializedMessage());
}


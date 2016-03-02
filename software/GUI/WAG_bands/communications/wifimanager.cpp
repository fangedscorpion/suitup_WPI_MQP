#include "wifimanager.h"
#include <QtNetwork>

WifiManager::WifiManager():QObject()
{
    connectedMapper = new QSignalMapper(this);
    disconnectedMapper = new QSignalMapper(this);
    recvdMapper = new QSignalMapper(this);

    QList<QHostAddress> addrlist = QNetworkInterface::allAddresses();
    qDebug()<<"WifiManager: address list"<<addrlist;

    QHostAddress myIp;

    for (int i=0;i < addrlist.length(); i++) {
        if (addrlist[i].protocol() == QAbstractSocket::IPv4Protocol) {
            if (addrlist[i] != QHostAddress("127.0.0.1")) {
                myIp = addrlist[i];
                break;
            }
        }
    }

    qDebug()<<"WifiManager: my IP "<<myIp;
    // TODO check if no IP found
    QString ipString = myIp.toString();
    int lastPd = ipString.lastIndexOf(".");

    if (!LocalTesting::inTestingMode()) {
        ipMap[CHEST] = ipString.replace(lastPd+1, 3, QString::number(CHEST_IP_END));
        ipMap[RIGHT_SHOULDER] = ipString.replace(lastPd+1, 3, QString::number(RIGHT_SHOULDER_IP_END));
        ipMap[LEFT_SHOULDER] = ipString.replace(lastPd+1, 3, QString::number(LEFT_SHOULDER_IP_END));
        ipMap[RIGHT_UPPER_ARM] = ipString.replace(lastPd+1, 3, QString::number(RIGHT_UPPER_ARM_IP_END));
        ipMap[LEFT_UPPER_ARM] = ipString.replace(lastPd+1, 3, QString::number(LEFT_UPPER_ARM_IP_END));
        ipMap[RIGHT_LOWER_ARM] = ipString.replace(lastPd+1, 3, QString::number(RIGHT_FOREARM_IP_END));
        ipMap[LEFT_LOWER_ARM] = ipString.replace(lastPd+1, 3, QString::number(LEFT_FOREARM_IP_END));

    } else {
        ipMap[CHEST] = "127.0.0.1";// for testing purposes only, remove later
        ipMap[RIGHT_SHOULDER] = "127.0.0.1";
        ipMap[LEFT_SHOULDER] = "127.0.0.1";
        ipMap[RIGHT_UPPER_ARM] = "127.0.0.1";
        ipMap[LEFT_UPPER_ARM] = "127.0.0.1";
        ipMap[RIGHT_LOWER_ARM] = "127.0.0.1";
        ipMap[LEFT_LOWER_ARM] = "127.0.0.1";
    }

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

    qDebug("WifiManager: Initiating conection");

    // need to check for duplicates (don't know why there would be)
    for (int i = 0; i < bandsToConnect.length(); i++) {
        qDebug()<<"WifiManager: connecting band "<<bandsToConnect[i];
        startSingleConnection(bandsToConnect[i]);
    }


}

void WifiManager::startSingleConnection(BandType bandToConnect) {
    QString ipAddr = ipMap[bandToConnect];
    qDebug()<<"WifiManager: Connecting to "<<ipAddr;
    quint16 portNum = portMap[bandToConnect];
    qDebug()<<"WifiManager: portNumber: "<<portNum;

    // TODO: make sure portnum and ip addr actually exist
    QTcpSocket *newSocket = new QTcpSocket();

    int convertedEnum = (int) bandToConnect;

    connectedMapper->setMapping(newSocket, convertedEnum);
    recvdMapper->setMapping(newSocket, convertedEnum);
    disconnectedMapper->setMapping(newSocket, convertedEnum);

    socketMap[bandToConnect] = newSocket;

    connect(newSocket, SIGNAL(connected()), connectedMapper, SLOT(map()));
    //connect(newSocket, SIGNAL(error()), errorMapper, SLOT(map()));
    connect(newSocket, SIGNAL(readyRead()), recvdMapper, SLOT(map()));
    connect(newSocket, SIGNAL(disconnected()), disconnectedMapper, SLOT(map()));

    connect(connectedMapper, SIGNAL(mapped(int)), this, SLOT(socketConnected(int)), Qt::UniqueConnection);
    connect(disconnectedMapper, SIGNAL(mapped(int)), this ,SLOT(socketDisconnected(int)), Qt::UniqueConnection);

    connect(recvdMapper, SIGNAL(mapped(int)), this, SLOT(checkForData(int)), Qt::UniqueConnection);
    newSocket->connectToHost(ipAddr, portNum);
}

void WifiManager::socketConnected(int connectedBand) {
    BandType bandEnum = (BandType) connectedBand;
    qDebug()<<"WifiManager: Socket conencted: "<<bandEnum;

    emit connectionStatusChanged(bandEnum, CONNECTED);


}

void WifiManager::socketError(int bandWithError) {

    BandType bandEnum = (BandType) bandWithError;
    qDebug("WifiManager: Socket error");
}

void WifiManager::socketDisconnected(int disconnectedBand) {
    BandType bandEnum = (BandType) disconnectedBand;
    emit connectionStatusChanged(bandEnum, DISCONNECTED);
}

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
        QTcpSocket *bandSocket = socketMap[destBand];

        if (bandSocket->bytesAvailable() != 0) {
            // read data from bandSocket
            routeToBandObject(destBand);
        }


        if (bandSocket->write(bandData) < bandData.length()) {
            qDebug("WifiManager: ERROR SENDING");
        }
    } else {
        qDebug()<<"WifiManager: Band "<<destBand<<" is not connected ";
    }

}

void WifiManager::sendRawDataToBand(BandType destBand, char * bandData) {
    if (socketMap.contains(destBand)) {
        // write to socket

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

    // read all data from the socket
    QByteArray readData = bandSocket->readAll();

    if (readData.length() == 0) {
        // Error probably occured
    } else {
        qDebug()<<"WifiManager: READ " <<readData<<" from band number " <<bandWithData;
        bool keepParsing = true;
        int iterations = 1;
        while (keepParsing) {
            qDebug()<<"WifiManager:: message parsing iterations "<<iterations;
            iterations++;

            BandMessage *msg;
            QElapsedTimer msgTimer;
            try {

                msg = new BandMessage();
                msgTimer = QElapsedTimer();
                msgTimer.start();
                msg->parseFromByteArray(readData);
                qDebug("Setting keep parsing to false");
                keepParsing = false;
            } catch(IncorrectDataLengthException *e) {
                qDebug("Exception thrown");
                readData = msg->handleException(e);
                // would need new line, length, and message type to be real packet
                qDebug()<<readData;
                if (readData.length() < 3) {
                    keepParsing = false;
                    qDebug("Setting keep parsing to false in exception");
                }
            }

            emit dataAvailable(bandWithData, msg, msgTimer);
        }
    }
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

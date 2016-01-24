#include "wifimanager.h"
#include <QtNetwork>

WifiManager::WifiManager():QObject()
{
    serv = new QTcpServer(this); // TODO: convert to multi-client system
    connect(serv, SIGNAL(newConnection()), this, SLOT(connectToNewDevice()));

    connectedMapper = new QSignalMapper(this);

    errorMapper = new QSignalMapper(this);

    recvdMapper = new QSignalMapper(this);

    // get my IP
    // generate band IPs based off this
    ipMap[CHEST] = "127.0.0.1";
    portMap[CHEST] = CHEST_BAND_PORT;
}

void WifiManager::initiateConnection()
{
    // TODO: initiate connection with bands (bands are server)
    // get list of enabled bands
    // make list of IPs based on enabled bands
    // generate sockets for connecting
    // add to socket map
    //serv->listen(QHostAddress::Any, SERVER_PORT);

    // get list of objects that should be socketified
    // for loop here TODO
    qDebug("Initiating conection");
    startSingleConnection(CHEST);


}

void WifiManager::startSingleConnection(BandType bandToConnect) {
    QString ipAddr = ipMap[bandToConnect];
    quint16 portNum = portMap[bandToConnect];

    // TODO: make sure portnum and ip addr actually exist
    QTcpSocket *newSocket = new QTcpSocket();

    connectedMapper->setMapping(newSocket, bandToConnect);
    errorMapper->setMapping(newSocket, bandToConnect);
    recvdMapper->setMapping(newSocket, bandToConnect);

    socketMap[bandToConnect] = newSocket;

    connect(newSocket, SIGNAL(connected()), connectedMapper, SLOT(map()));
    //connect(newSocket, SIGNAL(error()), errorMapper, SLOT(map()));
    connect(newSocket, SIGNAL(readyRead()), recvdMapper, SLOT(map()));

    connect(connectedMapper, SIGNAL(mapped(int)), this, SLOT(socketConnected(int)));

    connect(errorMapper, SIGNAL(mapped(int)), this, SLOT(socketError(int)));

    connect(recvdMapper, SIGNAL(mapped(int)), this, SLOT(checkForData(int)));

    newSocket->connectToHost(ipAddr, portNum);
}

void WifiManager::socketConnected(int connectedBand) {
    BandType bandEnum = (BandType) connectedBand;
    qDebug("Socket conencted");

}

void WifiManager::socketError(int bandWithError) {

    BandType bandEnum = (BandType) bandWithError;
    qDebug("Socket error");
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

        connect(socket, SIGNAL(readyRead()), this, SLOT(checkForData()));

        // TODO connect readyRead to slot
    } else {
        // error!
    }
}

// should stay same after convert computer to client
void WifiManager::checkForData(int checkBand) {
    BandType bandWithData= (BandType) checkBand;
    /* qDebug("checking for data");
    QList<BandType> possibleBands;

    // have to figure out way to make sure all bands required bands connect
    possibleBands = socketMap.keys();

    QList<BandType> bandsWithData;
    qDebug("HERE7890");

    // check which bands have data
    for (int i = 0; i < possibleBands.size(); i++) {
        qDebug("In loop");
        qDebug()<<"Checking band at "<<i;
        qDebug()<<"Band number is "<<possibleBands[i];
        if (socketMap[possibleBands[i]]->bytesAvailable() != 0) {
            qDebug("FoundData");
            bandsWithData<<possibleBands[i];
        }
        qDebug()<<"HERE"<<i;
    }

    qDebug("HERE"); */


    routeToBandObject(bandWithData);


    qDebug("HERE6789");
}

// should stay same after convert to client
void WifiManager::sendToBand(BandType destBand, QByteArray bandData) {
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

void WifiManager::sendToBand(BandType destBand, char * bandData) {
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

        emit dataAvailable(bandWithData, readData, QTime::currentTime());


        // actually send data to software band (need suit class here)
        QByteArray returnData = trimNewLineAtEnd(readData);
        qDebug()<<returnData;
        returnData = reverseByteArray(returnData);
        qDebug()<<returnData;
        sendToBand(bandWithData, returnData);
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

#include "wifimanager.h"
#include <QtNetwork>

WifiManager::WifiManager():QObject()
{
    serv = new QTcpServer(this);
    connect(serv, SIGNAL(newConnection()), this, SLOT(connectToNewDevice()));
}

void WifiManager::startListening()
{
    serv->listen();
}



void WifiManager::connectToNewDevice() {
    QTcpSocket *socket;
    socket = serv->nextPendingConnection();
    if (socket != 0) {
    // associate socket with band given port number
    } else {
        // error!
    }
}

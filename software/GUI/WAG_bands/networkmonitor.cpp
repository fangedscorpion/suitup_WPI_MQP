#include <sys/socket.h>
#include "networkmonitor.h"


NetworkMonitor::NetworkMonitor(int portNumber)
{




}

void NetworkMonitor::sendMotorCommand() {

}

NetworkMonitor::sendData(char * serializedData, int dataLen) {

}

// returns number of bytes of data sent
int NetworkMonitor::recieveData(char *dataBuffer) {
    recv();

    emit receievedQuaternion(QQuaternion(0, 1, 2, 3));
}

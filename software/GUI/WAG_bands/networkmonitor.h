#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <QQuaternion>
#include <QString>

class NetworkMonitor
{
public:
    NetworkMonitor(int portNumber);
    ~NetworkMonitor();

signals:
    void receievedQuaternion(QQuaternion quat);
    void acceptedConnection(QString ipAddress);

public slots:
    void sendMotorCommand();

private:
    int clntSocket;
    int recieveData(char *dataBuffer);
    void sendData(char * serializedData, int dataLen);
};

#endif // NETWORKMONITOR_H

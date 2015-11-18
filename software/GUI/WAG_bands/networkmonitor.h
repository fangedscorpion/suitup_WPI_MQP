#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <QQuaternion>

class NetworkMonitor
{
public:
    NetworkMonitor(int portNumber);

signals:
    void receievedQuaternion(QQuaternion quat);

public slots:
    void sendMotorCommand();

private:
    int socket;
};

#endif // NETWORKMONITOR_H

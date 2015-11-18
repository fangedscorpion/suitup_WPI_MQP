#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include <QQuaternion>
#include <QString>
#include <QObject>

class NetworkMonitor: public QObject {
    Q_OBJECT

public:
    NetworkMonitor(QObject *parent=0, int portNumber=-1);
    ~NetworkMonitor();

public slots:
    void sendMotorCommand();

signals:
    void receievedQuaternion(QQuaternion quat);
    void acceptedConnection(QString ipAddress);

private:
    int clntSocket;
    int recieveData(char *dataBuffer);
    void sendData(char * serializedData, int dataLen);
};

#endif // NETWORKMONITOR_H

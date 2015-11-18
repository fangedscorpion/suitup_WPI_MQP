#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QBluetoothDeviceInfo>
#include <QObject>

class BluetoothManager:public QObject
{
    Q_OBJECT
public:
    BluetoothManager();
    void startDiscovery();

public slots:
    void discoveredDevice(QBluetoothDeviceInfo info);
};

#endif // BLUETOOTHMANAGER_H

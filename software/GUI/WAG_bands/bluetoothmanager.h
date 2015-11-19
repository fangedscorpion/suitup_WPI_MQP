#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QBluetoothDeviceInfo>
#include <QObject>
#include <QLowEnergyController>
#include <vector>
#include <QBluetoothLocalDevice>

class BluetoothManager:public QObject
{
    Q_OBJECT
public:
    BluetoothManager();
    void startDiscovery();
    void tryToConnectToDevice(QBluetoothDeviceInfo connectToDev);
    void connectToSelectedDevice();
    void pairWithDevice(QBluetoothAddress address);
    void tryToPairWithSelectedDevice();

public slots:
    void discoveredDevice(QBluetoothDeviceInfo info);
    void connectedToBLEDev();
    void connectionErrorBLE(QLowEnergyController::Error errorRecvd);
    void disconnectedFromBLEDev();
    void devConnectedPairing(QBluetoothAddress address);
    void devDisconnectedPairing(QBluetoothAddress address);
    void pairingError(QBluetoothLocalDevice::Error err);
    void localDevStateChanged(QBluetoothLocalDevice::HostMode hostMode);
    void pairingDisplayConf(QBluetoothAddress address, QString pinCode);
    void pairingDisplayPin(QBluetoothAddress address, QString pinCode);
    void finishedPairing(QBluetoothAddress address, QBluetoothLocalDevice::Pairing pairingStatus);

private:
    //std::vector<QBluetoothDeviceInfo> selectedDevices;
    QBluetoothDeviceInfo selectedDev;
    QBluetoothLocalDevice *localDev;
};

#endif // BLUETOOTHMANAGER_H

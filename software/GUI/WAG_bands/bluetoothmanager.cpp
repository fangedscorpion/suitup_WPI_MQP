#include "bluetoothmanager.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QDebug>

BluetoothManager::BluetoothManager():QObject() {


}

void BluetoothManager::startDiscovery() {
    QBluetoothDeviceDiscoveryAgent *agent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(agent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(discoveredDevice(QBluetoothDeviceInfo)));

    agent->start();
}

void BluetoothManager::discoveredDevice(QBluetoothDeviceInfo info) {

    qDebug()<<info.name()<<" "<<info.address()<<" "<<info.coreConfigurations();
    if (info.coreConfigurations() == 0x02) {
        qDebug()<<"Standard bluetooth device";
    }
    else if (info.coreConfigurations() == 0x01) {
        qDebug()<<"BLE device";
    }
    else if (info.coreConfigurations() == 0x02) {
        qDebug("LE and standard enabled");
    }
    else if (info.coreConfigurations() == 0) {
        qDebug("Unknown configuration");
    }
    else {
        qDebug("I'm not sure how to check the core configuration");
    }
}





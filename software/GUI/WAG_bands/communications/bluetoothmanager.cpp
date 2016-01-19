#include "bluetoothmanager.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QDebug>
#include <vector>
#include <QBluetoothLocalDevice>
#include <QString>
#include <QBluetoothAddress>

BluetoothManager::BluetoothManager():QObject() {
    //std::vector<QBluetoothDeviceInfo> selectedDevices;
    localDev = new QBluetoothLocalDevice(this);

}

void BluetoothManager::startDiscovery() {
    QBluetoothDeviceDiscoveryAgent *agent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(agent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(discoveredDevice(QBluetoothDeviceInfo)));

    agent->start();
}

void BluetoothManager::discoveredDevice(QBluetoothDeviceInfo info) {
    // Adrress CF:37:E9:c8:32:29 << Bluefruit
    // Address 2: F2:14:C8:11:BB:5D
    QBluetoothAddress desiredAddress = QBluetoothAddress("F2:14:C8:11:BB:5D"); //CF:37:E9:C8:32:29");
    if (info.address() == desiredAddress) {
        qDebug("Found bluetooth chip");
        qDebug()<<info.name()<<" "<<info.address()<<" "<<info.coreConfigurations();
        //if (selectedDevices.size() <= 0) {
        // selectedDevices.push_back(info);
        //}
        selectedDev = info;

    }
    if (info.name() == "Mandi's iPhone") {
        qDebug("Found mandi's iPhone");
        selectedDev = info;
    }
    //qDebug()<<info.name()<<" "<<info.address()<<" "<<info.coreConfigurations();
    if (info.coreConfigurations() == QBluetoothDeviceInfo::BaseRateCoreConfiguration) {
        //    qDebug()<<"Standard bluetooth device";
    }
    else if (info.coreConfigurations() == QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        //    qDebug()<<"BLE device";
    }
    else if (info.coreConfigurations() == QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration) {
        //   qDebug("LE and standard enabled");
    }
    else if (info.coreConfigurations() == QBluetoothDeviceInfo::UnknownCoreConfiguration) {
        // qDebug("Unknown configuration");
    }
    else {
        //qDebug("I'm not sure how to check the core configuration");
    }
}

void BluetoothManager::tryToConnectToDevice(QBluetoothDeviceInfo connectToDev) {
    if ((connectToDev.coreConfigurations() == QBluetoothDeviceInfo::LowEnergyCoreConfiguration) || (connectToDev.coreConfigurations() == QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration)){
        qDebug("Try to connect");
        QLowEnergyController *controller = new QLowEnergyController(connectToDev);
        connect(controller, SIGNAL(connected()),
                this, SLOT(connectedToBLEDev()));
        connect(controller, SIGNAL(error(QLowEnergyController::Error)),
                this, SLOT(connectionErrorBLE(QLowEnergyController::Error)));
        connect(controller, SIGNAL(disconnected()),
                this, SLOT(disconnectedFromBLEDev()));
        controller->connectToDevice();
    }
    else {
        // F214c811bb5d
    }
}

void BluetoothManager::connectedToBLEDev() {
    qDebug("Connected to device");
}

void BluetoothManager::connectionErrorBLE(QLowEnergyController::Error errorRecvd) {
    qDebug()<<errorRecvd;
    qDebug("ERROR");
}

void BluetoothManager::disconnectedFromBLEDev() {
    qDebug("Disconnected");
}

void BluetoothManager::connectToSelectedDevice() {
    tryToConnectToDevice(selectedDev);
}

void BluetoothManager::tryToPairWithSelectedDevice() {
    pairWithDevice(selectedDev.address());
}


void BluetoothManager::pairWithDevice(QBluetoothAddress address) {
    connect(localDev, SIGNAL(deviceConnected(QBluetoothAddress)),this, SLOT(devConnectedPairing(QBluetoothAddress)));
    connect(localDev, SIGNAL(deviceDisconnected(QBluetoothAddress)), this, SLOT(devDisconnectedPairing(QBluetoothAddress)));
    connect(localDev, SIGNAL(error(QBluetoothLocalDevice::Error)), this, SLOT(pairingError(QBluetoothLocalDevice::Error)));
    connect(localDev, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)), this, SLOT(localDevStateChanged(QBluetoothLocalDevice::HostMode)));
    connect(localDev, SIGNAL(pairingDisplayConfirmation(QBluetoothAddress, QString)), this, SLOT(pairingDisplayConf(QBluetoothAddress, QString)));
    connect(localDev, SIGNAL(pairingDisplayPinCode(QBluetoothAddress, QString)), this, SLOT(pairingDisplayPin(QBluetoothAddress, QString)));
    connect(localDev, SIGNAL(pairingFinished(QBluetoothAddress, QBluetoothLocalDevice::Pairing)), this, SLOT(finishedPairing(QBluetoothAddress, QBluetoothLocalDevice::Pairing)));

    localDev->requestPairing(address, QBluetoothLocalDevice::AuthorizedPaired);
}

void BluetoothManager::devConnectedPairing(QBluetoothAddress address) {
    qDebug()<<"Pairing: Connected to "<<address;
}

void BluetoothManager::devDisconnectedPairing(QBluetoothAddress address) {
    qDebug()<<"Pairing: Connected to "<<address;
}

void BluetoothManager::pairingError(QBluetoothLocalDevice::Error err) {
    qDebug()<<"Pairing: error "<<err;
}

void BluetoothManager::localDevStateChanged(QBluetoothLocalDevice::HostMode hostMode) {
    qDebug()<<"Local device state changed to  "<<hostMode;
}

void BluetoothManager::pairingDisplayConf(QBluetoothAddress address, QString pinCode) {
    qDebug()<<"Pairing confirmation: "<<address<<"Pin code "<<pinCode;
}


void BluetoothManager::pairingDisplayPin(QBluetoothAddress address, QString pinCode) {
    qDebug()<<"Pairing display: "<<address<<"Pin code "<<pinCode;
}

void BluetoothManager::finishedPairing(QBluetoothAddress address, QBluetoothLocalDevice::Pairing pairingStatus) {
    qDebug()<<"Paired with "<<address<<" pairing state"<<pairingStatus;
}

/****************************************************************************
** Meta object code from reading C++ file 'bluetoothmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../WAG_bands/bluetoothmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bluetoothmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BluetoothManager_t {
    QByteArrayData data[26];
    char stringdata0[443];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BluetoothManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BluetoothManager_t qt_meta_stringdata_BluetoothManager = {
    {
QT_MOC_LITERAL(0, 0, 16), // "BluetoothManager"
QT_MOC_LITERAL(1, 17, 16), // "discoveredDevice"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 20), // "QBluetoothDeviceInfo"
QT_MOC_LITERAL(4, 56, 4), // "info"
QT_MOC_LITERAL(5, 61, 17), // "connectedToBLEDev"
QT_MOC_LITERAL(6, 79, 18), // "connectionErrorBLE"
QT_MOC_LITERAL(7, 98, 27), // "QLowEnergyController::Error"
QT_MOC_LITERAL(8, 126, 10), // "errorRecvd"
QT_MOC_LITERAL(9, 137, 22), // "disconnectedFromBLEDev"
QT_MOC_LITERAL(10, 160, 19), // "devConnectedPairing"
QT_MOC_LITERAL(11, 180, 17), // "QBluetoothAddress"
QT_MOC_LITERAL(12, 198, 7), // "address"
QT_MOC_LITERAL(13, 206, 22), // "devDisconnectedPairing"
QT_MOC_LITERAL(14, 229, 12), // "pairingError"
QT_MOC_LITERAL(15, 242, 28), // "QBluetoothLocalDevice::Error"
QT_MOC_LITERAL(16, 271, 3), // "err"
QT_MOC_LITERAL(17, 275, 20), // "localDevStateChanged"
QT_MOC_LITERAL(18, 296, 31), // "QBluetoothLocalDevice::HostMode"
QT_MOC_LITERAL(19, 328, 8), // "hostMode"
QT_MOC_LITERAL(20, 337, 18), // "pairingDisplayConf"
QT_MOC_LITERAL(21, 356, 7), // "pinCode"
QT_MOC_LITERAL(22, 364, 17), // "pairingDisplayPin"
QT_MOC_LITERAL(23, 382, 15), // "finishedPairing"
QT_MOC_LITERAL(24, 398, 30), // "QBluetoothLocalDevice::Pairing"
QT_MOC_LITERAL(25, 429, 13) // "pairingStatus"

    },
    "BluetoothManager\0discoveredDevice\0\0"
    "QBluetoothDeviceInfo\0info\0connectedToBLEDev\0"
    "connectionErrorBLE\0QLowEnergyController::Error\0"
    "errorRecvd\0disconnectedFromBLEDev\0"
    "devConnectedPairing\0QBluetoothAddress\0"
    "address\0devDisconnectedPairing\0"
    "pairingError\0QBluetoothLocalDevice::Error\0"
    "err\0localDevStateChanged\0"
    "QBluetoothLocalDevice::HostMode\0"
    "hostMode\0pairingDisplayConf\0pinCode\0"
    "pairingDisplayPin\0finishedPairing\0"
    "QBluetoothLocalDevice::Pairing\0"
    "pairingStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BluetoothManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x0a /* Public */,
       5,    0,   72,    2, 0x0a /* Public */,
       6,    1,   73,    2, 0x0a /* Public */,
       9,    0,   76,    2, 0x0a /* Public */,
      10,    1,   77,    2, 0x0a /* Public */,
      13,    1,   80,    2, 0x0a /* Public */,
      14,    1,   83,    2, 0x0a /* Public */,
      17,    1,   86,    2, 0x0a /* Public */,
      20,    2,   89,    2, 0x0a /* Public */,
      22,    2,   94,    2, 0x0a /* Public */,
      23,    2,   99,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QString,   12,   21,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QString,   12,   21,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 24,   12,   25,

       0        // eod
};

void BluetoothManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BluetoothManager *_t = static_cast<BluetoothManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->discoveredDevice((*reinterpret_cast< QBluetoothDeviceInfo(*)>(_a[1]))); break;
        case 1: _t->connectedToBLEDev(); break;
        case 2: _t->connectionErrorBLE((*reinterpret_cast< QLowEnergyController::Error(*)>(_a[1]))); break;
        case 3: _t->disconnectedFromBLEDev(); break;
        case 4: _t->devConnectedPairing((*reinterpret_cast< QBluetoothAddress(*)>(_a[1]))); break;
        case 5: _t->devDisconnectedPairing((*reinterpret_cast< QBluetoothAddress(*)>(_a[1]))); break;
        case 6: _t->pairingError((*reinterpret_cast< QBluetoothLocalDevice::Error(*)>(_a[1]))); break;
        case 7: _t->localDevStateChanged((*reinterpret_cast< QBluetoothLocalDevice::HostMode(*)>(_a[1]))); break;
        case 8: _t->pairingDisplayConf((*reinterpret_cast< QBluetoothAddress(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 9: _t->pairingDisplayPin((*reinterpret_cast< QBluetoothAddress(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: _t->finishedPairing((*reinterpret_cast< QBluetoothAddress(*)>(_a[1])),(*reinterpret_cast< QBluetoothLocalDevice::Pairing(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothDeviceInfo >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothAddress >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothAddress >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothAddress >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothAddress >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBluetoothAddress >(); break;
            }
            break;
        }
    }
}

const QMetaObject BluetoothManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BluetoothManager.data,
      qt_meta_data_BluetoothManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BluetoothManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BluetoothManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BluetoothManager.stringdata0))
        return static_cast<void*>(const_cast< BluetoothManager*>(this));
    return QObject::qt_metacast(_clname);
}

int BluetoothManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'networkmonitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../WAG_bands/networkmonitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkmonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NetworkMonitor_t {
    QByteArrayData data[7];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkMonitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkMonitor_t qt_meta_stringdata_NetworkMonitor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "NetworkMonitor"
QT_MOC_LITERAL(1, 15, 19), // "receievedQuaternion"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 4), // "quat"
QT_MOC_LITERAL(4, 41, 18), // "acceptedConnection"
QT_MOC_LITERAL(5, 60, 9), // "ipAddress"
QT_MOC_LITERAL(6, 70, 16) // "sendMotorCommand"

    },
    "NetworkMonitor\0receievedQuaternion\0\0"
    "quat\0acceptedConnection\0ipAddress\0"
    "sendMotorCommand"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkMonitor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QQuaternion,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void NetworkMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetworkMonitor *_t = static_cast<NetworkMonitor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->receievedQuaternion((*reinterpret_cast< QQuaternion(*)>(_a[1]))); break;
        case 1: _t->acceptedConnection((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sendMotorCommand(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NetworkMonitor::*_t)(QQuaternion );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkMonitor::receievedQuaternion)) {
                *result = 0;
            }
        }
        {
            typedef void (NetworkMonitor::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NetworkMonitor::acceptedConnection)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject NetworkMonitor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NetworkMonitor.data,
      qt_meta_data_NetworkMonitor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NetworkMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkMonitor.stringdata0))
        return static_cast<void*>(const_cast< NetworkMonitor*>(this));
    return QObject::qt_metacast(_clname);
}

int NetworkMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NetworkMonitor::receievedQuaternion(QQuaternion _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkMonitor::acceptedConnection(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

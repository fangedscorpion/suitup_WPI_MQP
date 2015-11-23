/****************************************************************************
** Meta object code from reading C++ file 'playbackcontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../WAG_bands/playbackcontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playbackcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PlaybackController_t {
    QByteArrayData data[16];
    char stringdata0[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlaybackController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlaybackController_t qt_meta_stringdata_PlaybackController = {
    {
QT_MOC_LITERAL(0, 0, 18), // "PlaybackController"
QT_MOC_LITERAL(1, 19, 12), // "frameChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "newFrame"
QT_MOC_LITERAL(4, 42, 10), // "togglePlay"
QT_MOC_LITERAL(5, 53, 18), // "setStepThroughMode"
QT_MOC_LITERAL(6, 72, 17), // "desiredModeString"
QT_MOC_LITERAL(7, 90, 18), // "toggleVoiceControl"
QT_MOC_LITERAL(8, 109, 16), // "toggleSuitActive"
QT_MOC_LITERAL(9, 126, 6), // "active"
QT_MOC_LITERAL(10, 133, 16), // "moveFramePointer"
QT_MOC_LITERAL(11, 150, 14), // "modifyHoldTime"
QT_MOC_LITERAL(12, 165, 11), // "holdSeconds"
QT_MOC_LITERAL(13, 177, 26), // "updateStepThroughTolerance"
QT_MOC_LITERAL(14, 204, 11), // "sliderValue"
QT_MOC_LITERAL(15, 216, 12) // "speedChanged"

    },
    "PlaybackController\0frameChanged\0\0"
    "newFrame\0togglePlay\0setStepThroughMode\0"
    "desiredModeString\0toggleVoiceControl\0"
    "toggleSuitActive\0active\0moveFramePointer\0"
    "modifyHoldTime\0holdSeconds\0"
    "updateStepThroughTolerance\0sliderValue\0"
    "speedChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlaybackController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   62,    2, 0x0a /* Public */,
       5,    1,   63,    2, 0x0a /* Public */,
       7,    0,   66,    2, 0x0a /* Public */,
       8,    1,   67,    2, 0x0a /* Public */,
      10,    1,   70,    2, 0x0a /* Public */,
      11,    1,   73,    2, 0x0a /* Public */,
      13,    1,   76,    2, 0x0a /* Public */,
      15,    1,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Double,   12,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

void PlaybackController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlaybackController *_t = static_cast<PlaybackController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->togglePlay(); break;
        case 2: _t->setStepThroughMode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->toggleVoiceControl(); break;
        case 4: _t->toggleSuitActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->moveFramePointer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->modifyHoldTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->updateStepThroughTolerance((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->speedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PlaybackController::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlaybackController::frameChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject PlaybackController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PlaybackController.data,
      qt_meta_data_PlaybackController,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PlaybackController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlaybackController::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PlaybackController.stringdata0))
        return static_cast<void*>(const_cast< PlaybackController*>(this));
    return QObject::qt_metacast(_clname);
}

int PlaybackController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void PlaybackController::frameChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

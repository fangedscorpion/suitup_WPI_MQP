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
    QByteArrayData data[20];
    char stringdata0[283];
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
QT_MOC_LITERAL(4, 42, 13), // "startPlayback"
QT_MOC_LITERAL(5, 56, 12), // "stopPlayback"
QT_MOC_LITERAL(6, 69, 14), // "endOfTimeRange"
QT_MOC_LITERAL(7, 84, 10), // "togglePlay"
QT_MOC_LITERAL(8, 95, 18), // "setStepThroughMode"
QT_MOC_LITERAL(9, 114, 17), // "desiredModeString"
QT_MOC_LITERAL(10, 132, 18), // "toggleVoiceControl"
QT_MOC_LITERAL(11, 151, 16), // "toggleSuitActive"
QT_MOC_LITERAL(12, 168, 6), // "active"
QT_MOC_LITERAL(13, 175, 16), // "moveFramePointer"
QT_MOC_LITERAL(14, 192, 14), // "modifyHoldTime"
QT_MOC_LITERAL(15, 207, 11), // "holdSeconds"
QT_MOC_LITERAL(16, 219, 26), // "updateStepThroughTolerance"
QT_MOC_LITERAL(17, 246, 11), // "sliderValue"
QT_MOC_LITERAL(18, 258, 12), // "speedChanged"
QT_MOC_LITERAL(19, 271, 11) // "positionMet"

    },
    "PlaybackController\0frameChanged\0\0"
    "newFrame\0startPlayback\0stopPlayback\0"
    "endOfTimeRange\0togglePlay\0setStepThroughMode\0"
    "desiredModeString\0toggleVoiceControl\0"
    "toggleSuitActive\0active\0moveFramePointer\0"
    "modifyHoldTime\0holdSeconds\0"
    "updateStepThroughTolerance\0sliderValue\0"
    "speedChanged\0positionMet"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlaybackController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    0,   82,    2, 0x06 /* Public */,
       5,    0,   83,    2, 0x06 /* Public */,
       6,    0,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   85,    2, 0x0a /* Public */,
       8,    1,   86,    2, 0x0a /* Public */,
      10,    0,   89,    2, 0x0a /* Public */,
      11,    1,   90,    2, 0x0a /* Public */,
      13,    1,   93,    2, 0x0a /* Public */,
      14,    1,   96,    2, 0x0a /* Public */,
      16,    1,   99,    2, 0x0a /* Public */,
      18,    1,  102,    2, 0x0a /* Public */,
      19,    0,  105,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Double,   15,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,

       0        // eod
};

void PlaybackController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlaybackController *_t = static_cast<PlaybackController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->startPlayback(); break;
        case 2: _t->stopPlayback(); break;
        case 3: _t->endOfTimeRange(); break;
        case 4: _t->togglePlay(); break;
        case 5: _t->setStepThroughMode((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->toggleVoiceControl(); break;
        case 7: _t->toggleSuitActive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->moveFramePointer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->modifyHoldTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->updateStepThroughTolerance((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->speedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->positionMet(); break;
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
        {
            typedef void (PlaybackController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlaybackController::startPlayback)) {
                *result = 1;
            }
        }
        {
            typedef void (PlaybackController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlaybackController::stopPlayback)) {
                *result = 2;
            }
        }
        {
            typedef void (PlaybackController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PlaybackController::endOfTimeRange)) {
                *result = 3;
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
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void PlaybackController::frameChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PlaybackController::startPlayback()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void PlaybackController::stopPlayback()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void PlaybackController::endOfTimeRange()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'spotcontrol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../spotcontrol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spotcontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SpotControl_t {
    QByteArrayData data[30];
    char stringdata0[255];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpotControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpotControl_t qt_meta_stringdata_SpotControl = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SpotControl"
QT_MOC_LITERAL(1, 12, 9), // "moveToPos"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 1), // "x"
QT_MOC_LITERAL(4, 25, 1), // "y"
QT_MOC_LITERAL(5, 27, 1), // "z"
QT_MOC_LITERAL(6, 29, 10), // "clearError"
QT_MOC_LITERAL(7, 40, 8), // "complete"
QT_MOC_LITERAL(8, 49, 8), // "progress"
QT_MOC_LITERAL(9, 58, 7), // "percent"
QT_MOC_LITERAL(10, 66, 10), // "startTimer"
QT_MOC_LITERAL(11, 77, 3), // "sec"
QT_MOC_LITERAL(12, 81, 13), // "startSpotting"
QT_MOC_LITERAL(13, 95, 10), // "doneMoving"
QT_MOC_LITERAL(14, 106, 18), // "rxStartCoordinates"
QT_MOC_LITERAL(15, 125, 2), // "x1"
QT_MOC_LITERAL(16, 128, 2), // "y1"
QT_MOC_LITERAL(17, 131, 2), // "x2"
QT_MOC_LITERAL(18, 134, 2), // "y2"
QT_MOC_LITERAL(19, 137, 15), // "rxScriptLoopCnt"
QT_MOC_LITERAL(20, 153, 7), // "loopCnt"
QT_MOC_LITERAL(21, 161, 16), // "rxSleepTimerDone"
QT_MOC_LITERAL(22, 178, 15), // "rxSensorVersion"
QT_MOC_LITERAL(23, 194, 7), // "version"
QT_MOC_LITERAL(24, 202, 12), // "rxRobotError"
QT_MOC_LITERAL(25, 215, 11), // "moveToCoord"
QT_MOC_LITERAL(26, 227, 9), // "datum1_x1"
QT_MOC_LITERAL(27, 237, 9), // "datum1_y1"
QT_MOC_LITERAL(28, 247, 3), // "col"
QT_MOC_LITERAL(29, 251, 3) // "row"

    },
    "SpotControl\0moveToPos\0\0x\0y\0z\0clearError\0"
    "complete\0progress\0percent\0startTimer\0"
    "sec\0startSpotting\0doneMoving\0"
    "rxStartCoordinates\0x1\0y1\0x2\0y2\0"
    "rxScriptLoopCnt\0loopCnt\0rxSleepTimerDone\0"
    "rxSensorVersion\0version\0rxRobotError\0"
    "moveToCoord\0datum1_x1\0datum1_y1\0col\0"
    "row"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpotControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   79,    2, 0x06 /* Public */,
       6,    0,   86,    2, 0x06 /* Public */,
       7,    0,   87,    2, 0x06 /* Public */,
       8,    1,   88,    2, 0x06 /* Public */,
      10,    1,   91,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   94,    2, 0x0a /* Public */,
      13,    0,   95,    2, 0x0a /* Public */,
      14,    5,   96,    2, 0x0a /* Public */,
      19,    1,  107,    2, 0x0a /* Public */,
      21,    0,  110,    2, 0x0a /* Public */,
      22,    1,  111,    2, 0x0a /* Public */,
      24,    0,  114,    2, 0x0a /* Public */,
      25,    4,  115,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   15,   16,   17,   18,    5,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   26,   27,   28,   29,

       0        // eod
};

void SpotControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SpotControl *_t = static_cast<SpotControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->moveToPos((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->clearError(); break;
        case 2: _t->complete(); break;
        case 3: _t->progress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->startTimer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->startSpotting(); break;
        case 6: _t->doneMoving(); break;
        case 7: _t->rxStartCoordinates((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 8: _t->rxScriptLoopCnt((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->rxSleepTimerDone(); break;
        case 10: _t->rxSensorVersion((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->rxRobotError(); break;
        case 12: _t->moveToCoord((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SpotControl::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpotControl::moveToPos)) {
                *result = 0;
            }
        }
        {
            typedef void (SpotControl::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpotControl::clearError)) {
                *result = 1;
            }
        }
        {
            typedef void (SpotControl::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpotControl::complete)) {
                *result = 2;
            }
        }
        {
            typedef void (SpotControl::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpotControl::progress)) {
                *result = 3;
            }
        }
        {
            typedef void (SpotControl::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SpotControl::startTimer)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject SpotControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SpotControl.data,
      qt_meta_data_SpotControl,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SpotControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpotControl::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SpotControl.stringdata0))
        return static_cast<void*>(const_cast< SpotControl*>(this));
    return QObject::qt_metacast(_clname);
}

int SpotControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void SpotControl::moveToPos(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SpotControl::clearError()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void SpotControl::complete()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void SpotControl::progress(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SpotControl::startTimer(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE

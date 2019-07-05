/****************************************************************************
** Meta object code from reading C++ file 'robotcontrol.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../robotcontrol.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'robotcontrol.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RobotControl_t {
    QByteArrayData data[19];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RobotControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RobotControl_t qt_meta_stringdata_RobotControl = {
    {
QT_MOC_LITERAL(0, 0, 12), // "RobotControl"
QT_MOC_LITERAL(1, 13, 13), // "PortConnected"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 1), // "b"
QT_MOC_LITERAL(4, 30, 10), // "CurrentPos"
QT_MOC_LITERAL(5, 41, 1), // "x"
QT_MOC_LITERAL(6, 43, 1), // "y"
QT_MOC_LITERAL(7, 45, 1), // "z"
QT_MOC_LITERAL(8, 47, 8), // "txStatus"
QT_MOC_LITERAL(9, 56, 6), // "moving"
QT_MOC_LITERAL(10, 63, 14), // "errorGenerated"
QT_MOC_LITERAL(11, 78, 11), // "ConnectPort"
QT_MOC_LITERAL(12, 90, 4), // "port"
QT_MOC_LITERAL(13, 95, 12), // "moveAbsolute"
QT_MOC_LITERAL(14, 108, 10), // "alarmReset"
QT_MOC_LITERAL(15, 119, 9), // "getStatus"
QT_MOC_LITERAL(16, 129, 8), // "ReadData"
QT_MOC_LITERAL(17, 138, 7), // "timeOut"
QT_MOC_LITERAL(18, 146, 14) // "commandTimeOut"

    },
    "RobotControl\0PortConnected\0\0b\0CurrentPos\0"
    "x\0y\0z\0txStatus\0moving\0errorGenerated\0"
    "ConnectPort\0port\0moveAbsolute\0alarmReset\0"
    "getStatus\0ReadData\0timeOut\0commandTimeOut"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RobotControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    3,   72,    2, 0x06 /* Public */,
       8,    1,   79,    2, 0x06 /* Public */,
      10,    0,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   83,    2, 0x0a /* Public */,
      13,    3,   86,    2, 0x0a /* Public */,
      14,    0,   93,    2, 0x0a /* Public */,
      15,    0,   94,    2, 0x0a /* Public */,
      16,    0,   95,    2, 0x08 /* Private */,
      17,    0,   96,    2, 0x08 /* Private */,
      18,    0,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    5,    6,    7,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    5,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RobotControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RobotControl *_t = static_cast<RobotControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->PortConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->CurrentPos((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->txStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->errorGenerated(); break;
        case 4: _t->ConnectPort((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->moveAbsolute((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: _t->alarmReset(); break;
        case 7: _t->getStatus(); break;
        case 8: _t->ReadData(); break;
        case 9: _t->timeOut(); break;
        case 10: _t->commandTimeOut(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RobotControl::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RobotControl::PortConnected)) {
                *result = 0;
            }
        }
        {
            typedef void (RobotControl::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RobotControl::CurrentPos)) {
                *result = 1;
            }
        }
        {
            typedef void (RobotControl::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RobotControl::txStatus)) {
                *result = 2;
            }
        }
        {
            typedef void (RobotControl::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RobotControl::errorGenerated)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject RobotControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RobotControl.data,
      qt_meta_data_RobotControl,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RobotControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RobotControl::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RobotControl.stringdata0))
        return static_cast<void*>(const_cast< RobotControl*>(this));
    return QObject::qt_metacast(_clname);
}

int RobotControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void RobotControl::PortConnected(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RobotControl::CurrentPos(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RobotControl::txStatus(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RobotControl::errorGenerated()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'dlghardwaresettings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dlghardwaresettings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dlghardwaresettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_dlgHardwareSettings_t {
    QByteArrayData data[8];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dlgHardwareSettings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dlgHardwareSettings_t qt_meta_stringdata_dlgHardwareSettings = {
    {
QT_MOC_LITERAL(0, 0, 19), // "dlgHardwareSettings"
QT_MOC_LITERAL(1, 20, 15), // "txRobotPortName"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 3), // "str"
QT_MOC_LITERAL(4, 41, 16), // "rxRobotConnected"
QT_MOC_LITERAL(5, 58, 9), // "connected"
QT_MOC_LITERAL(6, 68, 21), // "on_bttn_Close_clicked"
QT_MOC_LITERAL(7, 90, 28) // "on_bttn_ConnectRobot_clicked"

    },
    "dlgHardwareSettings\0txRobotPortName\0"
    "\0str\0rxRobotConnected\0connected\0"
    "on_bttn_Close_clicked\0"
    "on_bttn_ConnectRobot_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dlgHardwareSettings[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   37,    2, 0x0a /* Public */,
       6,    0,   40,    2, 0x08 /* Private */,
       7,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void dlgHardwareSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        dlgHardwareSettings *_t = static_cast<dlgHardwareSettings *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->txRobotPortName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->rxRobotConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_bttn_Close_clicked(); break;
        case 3: _t->on_bttn_ConnectRobot_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (dlgHardwareSettings::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dlgHardwareSettings::txRobotPortName)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject dlgHardwareSettings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_dlgHardwareSettings.data,
      qt_meta_data_dlgHardwareSettings,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *dlgHardwareSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dlgHardwareSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_dlgHardwareSettings.stringdata0))
        return static_cast<void*>(const_cast< dlgHardwareSettings*>(this));
    return QDialog::qt_metacast(_clname);
}

int dlgHardwareSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void dlgHardwareSettings::txRobotPortName(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[66];
    char stringdata0[1179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 16), // "txRobotConnected"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 1), // "b"
QT_MOC_LITERAL(4, 31, 11), // "txRobotPort"
QT_MOC_LITERAL(5, 43, 3), // "str"
QT_MOC_LITERAL(6, 47, 13), // "txRobotAbsPos"
QT_MOC_LITERAL(7, 61, 1), // "x"
QT_MOC_LITERAL(8, 63, 1), // "y"
QT_MOC_LITERAL(9, 65, 1), // "z"
QT_MOC_LITERAL(10, 67, 15), // "txSensorVersion"
QT_MOC_LITERAL(11, 83, 7), // "version"
QT_MOC_LITERAL(12, 91, 20), // "txRobotStatusRequest"
QT_MOC_LITERAL(13, 112, 12), // "txDoneMoving"
QT_MOC_LITERAL(14, 125, 9), // "startSpot"
QT_MOC_LITERAL(15, 135, 18), // "txStartCoordinates"
QT_MOC_LITERAL(16, 154, 2), // "x1"
QT_MOC_LITERAL(17, 157, 2), // "y1"
QT_MOC_LITERAL(18, 160, 2), // "x2"
QT_MOC_LITERAL(19, 163, 2), // "y2"
QT_MOC_LITERAL(20, 166, 12), // "txRobotError"
QT_MOC_LITERAL(21, 179, 11), // "moveToCoord"
QT_MOC_LITERAL(22, 191, 8), // "datum_x1"
QT_MOC_LITERAL(23, 200, 9), // "datum1_y1"
QT_MOC_LITERAL(24, 210, 3), // "col"
QT_MOC_LITERAL(25, 214, 3), // "row"
QT_MOC_LITERAL(26, 218, 12), // "txClearError"
QT_MOC_LITERAL(27, 231, 16), // "rxRobotConnected"
QT_MOC_LITERAL(28, 248, 11), // "rxRobotPort"
QT_MOC_LITERAL(29, 260, 14), // "rxUpdatedReags"
QT_MOC_LITERAL(30, 275, 4), // "list"
QT_MOC_LITERAL(31, 280, 13), // "rxRobotStatus"
QT_MOC_LITERAL(32, 294, 6), // "moving"
QT_MOC_LITERAL(33, 301, 12), // "rxRobotError"
QT_MOC_LITERAL(34, 314, 14), // "fwdMoveCommand"
QT_MOC_LITERAL(35, 329, 10), // "rxProgress"
QT_MOC_LITERAL(36, 340, 8), // "progress"
QT_MOC_LITERAL(37, 349, 16), // "rxCompleteSignal"
QT_MOC_LITERAL(38, 366, 36), // "on_actionHardware_Settings_tr..."
QT_MOC_LITERAL(39, 403, 23), // "on_bttn_MoveAbs_clicked"
QT_MOC_LITERAL(40, 427, 24), // "on_actionAbout_triggered"
QT_MOC_LITERAL(41, 452, 40), // "on_actionReagent_Configuratio..."
QT_MOC_LITERAL(42, 493, 25), // "on_bttn_StartSpot_clicked"
QT_MOC_LITERAL(43, 519, 26), // "on_bttn_SetTopLeft_clicked"
QT_MOC_LITERAL(44, 546, 30), // "on_bttn_SetBottomRight_clicked"
QT_MOC_LITERAL(45, 577, 20), // "on_bttn_SetZ_clicked"
QT_MOC_LITERAL(46, 598, 37), // "on_actionSave_Configuration_t..."
QT_MOC_LITERAL(47, 636, 37), // "on_actionLoad_Configuration_t..."
QT_MOC_LITERAL(48, 674, 23), // "on_actionExit_triggered"
QT_MOC_LITERAL(49, 698, 23), // "on_actionHelp_triggered"
QT_MOC_LITERAL(50, 722, 29), // "on_bttn_QuickDatumOne_clicked"
QT_MOC_LITERAL(51, 752, 29), // "on_bttn_QuickDatumTwo_clicked"
QT_MOC_LITERAL(52, 782, 28), // "on_bttn_autoPopulate_clicked"
QT_MOC_LITERAL(53, 811, 24), // "on_bttnTestCoord_clicked"
QT_MOC_LITERAL(54, 836, 32), // "on_spin_vertGrid_v3_valueChanged"
QT_MOC_LITERAL(55, 869, 4), // "arg1"
QT_MOC_LITERAL(56, 874, 31), // "on_spin_horGrid_v3_valueChanged"
QT_MOC_LITERAL(57, 906, 32), // "on_spin_vertGrid_v4_valueChanged"
QT_MOC_LITERAL(58, 939, 31), // "on_spin_horGrid_v4_valueChanged"
QT_MOC_LITERAL(59, 971, 32), // "on_spin_overshootV3_valueChanged"
QT_MOC_LITERAL(60, 1004, 32), // "on_spin_overshootV4_valueChanged"
QT_MOC_LITERAL(61, 1037, 32), // "on_spin_v4_spotSize_valueChanged"
QT_MOC_LITERAL(62, 1070, 32), // "on_spin_v3_spotSize_valueChanged"
QT_MOC_LITERAL(63, 1103, 26), // "on_bttn_clearError_clicked"
QT_MOC_LITERAL(64, 1130, 42), // "on_combo_SensorVersion_curren..."
QT_MOC_LITERAL(65, 1173, 5) // "index"

    },
    "MainWindow\0txRobotConnected\0\0b\0"
    "txRobotPort\0str\0txRobotAbsPos\0x\0y\0z\0"
    "txSensorVersion\0version\0txRobotStatusRequest\0"
    "txDoneMoving\0startSpot\0txStartCoordinates\0"
    "x1\0y1\0x2\0y2\0txRobotError\0moveToCoord\0"
    "datum_x1\0datum1_y1\0col\0row\0txClearError\0"
    "rxRobotConnected\0rxRobotPort\0"
    "rxUpdatedReags\0list\0rxRobotStatus\0"
    "moving\0rxRobotError\0fwdMoveCommand\0"
    "rxProgress\0progress\0rxCompleteSignal\0"
    "on_actionHardware_Settings_triggered\0"
    "on_bttn_MoveAbs_clicked\0"
    "on_actionAbout_triggered\0"
    "on_actionReagent_Configuration_triggered\0"
    "on_bttn_StartSpot_clicked\0"
    "on_bttn_SetTopLeft_clicked\0"
    "on_bttn_SetBottomRight_clicked\0"
    "on_bttn_SetZ_clicked\0"
    "on_actionSave_Configuration_triggered\0"
    "on_actionLoad_Configuration_triggered\0"
    "on_actionExit_triggered\0on_actionHelp_triggered\0"
    "on_bttn_QuickDatumOne_clicked\0"
    "on_bttn_QuickDatumTwo_clicked\0"
    "on_bttn_autoPopulate_clicked\0"
    "on_bttnTestCoord_clicked\0"
    "on_spin_vertGrid_v3_valueChanged\0arg1\0"
    "on_spin_horGrid_v3_valueChanged\0"
    "on_spin_vertGrid_v4_valueChanged\0"
    "on_spin_horGrid_v4_valueChanged\0"
    "on_spin_overshootV3_valueChanged\0"
    "on_spin_overshootV4_valueChanged\0"
    "on_spin_v4_spotSize_valueChanged\0"
    "on_spin_v3_spotSize_valueChanged\0"
    "on_bttn_clearError_clicked\0"
    "on_combo_SensorVersion_currentIndexChanged\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      45,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  239,    2, 0x06 /* Public */,
       4,    1,  242,    2, 0x06 /* Public */,
       6,    3,  245,    2, 0x06 /* Public */,
      10,    1,  252,    2, 0x06 /* Public */,
      12,    0,  255,    2, 0x06 /* Public */,
      13,    0,  256,    2, 0x06 /* Public */,
      14,    0,  257,    2, 0x06 /* Public */,
      15,    5,  258,    2, 0x06 /* Public */,
      20,    0,  269,    2, 0x06 /* Public */,
      21,    4,  270,    2, 0x06 /* Public */,
      26,    0,  279,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      27,    1,  280,    2, 0x0a /* Public */,
      28,    1,  283,    2, 0x0a /* Public */,
      29,    1,  286,    2, 0x0a /* Public */,
      31,    1,  289,    2, 0x0a /* Public */,
      33,    0,  292,    2, 0x0a /* Public */,
      34,    3,  293,    2, 0x0a /* Public */,
      35,    1,  300,    2, 0x0a /* Public */,
      37,    0,  303,    2, 0x0a /* Public */,
      38,    0,  304,    2, 0x08 /* Private */,
      39,    0,  305,    2, 0x08 /* Private */,
      40,    0,  306,    2, 0x08 /* Private */,
      41,    0,  307,    2, 0x08 /* Private */,
      42,    0,  308,    2, 0x08 /* Private */,
      43,    0,  309,    2, 0x08 /* Private */,
      44,    0,  310,    2, 0x08 /* Private */,
      45,    0,  311,    2, 0x08 /* Private */,
      46,    0,  312,    2, 0x08 /* Private */,
      47,    0,  313,    2, 0x08 /* Private */,
      48,    0,  314,    2, 0x08 /* Private */,
      49,    0,  315,    2, 0x08 /* Private */,
      50,    0,  316,    2, 0x08 /* Private */,
      51,    0,  317,    2, 0x08 /* Private */,
      52,    0,  318,    2, 0x08 /* Private */,
      53,    0,  319,    2, 0x08 /* Private */,
      54,    1,  320,    2, 0x08 /* Private */,
      56,    1,  323,    2, 0x08 /* Private */,
      57,    1,  326,    2, 0x08 /* Private */,
      58,    1,  329,    2, 0x08 /* Private */,
      59,    1,  332,    2, 0x08 /* Private */,
      60,    1,  335,    2, 0x08 /* Private */,
      61,    1,  338,    2, 0x08 /* Private */,
      62,    1,  341,    2, 0x08 /* Private */,
      63,    0,  344,    2, 0x08 /* Private */,
      64,    1,  345,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    7,    8,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   16,   17,   18,   19,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   22,   23,   24,   25,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QStringList,   30,
    QMetaType::Void, QMetaType::Bool,   32,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    7,    8,    9,
    QMetaType::Void, QMetaType::Int,   36,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   55,
    QMetaType::Void, QMetaType::Int,   55,
    QMetaType::Void, QMetaType::Int,   55,
    QMetaType::Void, QMetaType::Int,   55,
    QMetaType::Void, QMetaType::Int,   55,
    QMetaType::Void, QMetaType::Int,   55,
    QMetaType::Void, QMetaType::Int,   55,
    QMetaType::Void, QMetaType::Int,   55,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   65,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->txRobotConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->txRobotPort((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->txRobotAbsPos((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->txSensorVersion((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->txRobotStatusRequest(); break;
        case 5: _t->txDoneMoving(); break;
        case 6: _t->startSpot(); break;
        case 7: _t->txStartCoordinates((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 8: _t->txRobotError(); break;
        case 9: _t->moveToCoord((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 10: _t->txClearError(); break;
        case 11: _t->rxRobotConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->rxRobotPort((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->rxUpdatedReags((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 14: _t->rxRobotStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->rxRobotError(); break;
        case 16: _t->fwdMoveCommand((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 17: _t->rxProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->rxCompleteSignal(); break;
        case 19: _t->on_actionHardware_Settings_triggered(); break;
        case 20: _t->on_bttn_MoveAbs_clicked(); break;
        case 21: _t->on_actionAbout_triggered(); break;
        case 22: _t->on_actionReagent_Configuration_triggered(); break;
        case 23: _t->on_bttn_StartSpot_clicked(); break;
        case 24: _t->on_bttn_SetTopLeft_clicked(); break;
        case 25: _t->on_bttn_SetBottomRight_clicked(); break;
        case 26: _t->on_bttn_SetZ_clicked(); break;
        case 27: _t->on_actionSave_Configuration_triggered(); break;
        case 28: _t->on_actionLoad_Configuration_triggered(); break;
        case 29: _t->on_actionExit_triggered(); break;
        case 30: _t->on_actionHelp_triggered(); break;
        case 31: _t->on_bttn_QuickDatumOne_clicked(); break;
        case 32: _t->on_bttn_QuickDatumTwo_clicked(); break;
        case 33: _t->on_bttn_autoPopulate_clicked(); break;
        case 34: _t->on_bttnTestCoord_clicked(); break;
        case 35: _t->on_spin_vertGrid_v3_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->on_spin_horGrid_v3_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->on_spin_vertGrid_v4_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->on_spin_horGrid_v4_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->on_spin_overshootV3_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->on_spin_overshootV4_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->on_spin_v4_spotSize_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->on_spin_v3_spotSize_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->on_bttn_clearError_clicked(); break;
        case 44: _t->on_combo_SensorVersion_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::txRobotConnected)) {
                *result = 0;
            }
        }
        {
            typedef void (MainWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::txRobotPort)) {
                *result = 1;
            }
        }
        {
            typedef void (MainWindow::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::txRobotAbsPos)) {
                *result = 2;
            }
        }
        {
            typedef void (MainWindow::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::txSensorVersion)) {
                *result = 3;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::txRobotStatusRequest)) {
                *result = 4;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::txDoneMoving)) {
                *result = 5;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::startSpot)) {
                *result = 6;
            }
        }
        {
            typedef void (MainWindow::*_t)(int , int , int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::txStartCoordinates)) {
                *result = 7;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::txRobotError)) {
                *result = 8;
            }
        }
        {
            typedef void (MainWindow::*_t)(int , int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::moveToCoord)) {
                *result = 9;
            }
        }
        {
            typedef void (MainWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::txClearError)) {
                *result = 10;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 45)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 45;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 45)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 45;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::txRobotConnected(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::txRobotPort(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::txRobotAbsPos(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::txSensorVersion(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::txRobotStatusRequest()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void MainWindow::txDoneMoving()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void MainWindow::startSpot()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void MainWindow::txStartCoordinates(int _t1, int _t2, int _t3, int _t4, int _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MainWindow::txRobotError()
{
    QMetaObject::activate(this, &staticMetaObject, 8, Q_NULLPTR);
}

// SIGNAL 9
void MainWindow::moveToCoord(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void MainWindow::txClearError()
{
    QMetaObject::activate(this, &staticMetaObject, 10, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

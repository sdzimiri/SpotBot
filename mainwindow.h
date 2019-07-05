#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dlghardwaresettings.h"
#include "spotcontrol.h"
#include "robotcontrol.h"
#include "dlgreagents.h"
#include <QThread>
#include <QGraphicsScene>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void txRobotConnected(bool b);
    void txRobotPort(QString str);
    void txRobotAbsPos(int x, int y, int z);
    void txSensorVersion(int version);
    void txRobotStatusRequest();
    void txDoneMoving();
    void startSpot();
    void txStartCoordinates(int x1, int y1, int x2, int y2, int z);
    void txRobotError();
    void moveToCoord(int datum_x1, int datum1_y1, int col, int row);
    void txClearError();

public slots:
    void rxRobotConnected(bool b);
    void rxRobotPort(QString str);
    void rxUpdatedReags(QStringList list);
    void rxRobotStatus(bool moving);
    void rxRobotError();
    void fwdMoveCommand(int x, int y, int z);
    void rxProgress(int progress);
    void rxCompleteSignal();

private slots:
    void on_actionHardware_Settings_triggered();
    void on_bttn_MoveAbs_clicked();
    void on_actionAbout_triggered();
    void on_actionReagent_Configuration_triggered();
    void on_bttn_StartSpot_clicked();
    void on_bttn_SetTopLeft_clicked();
    void on_bttn_SetBottomRight_clicked();
    void on_bttn_SetZ_clicked();

    void on_actionSave_Configuration_triggered();

    void on_actionLoad_Configuration_triggered();

    void on_actionExit_triggered();

    void on_actionHelp_triggered();

    void on_bttn_QuickDatumOne_clicked();

    void on_bttn_QuickDatumTwo_clicked();

    void on_bttn_autoPopulate_clicked();

    void on_bttnTestCoord_clicked();

    void on_spin_vertGrid_v3_valueChanged(int arg1);

    void on_spin_horGrid_v3_valueChanged(int arg1);

    void on_spin_vertGrid_v4_valueChanged(int arg1);

    void on_spin_horGrid_v4_valueChanged(int arg1);

    void on_spin_overshootV3_valueChanged(int arg1);

    void on_spin_overshootV4_valueChanged(int arg1);

    void on_spin_v4_spotSize_valueChanged(int arg1);

    void on_spin_v3_spotSize_valueChanged(int arg1);

    void on_bttn_clearError_clicked();

    void on_combo_SensorVersion_currentIndexChanged(int index);

    void on_groupBox_4_clicked();

private:

    void updateReagAssignments();
    void addReagentToSensors(QString name, QVariant v);
    void clearReagents();
    void ejectTip();
    void setReagAssignments();
    void loadReagAssignments();
    int incrementReagCount(int count,int max);
    void updateSpotGraphics();

    Ui::MainWindow *ui;

    //Sensor version
    int sensor_version;

    //Hardware control classes & Threads
    QThread *controlThread;
    SpotControl *spotCtrl;
    RobotControl *rbtCtrl;

    //Other windows
    dlgHardwareSettings *hardSet;
    dlgReagents *reagSet;

    //Program state vars
    QString reagList[20];   //list of all available (entered) reagents
    int reagAssign[8][15];     //array of each sensor location, and what reagent (indexed) is designated at each location (sensor V1/V2)
    int reagAssign_v3[8];     //array of each sensor location, and what reagent (indexed) is designated at each location (sensor V3)
    int reagAssign_v4[2][8];     //array of each sensor location, and what reagent (indexed) is designated at each location (sensor V4)
    int reagAssign_hs[6][4];    //array of each sensor location, and what reagent (indexed) is designated at each location (hyperspectral apertures)
    bool robotConnected;
    bool topLeftClicked;
    bool bottomRightClicked;
    bool zClicked;

    bool updateManualCoords;

    //Graphics Scenes for spotting layouts
    QGraphicsScene *v3Scene;
    QGraphicsScene *v4Scene;

};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlghardwaresettings.h"
#include "robotcontrol.h"
#include "dlgabout.h"
#include "dlgreagents.h"
#include "dlghelp.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>


//Default Constructor - Set up UI
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //Initialize member classes
    rbtCtrl = new RobotControl();
    hardSet = new dlgHardwareSettings();
    reagSet = new dlgReagents();

    //Set up automation thread
    spotCtrl = new SpotControl();
    controlThread = new QThread();
    spotCtrl->moveToThread(controlThread);

    //Connect automation thread signals / slots
    connect(this,SIGNAL(startSpot()),spotCtrl,SLOT(startSpotting()));
    connect(spotCtrl,SIGNAL(moveToPos(int,int,int)),this,SLOT(fwdMoveCommand(int,int,int)));
    connect(this,SIGNAL(txDoneMoving()),spotCtrl,SLOT(doneMoving()));
    connect(this,SIGNAL(txStartCoordinates(int,int,int,int,int)),spotCtrl,SLOT(rxStartCoordinates(int,int,int,int,int)));
    connect(spotCtrl,SIGNAL(complete()),this,SLOT(rxCompleteSignal()));
    connect(spotCtrl,SIGNAL(progress(int)),this,SLOT(rxProgress(int)));
    connect(this,SIGNAL(txSensorVersion(int)),spotCtrl,SLOT(rxSensorVersion(int)));
    connect(this,SIGNAL(txRobotError()),spotCtrl,SLOT(rxRobotError()));
    connect(this,SIGNAL(moveToCoord(int,int,int,int)),spotCtrl,SLOT(moveToCoord(int,int,int,int)));
    controlThread->start();

    //Connect Signals/Slots for Hardware Settings Window
    connect(hardSet,SIGNAL(txRobotPortName(QString)),this,SLOT(rxRobotPort(QString)));
    connect(this,SIGNAL(txRobotConnected(bool)),hardSet,SLOT(rxRobotConnected(bool)));

    //Connect Signals/Slots for Robot Control Class
    connect(rbtCtrl,SIGNAL(PortConnected(bool)),this,SLOT(rxRobotConnected(bool)));
    connect(this,SIGNAL(txRobotPort(QString)),rbtCtrl,SLOT(ConnectPort(QString)));
    connect(this,SIGNAL(txRobotAbsPos(int,int,int)),rbtCtrl,SLOT(moveAbsolute(int,int,int)));
    connect(this,SIGNAL(txRobotStatusRequest()),rbtCtrl,SLOT(getStatus()));
    connect(this,SIGNAL(txClearError()),rbtCtrl,SLOT(alarmReset()));
    connect(rbtCtrl,SIGNAL(txStatus(bool)),this,SLOT(rxRobotStatus(bool)));
    connect(rbtCtrl,SIGNAL(errorGenerated()),this,SLOT(rxRobotError()));


    //Connect Signals/Slots for Reagent Settings Window
    connect(reagSet,SIGNAL(txUpdatedReagents(QStringList)),this,SLOT(rxUpdatedReags(QStringList)));

    //Initialize local vars
    robotConnected = false;
    topLeftClicked = false;
    bottomRightClicked = false;
    zClicked = false;
    updateManualCoords = false;

    //Initialize string array
    for (int i =0;i<20;i++)
        reagList[i] = "";

    //Initialize sensor / reagent assignment array for V1/V2
    for (int i =0;i<8;i++)
        for (int j=0;j<15;j++)
            reagAssign[i][j] = -1;

    //Inialize sensor / reagent assignment array for V3
    for (int i =0;i<8;i++)
        reagAssign_v3[i] = -1;

    //Inialize sensor / reagent assignment array for V4
    for (int i =0;i<2;i++)
        for (int j =0;j<8;j++)
            reagAssign_v4[i][j] = -1;

    //Inialize sensor / reagent assignment array for HS APER
    for (int i =0;i<6;i++)
        for (int j =0;j<4;j++)
            reagAssign_hs[i][j] = -1;


    addReagentToSensors("None",QVariant(-1));

    //Initialize Graphics for V3/V4 spotting maps
    v3Scene = new QGraphicsScene();
    v4Scene = new QGraphicsScene();

    updateSpotGraphics();

    //Sensor version default
    ui->combo_SensorVersion->addItem("NanoFET v1",QVariant(1));
    ui->combo_SensorVersion->addItem("NanoFET v2",QVariant(2));
    ui->combo_SensorVersion->addItem("NanoFET v3",QVariant(3));
    ui->combo_SensorVersion->addItem("NanoFET v4",QVariant(4));
    ui->combo_SensorVersion->addItem("Hyperspectral Aperture",QVariant(5));

    //sensor version default
    sensor_version = ui->combo_SensorVersion->currentIndex() + 1;

}

//Forward move command from spotting automation thread
void MainWindow::fwdMoveCommand(int x, int y, int z)
{
    if (updateManualCoords)
    {
        ui->spin_Xpos->setValue(x);
        ui->spin_Ypos->setValue(y);
        ui->spin_Zpos->setValue(z);
    }
    emit txRobotAbsPos(x,y,z);
}

//Private function, updates all sensor comboboxes with refreshed reagent selections and indices
void MainWindow::updateReagAssignments()
{
    //Clear current entries
    clearReagents();

    //Add the none option
    addReagentToSensors("None",QVariant(-1));

    //Iterate through list of reagents, adding non-empty ones to the comboboxes
    for (int i = 0;i<20;i++)
    {
        if (reagList[i].isEmpty() == false)
            addReagentToSensors(reagList[i],QVariant(i));
    }

}

//Slot for receiving updated reagent lists from reagent configuration
void MainWindow::rxUpdatedReags(QStringList list)
{
    for (int i=0;i<20;i++)
        reagList[i] = list[i];

    updateReagAssignments();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Robot Control Class has transmitted its connection status, forward to hardware config window
void MainWindow::rxRobotConnected(bool b)
{
    emit txRobotConnected(b);

    robotConnected = b;

    if (b)
    {
        ui->lbl_robotStatus->setText("Connected");
        ui->spin_Xpos->setEnabled(true);
        ui->spin_Ypos->setEnabled(true);
        ui->spin_Zpos->setEnabled(true);
        ui->bttn_MoveAbs->setEnabled(true);
        ui->bttn_QuickDatumOne->setEnabled(true);
        //ui->bttn_QuickDatumTwo->setEnabled(true);

        ui->spin_Xpos->setValue(132000);
        ui->spin_Ypos->setValue(65000);
        ui->spin_Zpos->setValue(15000);

        //test coordinate
        ui->bttnTestCoord->setEnabled(true);
        ui->spin_TestCoordColumn->setEnabled(true);
        ui->spin_TestCoordRow->setEnabled(true);


    }
    else
    {
        ui->lbl_robotStatus->setText("Disconnected");
        ui->spin_Xpos->setEnabled(false);
        ui->spin_Ypos->setEnabled(false);
        ui->spin_Zpos->setEnabled(false);
        ui->bttn_MoveAbs->setEnabled(false);
        ui->bttn_QuickDatumOne->setEnabled(false);
        ui->bttn_QuickDatumTwo->setEnabled(false);
    }

    //If both instruments are connected, enable spotting controls
    if (robotConnected)
    {
        ui->bttn_StartSpot->setEnabled(true);
        ui->comboBox_testSpot_1->setEnabled(true);
        ui->comboBox_testSpot_2->setEnabled(true);
        ui->bttn_SetTopLeft->setEnabled(true);
        //ui->bttn_SetBottomRight->setEnabled(true);
        ui->bttn_SetZ->setEnabled(true);
        ui->spin_topLeftX->setEnabled(true);
        ui->spin_topLeftY->setEnabled(true);
        //ui->spin_bottomRightX->setEnabled(true);
        //ui->spin_bottomRightY->setEnabled(true);
        ui->spin_Z->setEnabled(true);
        ui->spin_ScriptLoopCount->setEnabled(true);
    }
    else
    {
        ui->comboBox_testSpot_1->setEnabled(false);
        ui->comboBox_testSpot_2->setEnabled(false);
        ui->bttn_SetTopLeft->setEnabled(false);
        ui->bttn_SetBottomRight->setEnabled(false);
        ui->bttn_SetZ->setEnabled(false);
        ui->spin_topLeftX->setEnabled(false);
        ui->spin_topLeftY->setEnabled(false);
        ui->spin_bottomRightX->setEnabled(false);
        ui->spin_bottomRightY->setEnabled(false);
        ui->spin_Z->setEnabled(false);
        ui->spin_ScriptLoopCount->setEnabled(false);
    }

}

//Slot for receiving port for the robot
void MainWindow::rxRobotPort(QString str)
{
    emit txRobotPort(str);
}

//User clicks on absolute move
void MainWindow::on_bttn_MoveAbs_clicked()
{
    emit txRobotAbsPos(ui->spin_Xpos->value(),ui->spin_Ypos->value(),ui->spin_Zpos->value());
}

//User clicks on "About"
void MainWindow::on_actionAbout_triggered()
{
    dlgAbout *dlg = new dlgAbout();
    dlg->show();
}

//User selects to open hardwar settings
void MainWindow::on_actionHardware_Settings_triggered()
{
    hardSet->updateGui();
    hardSet->show();
}

//User clicks on Reagent configuration
void MainWindow::on_actionReagent_Configuration_triggered()
{
    reagSet->setReagents(reagList);
    reagSet->show();
}

//Clear combobox entries
void MainWindow::clearReagents()
{
    //SENSOR V1/V2 #################
    //column 13
    ui->comboBox_c13r18->clear();
    ui->comboBox_c13r19->clear();
    ui->comboBox_c13r21->clear();
    ui->comboBox_c13r23->clear();
    ui->comboBox_c13r25->clear();
    ui->comboBox_c13r27->clear();
    ui->comboBox_c13r30->clear();
    ui->comboBox_c13r31->clear();
    ui->comboBox_c13r32->clear();

    //column 14
    ui->comboBox_c14r18->clear();
    ui->comboBox_c14r19->clear();
    ui->comboBox_c14r22->clear();
    ui->comboBox_c14r24->clear();
    ui->comboBox_c14r26->clear();
    ui->comboBox_c14r28->clear();
    ui->comboBox_c14r30->clear();
    ui->comboBox_c14r31->clear();
    ui->comboBox_c14r32->clear();

    //Column 15
    ui->comboBox_c15r18->clear();
    ui->comboBox_c15r19->clear();
    ui->comboBox_c15r21->clear();
    ui->comboBox_c15r23->clear();
    ui->comboBox_c15r25->clear();
    ui->comboBox_c15r27->clear();
    ui->comboBox_c15r30->clear();
    ui->comboBox_c15r31->clear();
    ui->comboBox_c15r32->clear();

    //Column 16
    ui->comboBox_c16r18->clear();
    ui->comboBox_c16r19->clear();
    ui->comboBox_c16r22->clear();
    ui->comboBox_c16r24->clear();
    ui->comboBox_c16r26->clear();
    ui->comboBox_c16r28->clear();
    ui->comboBox_c16r30->clear();
    ui->comboBox_c16r31->clear();
    ui->comboBox_c16r32->clear();

    //Column 17
    ui->comboBox_c17r18->clear();
    ui->comboBox_c17r19->clear();
    ui->comboBox_c17r21->clear();
    ui->comboBox_c17r23->clear();
    ui->comboBox_c17r25->clear();
    ui->comboBox_c17r27->clear();
    ui->comboBox_c17r30->clear();
    ui->comboBox_c17r31->clear();
    ui->comboBox_c17r32->clear();

    //Column 18
    ui->comboBox_c18r18->clear();
    ui->comboBox_c18r19->clear();
    ui->comboBox_c18r22->clear();
    ui->comboBox_c18r24->clear();
    ui->comboBox_c18r26->clear();
    ui->comboBox_c18r28->clear();
    ui->comboBox_c18r30->clear();
    ui->comboBox_c18r31->clear();
    ui->comboBox_c18r32->clear();

    //Column 19
    ui->comboBox_c19r18->clear();
    ui->comboBox_c19r19->clear();
    ui->comboBox_c19r21->clear();
    ui->comboBox_c19r23->clear();
    ui->comboBox_c19r25->clear();
    ui->comboBox_c19r27->clear();
    ui->comboBox_c19r30->clear();
    ui->comboBox_c19r31->clear();
    ui->comboBox_c19r32->clear();

    //Column 20
    ui->comboBox_c20r19->clear();

    //Test Spot
    ui->comboBox_testSpot_1->clear();
    ui->comboBox_testSpot_2->clear();

    //SENSOR V3 ###################

    ui->comboBox_v3_1->clear();
    ui->comboBox_v3_2->clear();
    ui->comboBox_v3_3->clear();
    ui->comboBox_v3_4->clear();
    ui->comboBox_v3_5->clear();
    ui->comboBox_v3_6->clear();
    ui->comboBox_v3_7->clear();
    ui->comboBox_v3_8->clear();

    //SENSOR V4 ###################

    ui->comboBox_v4_c1r1->clear();
    ui->comboBox_v4_c1r2->clear();
    ui->comboBox_v4_c1r3->clear();
    ui->comboBox_v4_c1r4->clear();
    ui->comboBox_v4_c1r5->clear();
    ui->comboBox_v4_c1r6->clear();
    ui->comboBox_v4_c1r7->clear();
    ui->comboBox_v4_c1r8->clear();

    ui->comboBox_v4_c2r1->clear();
    ui->comboBox_v4_c2r2->clear();
    ui->comboBox_v4_c2r3->clear();
    ui->comboBox_v4_c2r4->clear();
    ui->comboBox_v4_c2r5->clear();
    ui->comboBox_v4_c2r6->clear();
    ui->comboBox_v4_c2r7->clear();
    ui->comboBox_v4_c2r8->clear();

    // HYPERSPECTRAL APERTURES #######

    ui->comboBox_hsa_c1r1->clear();
    ui->comboBox_hsa_c1r2->clear();
    ui->comboBox_hsa_c1r3->clear();
    ui->comboBox_hsa_c1r4->clear();

    ui->comboBox_hsa_c2r1->clear();
    ui->comboBox_hsa_c2r2->clear();
    ui->comboBox_hsa_c2r3->clear();
    ui->comboBox_hsa_c2r4->clear();

    ui->comboBox_hsa_c3r1->clear();
    ui->comboBox_hsa_c3r2->clear();
    ui->comboBox_hsa_c3r3->clear();
    ui->comboBox_hsa_c3r4->clear();

    ui->comboBox_hsa_c4r1->clear();
    ui->comboBox_hsa_c4r2->clear();
    ui->comboBox_hsa_c4r3->clear();
    ui->comboBox_hsa_c4r4->clear();

    ui->comboBox_hsa_c5r1->clear();
    ui->comboBox_hsa_c5r2->clear();
    ui->comboBox_hsa_c5r3->clear();
    ui->comboBox_hsa_c5r4->clear();

    ui->comboBox_hsa_c6r1->clear();
    ui->comboBox_hsa_c6r2->clear();
    ui->comboBox_hsa_c6r3->clear();
    ui->comboBox_hsa_c6r4->clear();




}

//Updates selected GUI values based upon loaded config file
void MainWindow::loadReagAssignments()
{
    int i;

    //SENSOR V1 / V2 ########################################################
    //column 13~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    i = 0;
    while(reagAssign[0][0] != ui->comboBox_c13r18->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c13r18->setCurrentIndex(i);

    i = 0;
    while(reagAssign[0][1] != ui->comboBox_c13r19->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c13r19->setCurrentIndex(i);

    i = 0;
    while(reagAssign[0][3] != ui->comboBox_c13r21->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c13r21->setCurrentIndex(i);

    i = 0;
    while(reagAssign[0][5] != ui->comboBox_c13r23->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c13r23->setCurrentIndex(i);

    i = 0;
    while(reagAssign[0][7] != ui->comboBox_c13r25->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c13r25->setCurrentIndex(i);

    i = 0;
    while(reagAssign[0][9] != ui->comboBox_c13r27->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c13r27->setCurrentIndex(i);

    i = 0;
    while(reagAssign[0][12] != ui->comboBox_c13r30->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c13r30->setCurrentIndex(i);

    i = 0;
    while(reagAssign[0][13] != ui->comboBox_c13r31->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c13r31->setCurrentIndex(i);

    i = 0;
    while(reagAssign[0][14] != ui->comboBox_c13r32->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c13r32->setCurrentIndex(i);

    //column 14~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    i = 0;
    while(reagAssign[1][0] != ui->comboBox_c14r18->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c14r18->setCurrentIndex(i);

    i = 0;
    while(reagAssign[1][1] != ui->comboBox_c14r19->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c14r19->setCurrentIndex(i);

    i = 0;
    while(reagAssign[1][4] != ui->comboBox_c14r22->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c14r22->setCurrentIndex(i);

    i = 0;
    while(reagAssign[1][6] != ui->comboBox_c14r24->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c14r24->setCurrentIndex(i);

    i = 0;
    while(reagAssign[1][8] != ui->comboBox_c14r26->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c14r26->setCurrentIndex(i);

    i = 0;
    while(reagAssign[1][10] != ui->comboBox_c14r28->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c14r28->setCurrentIndex(i);

    i = 0;
    while(reagAssign[1][12] != ui->comboBox_c14r30->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c14r30->setCurrentIndex(i);

    i = 0;
    while(reagAssign[1][13] != ui->comboBox_c14r31->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c14r31->setCurrentIndex(i);

    i = 0;
    while(reagAssign[1][14] != ui->comboBox_c14r32->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c14r32->setCurrentIndex(i);

    //column 15~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    i = 0;
    while(reagAssign[2][0] != ui->comboBox_c15r18->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c15r18->setCurrentIndex(i);

    i = 0;
    while(reagAssign[2][1] != ui->comboBox_c15r19->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c15r19->setCurrentIndex(i);

    i = 0;
    while(reagAssign[2][3] != ui->comboBox_c15r21->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c15r21->setCurrentIndex(i);

    i = 0;
    while(reagAssign[2][5] != ui->comboBox_c15r23->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c15r23->setCurrentIndex(i);

    i = 0;
    while(reagAssign[2][7] != ui->comboBox_c15r25->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c15r25->setCurrentIndex(i);

    i = 0;
    while(reagAssign[2][9] != ui->comboBox_c15r27->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c15r27->setCurrentIndex(i);

    i = 0;
    while(reagAssign[2][12] != ui->comboBox_c15r30->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c15r30->setCurrentIndex(i);

    i = 0;
    while(reagAssign[2][13] != ui->comboBox_c15r31->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c15r31->setCurrentIndex(i);

    i = 0;
    while(reagAssign[2][14] != ui->comboBox_c15r32->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c15r32->setCurrentIndex(i);

    //column 16~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    i = 0;
    while(reagAssign[3][0] != ui->comboBox_c16r18->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c16r18->setCurrentIndex(i);

    i = 0;
    while(reagAssign[3][1] != ui->comboBox_c16r19->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c16r19->setCurrentIndex(i);

    i = 0;
    while(reagAssign[3][4] != ui->comboBox_c16r22->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c16r22->setCurrentIndex(i);

    i = 0;
    while(reagAssign[3][6] != ui->comboBox_c16r24->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c16r24->setCurrentIndex(i);

    i = 0;
    while(reagAssign[3][8] != ui->comboBox_c16r26->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c16r26->setCurrentIndex(i);

    i = 0;
    while(reagAssign[3][10] != ui->comboBox_c16r28->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c16r28->setCurrentIndex(i);

    i = 0;
    while(reagAssign[3][12] != ui->comboBox_c16r30->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c16r30->setCurrentIndex(i);

    i = 0;
    while(reagAssign[3][13] != ui->comboBox_c16r31->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c16r31->setCurrentIndex(i);

    i = 0;
    while(reagAssign[3][14] != ui->comboBox_c16r32->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c16r32->setCurrentIndex(i);

    //column 17~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    i = 0;
    while(reagAssign[4][0] != ui->comboBox_c17r18->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c17r18->setCurrentIndex(i);

    i = 0;
    while(reagAssign[4][1] != ui->comboBox_c17r19->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c17r19->setCurrentIndex(i);

    i = 0;
    while(reagAssign[4][3] != ui->comboBox_c17r21->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c17r21->setCurrentIndex(i);

    i = 0;
    while(reagAssign[4][5] != ui->comboBox_c17r23->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c17r23->setCurrentIndex(i);

    i = 0;
    while(reagAssign[4][7] != ui->comboBox_c17r25->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c17r25->setCurrentIndex(i);

    i = 0;
    while(reagAssign[4][9] != ui->comboBox_c17r27->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c17r27->setCurrentIndex(i);

    i = 0;
    while(reagAssign[4][12] != ui->comboBox_c17r30->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c17r30->setCurrentIndex(i);

    i = 0;
    while(reagAssign[4][13] != ui->comboBox_c17r31->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c17r31->setCurrentIndex(i);

    i = 0;
    while(reagAssign[4][14] != ui->comboBox_c17r32->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c17r32->setCurrentIndex(i);

    //column 18~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    i = 0;
    while(reagAssign[5][0] != ui->comboBox_c18r18->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c18r18->setCurrentIndex(i);

    i = 0;
    while(reagAssign[5][1] != ui->comboBox_c18r19->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c18r19->setCurrentIndex(i);

    i = 0;
    while(reagAssign[5][4] != ui->comboBox_c18r22->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c18r22->setCurrentIndex(i);

    i = 0;
    while(reagAssign[5][6] != ui->comboBox_c18r24->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c18r24->setCurrentIndex(i);

    i = 0;
    while(reagAssign[5][8] != ui->comboBox_c18r26->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c18r26->setCurrentIndex(i);

    i = 0;
    while(reagAssign[5][10] != ui->comboBox_c18r28->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c18r28->setCurrentIndex(i);

    i = 0;
    while(reagAssign[5][12] != ui->comboBox_c18r30->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c18r30->setCurrentIndex(i);

    i = 0;
    while(reagAssign[5][13] != ui->comboBox_c18r31->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c18r31->setCurrentIndex(i);

    i = 0;
    while(reagAssign[5][14] != ui->comboBox_c18r32->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c18r32->setCurrentIndex(i);

    //column 19~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    i = 0;
    while(reagAssign[6][0] != ui->comboBox_c19r18->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c19r18->setCurrentIndex(i);

    i = 0;
    while(reagAssign[6][1] != ui->comboBox_c19r19->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c19r19->setCurrentIndex(i);

    i = 0;
    while(reagAssign[6][3] != ui->comboBox_c19r21->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c19r21->setCurrentIndex(i);

    i = 0;
    while(reagAssign[6][5] != ui->comboBox_c19r23->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c19r23->setCurrentIndex(i);

    i = 0;
    while(reagAssign[6][7] != ui->comboBox_c19r25->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c19r25->setCurrentIndex(i);

    i = 0;
    while(reagAssign[6][9] != ui->comboBox_c19r27->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c19r27->setCurrentIndex(i);

    i = 0;
    while(reagAssign[6][12] != ui->comboBox_c19r30->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c19r30->setCurrentIndex(i);

    i = 0;
    while(reagAssign[6][13] != ui->comboBox_c19r31->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c19r31->setCurrentIndex(i);

    i = 0;
    while(reagAssign[6][14] != ui->comboBox_c19r32->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c19r32->setCurrentIndex(i);

    //Column 20 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    i = 0;
    while(reagAssign[7][1] != ui->comboBox_c20r19->itemData(i).toInt())
        i= i+1;
    ui->comboBox_c20r19->setCurrentIndex(i);

    //SENSOR V3 ########################################################

    i = 0;
    while(reagAssign_v3[0] != ui->comboBox_v3_1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v3_1->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v3[1] != ui->comboBox_v3_2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v3_2->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v3[2] != ui->comboBox_v3_3->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v3_3->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v3[3] != ui->comboBox_v3_4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v3_4->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v3[4] != ui->comboBox_v3_4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v3_4->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v3[5] != ui->comboBox_v3_5->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v3_5->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v3[6] != ui->comboBox_v3_6->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v3_6->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v3[7] != ui->comboBox_v3_7->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v3_7->setCurrentIndex(i);

    //SENSOR V4 ########################################################

    //Column 1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    i = 0;
    while(reagAssign_v4[0][0] != ui->comboBox_v4_c1r1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c1r1->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[0][1] != ui->comboBox_v4_c1r2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c1r2->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[0][2] != ui->comboBox_v4_c1r3->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c1r3->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[0][3] != ui->comboBox_v4_c1r4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c1r4->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[0][4] != ui->comboBox_v4_c1r5->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c1r5->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[0][5] != ui->comboBox_v4_c1r6->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c1r6->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[0][6] != ui->comboBox_v4_c1r7->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c1r7->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[0][7] != ui->comboBox_v4_c1r8->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c1r8->setCurrentIndex(i);

    //Column 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    i = 0;
    while(reagAssign_v4[1][0] != ui->comboBox_v4_c2r1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c2r1->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[1][1] != ui->comboBox_v4_c2r2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c2r2->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[1][2] != ui->comboBox_v4_c2r3->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c2r3->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[1][3] != ui->comboBox_v4_c2r4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c2r4->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[1][4] != ui->comboBox_v4_c2r5->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c2r5->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[1][5] != ui->comboBox_v4_c2r6->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c2r6->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[1][6] != ui->comboBox_v4_c2r7->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c2r7->setCurrentIndex(i);

    i = 0;
    while(reagAssign_v4[1][7] != ui->comboBox_v4_c2r8->itemData(i).toInt())
        i= i+1;
    ui->comboBox_v4_c2r8->setCurrentIndex(i);


    //HYPERSPECTRAL APERTURES##################################################

    //Column 1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    i = 0;
    while(reagAssign_hs[0][0] != ui->comboBox_hsa_c1r1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c1r1->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[0][1] != ui->comboBox_hsa_c1r2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c1r2->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[0][2] != ui->comboBox_hsa_c1r3->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c1r3->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[0][3] != ui->comboBox_hsa_c1r4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c1r4->setCurrentIndex(i);

    //Column 2 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    i = 0;
    while(reagAssign_hs[1][0] != ui->comboBox_hsa_c2r1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c2r1->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[1][1] != ui->comboBox_hsa_c2r2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c2r2->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[1][2] != ui->comboBox_hsa_c2r3->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c2r3->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[1][3] != ui->comboBox_hsa_c2r4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c2r4->setCurrentIndex(i);

    //Column 3 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    i = 0;
    while(reagAssign_hs[2][0] != ui->comboBox_hsa_c3r1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c3r1->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[2][1] != ui->comboBox_hsa_c3r2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c3r2->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[2][2] != ui->comboBox_hsa_c3r3->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c3r3->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[2][3] != ui->comboBox_hsa_c3r4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c3r4->setCurrentIndex(i);

    //Column 4 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    i = 0;
    while(reagAssign_hs[3][0] != ui->comboBox_hsa_c4r1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c4r1->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[3][1] != ui->comboBox_hsa_c4r2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c4r2->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[3][2] != ui->comboBox_hsa_c4r3->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c4r3->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[3][3] != ui->comboBox_hsa_c4r4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c4r4->setCurrentIndex(i);

    //Column 5 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    i = 0;
    while(reagAssign_hs[4][0] != ui->comboBox_hsa_c5r1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c5r1->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[4][1] != ui->comboBox_hsa_c5r2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c5r2->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[4][2] != ui->comboBox_hsa_c5r3->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c5r3->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[4][3] != ui->comboBox_hsa_c5r4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c5r4->setCurrentIndex(i);

    //Column 6 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    i = 0;
    while(reagAssign_hs[5][0] != ui->comboBox_hsa_c6r1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c6r1->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[5][1] != ui->comboBox_hsa_c6r2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c6r2->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[5][2] != ui->comboBox_hsa_c6r3->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c6r3->setCurrentIndex(i);

    i = 0;
    while(reagAssign_hs[5][3] != ui->comboBox_hsa_c6r4->itemData(i).toInt())
        i= i+1;
    ui->comboBox_hsa_c6r4->setCurrentIndex(i);


}

//Helper function for automatic reagent population
int MainWindow::incrementReagCount(int count, int max)
{
    count = count +1;
    if (count >= max)
        count = 1;

    return count;
}

//This will update the spotting tool graphics for sensor V3/V4
void MainWindow::updateSpotGraphics()
{

    //DRAW SPOT V3 GRAPHICS
    QPixmap *pix = new QPixmap(296,167);
    QPainter *paint = new QPainter(pix);

    int width = 264;
    int height = 33;
    int x_0 = 16;
    int y_0 = 67;
    int d = 26*float(ui->spin_v3_spotSize->value())/400;

    //Draw backgrounds
    paint->setPen(Qt::white);
    paint->setBrush(Qt::white);
    paint->drawRect(0,0,296,166); //canvas size

    paint->setPen(Qt::gray);
    paint->setBrush(Qt::gray);
    paint->drawRect(x_0,y_0,width,height);


    int num_y = ui->spin_vertGrid_v3->value();
    int num_x = ui->spin_horGrid_v3->value();
    float overshoot = float(ui->spin_overshootV3->value())/100;

    //Divide width by num_x+1, draw circles
    paint->setPen(Qt::green);
    paint->setBrush(Qt::transparent);
    float x_pitch = float(width+width*overshoot*2)/float(num_x+1);
    float y_pitch = float(height+height*overshoot*2)/float(num_y+1);

    for(int i=1;i<num_y+1;i++)
        for(int j=1;j<num_x+1;j++)
            paint->drawEllipse(x_0-width*overshoot+x_pitch*j-d/2,y_0-height*overshoot+y_pitch*i-d/2,d,d);



    v3Scene->addPixmap(*pix);
    ui->graphicsView_SensorV3->setScene(v3Scene);

    //DRAW SPOT V4 GRAPHICS

    pix = new QPixmap(296,166);
    paint = new QPainter(pix);

    width = 264;
    height = 88;
    x_0 = 16;
    y_0 = 39;
    d = 70*float(ui->spin_v4_spotSize->value())/400;

    //Draw backgrounds
    paint->setPen(Qt::white);
    paint->setBrush(Qt::white);
    paint->drawRect(0,0,296,166); //canvas size

    paint->setPen(Qt::gray);
    paint->setBrush(Qt::gray);
    paint->drawRect(x_0,y_0,width,height);


    num_y = ui->spin_vertGrid_v4->value();
    num_x = ui->spin_horGrid_v4->value();
    overshoot = float(ui->spin_overshootV4->value())/100;

    //Divide width by num_x+1, draw circles
    paint->setPen(Qt::green);
    paint->setBrush(Qt::transparent);
    x_pitch = float(width+width*overshoot*2)/float(num_x+1);
    y_pitch = float(height+height*overshoot*2)/float(num_y+1);

    for(int i=1;i<num_y+1;i++)
        for(int j=1;j<num_x+1;j++)
            paint->drawEllipse(x_0-width*overshoot+x_pitch*j-d/2,y_0-height*overshoot+y_pitch*i-d/2,d,d);

    v4Scene->addPixmap(*pix);
    ui->graphicsView_SensorV4->setScene(v4Scene);

}

//Copies currently assigned reagents to the class variable
void MainWindow::setReagAssignments()
{
    //Reinitialize reagent assignment array for all versions
    for (int i=0;i<8;i++)
        for (int j = 0; j< 8;j++)
            reagAssign[i][j] = -1;

    for (int i=0; i< 8; i++)
        reagAssign_v3[i] = -1;

    for (int i=0; i<2; i++)
        for (int j =0; j < 8; j++)
            reagAssign_v4[i][j] = -1;

    //SENSOR V1/V2 ##########################################################
    //column 13
    reagAssign[0][0] = ui->comboBox_c13r18->currentData().toInt();
    reagAssign[0][1] = ui->comboBox_c13r19->currentData().toInt();
    reagAssign[0][3] = ui->comboBox_c13r21->currentData().toInt();
    reagAssign[0][5] = ui->comboBox_c13r23->currentData().toInt();
    reagAssign[0][7] = ui->comboBox_c13r25->currentData().toInt();
    reagAssign[0][9] = ui->comboBox_c13r27->currentData().toInt();
    reagAssign[0][12] = ui->comboBox_c13r30->currentData().toInt();
    reagAssign[0][13] = ui->comboBox_c13r31->currentData().toInt();
    reagAssign[0][14] = ui->comboBox_c13r32->currentData().toInt();

    //column 14
    reagAssign[1][0] = ui->comboBox_c14r18->currentData().toInt();
    reagAssign[1][1] = ui->comboBox_c14r19->currentData().toInt();
    reagAssign[1][4] = ui->comboBox_c14r22->currentData().toInt();
    reagAssign[1][6] = ui->comboBox_c14r24->currentData().toInt();
    reagAssign[1][8] = ui->comboBox_c14r26->currentData().toInt();
    reagAssign[1][10] = ui->comboBox_c14r28->currentData().toInt();
    reagAssign[1][12] = ui->comboBox_c14r30->currentData().toInt();
    reagAssign[1][13] = ui->comboBox_c14r31->currentData().toInt();
    reagAssign[1][14] = ui->comboBox_c14r32->currentData().toInt();

    //Column 15
    reagAssign[2][0] = ui->comboBox_c15r18->currentData().toInt();
    reagAssign[2][1] = ui->comboBox_c15r19->currentData().toInt();
    reagAssign[2][3] =  ui->comboBox_c15r21->currentData().toInt();
    reagAssign[2][5] = ui->comboBox_c15r23->currentData().toInt();
    reagAssign[2][7] = ui->comboBox_c15r25->currentData().toInt();
    reagAssign[2][9] = ui->comboBox_c15r27->currentData().toInt();
    reagAssign[2][12] = ui->comboBox_c15r30->currentData().toInt();
    reagAssign[2][13] = ui->comboBox_c15r31->currentData().toInt();
    reagAssign[2][14] = ui->comboBox_c15r32->currentData().toInt();

    //Column 16
    reagAssign[3][0] = ui->comboBox_c16r18->currentData().toInt();
    reagAssign[3][1] = ui->comboBox_c16r19->currentData().toInt();
    reagAssign[3][4] = ui->comboBox_c16r22->currentData().toInt();
    reagAssign[3][6] = ui->comboBox_c16r24->currentData().toInt();
    reagAssign[3][8] = ui->comboBox_c16r26->currentData().toInt();
    reagAssign[3][10] = ui->comboBox_c16r28->currentData().toInt();
    reagAssign[3][12] = ui->comboBox_c16r30->currentData().toInt();
    reagAssign[3][13] = ui->comboBox_c16r31->currentData().toInt();
    reagAssign[3][14] = ui->comboBox_c16r32->currentData().toInt();

    //Column 17
    reagAssign[4][0] = ui->comboBox_c17r18->currentData().toInt();
    reagAssign[4][1] = ui->comboBox_c17r19->currentData().toInt();
    reagAssign[4][3] = ui->comboBox_c17r21->currentData().toInt();
    reagAssign[4][5] = ui->comboBox_c17r23->currentData().toInt();
    reagAssign[4][7] = ui->comboBox_c17r25->currentData().toInt();
    reagAssign[4][9] = ui->comboBox_c17r27->currentData().toInt();
    reagAssign[4][12] = ui->comboBox_c17r30->currentData().toInt();
    reagAssign[4][13] = ui->comboBox_c17r31->currentData().toInt();
    reagAssign[4][14] = ui->comboBox_c17r32->currentData().toInt();

    //Column 18
    reagAssign[5][0] = ui->comboBox_c18r18->currentData().toInt();
    reagAssign[5][1] = ui->comboBox_c18r19->currentData().toInt();
    reagAssign[5][4] = ui->comboBox_c18r22->currentData().toInt();
    reagAssign[5][6] = ui->comboBox_c18r24->currentData().toInt();
    reagAssign[5][8] = ui->comboBox_c18r26->currentData().toInt();
    reagAssign[5][10] = ui->comboBox_c18r28->currentData().toInt();
    reagAssign[5][12] = ui->comboBox_c18r30->currentData().toInt();
    reagAssign[5][13] = ui->comboBox_c18r31->currentData().toInt();
    reagAssign[5][14] = ui->comboBox_c18r32->currentData().toInt();

    //Column 19
    reagAssign[6][0] = ui->comboBox_c19r18->currentData().toInt();
    reagAssign[6][1] = ui->comboBox_c19r19->currentData().toInt();
    reagAssign[6][3] = ui->comboBox_c19r21->currentData().toInt();
    reagAssign[6][5] = ui->comboBox_c19r23->currentData().toInt();
    reagAssign[6][7] = ui->comboBox_c19r25->currentData().toInt();
    reagAssign[6][9] = ui->comboBox_c19r27->currentData().toInt();
    reagAssign[6][12] = ui->comboBox_c19r30->currentData().toInt();
    reagAssign[6][13] = ui->comboBox_c19r31->currentData().toInt();
    reagAssign[6][14] = ui->comboBox_c19r32->currentData().toInt();

    //Column 20
    reagAssign[7][1] = ui->comboBox_c20r19->currentData().toInt();

    //SENSOR V3 ##########################################################
    reagAssign_v3[0] = ui->comboBox_v3_1->currentData().toInt();
    reagAssign_v3[1] = ui->comboBox_v3_2->currentData().toInt();
    reagAssign_v3[2] = ui->comboBox_v3_3->currentData().toInt();
    reagAssign_v3[3] = ui->comboBox_v3_4->currentData().toInt();
    reagAssign_v3[4] = ui->comboBox_v3_5->currentData().toInt();
    reagAssign_v3[5] = ui->comboBox_v3_6->currentData().toInt();
    reagAssign_v3[6] = ui->comboBox_v3_7->currentData().toInt();
    reagAssign_v3[7] = ui->comboBox_v3_8->currentData().toInt();

    //SENSOR V4 ##########################################################
    reagAssign_v4[0][0] = ui->comboBox_v4_c1r1->currentData().toInt();
    reagAssign_v4[0][1] = ui->comboBox_v4_c1r2->currentData().toInt();
    reagAssign_v4[0][2] = ui->comboBox_v4_c1r3->currentData().toInt();
    reagAssign_v4[0][3] = ui->comboBox_v4_c1r4->currentData().toInt();
    reagAssign_v4[0][4] = ui->comboBox_v4_c1r5->currentData().toInt();
    reagAssign_v4[0][5] = ui->comboBox_v4_c1r6->currentData().toInt();
    reagAssign_v4[0][6] = ui->comboBox_v4_c1r7->currentData().toInt();
    reagAssign_v4[0][7] = ui->comboBox_v4_c1r8->currentData().toInt();

    reagAssign_v4[1][0] = ui->comboBox_v4_c2r1->currentData().toInt();
    reagAssign_v4[1][1] = ui->comboBox_v4_c2r2->currentData().toInt();
    reagAssign_v4[1][2] = ui->comboBox_v4_c2r3->currentData().toInt();
    reagAssign_v4[1][3] = ui->comboBox_v4_c2r4->currentData().toInt();
    reagAssign_v4[1][4] = ui->comboBox_v4_c2r5->currentData().toInt();
    reagAssign_v4[1][5] = ui->comboBox_v4_c2r6->currentData().toInt();
    reagAssign_v4[1][6] = ui->comboBox_v4_c2r7->currentData().toInt();
    reagAssign_v4[1][7] = ui->comboBox_v4_c2r8->currentData().toInt();

    //HYPERSPECTRAL APERTURES############################################
    reagAssign_hs[0][0] = ui->comboBox_hsa_c1r1->currentData().toInt();
    reagAssign_hs[0][1] = ui->comboBox_hsa_c1r2->currentData().toInt();
    reagAssign_hs[0][2] = ui->comboBox_hsa_c1r3->currentData().toInt();
    reagAssign_hs[0][3] = ui->comboBox_hsa_c1r4->currentData().toInt();

    reagAssign_hs[1][0] = ui->comboBox_hsa_c2r1->currentData().toInt();
    reagAssign_hs[1][1] = ui->comboBox_hsa_c2r2->currentData().toInt();
    reagAssign_hs[1][2] = ui->comboBox_hsa_c2r3->currentData().toInt();
    reagAssign_hs[1][3] = ui->comboBox_hsa_c2r4->currentData().toInt();

    reagAssign_hs[2][0] = ui->comboBox_hsa_c3r1->currentData().toInt();
    reagAssign_hs[2][1] = ui->comboBox_hsa_c3r2->currentData().toInt();
    reagAssign_hs[2][2] = ui->comboBox_hsa_c3r3->currentData().toInt();
    reagAssign_hs[2][3] = ui->comboBox_hsa_c3r4->currentData().toInt();

    reagAssign_hs[3][0] = ui->comboBox_hsa_c4r1->currentData().toInt();
    reagAssign_hs[3][1] = ui->comboBox_hsa_c4r2->currentData().toInt();
    reagAssign_hs[3][2] = ui->comboBox_hsa_c4r3->currentData().toInt();
    reagAssign_hs[3][3] = ui->comboBox_hsa_c4r4->currentData().toInt();

    reagAssign_hs[4][0] = ui->comboBox_hsa_c5r1->currentData().toInt();
    reagAssign_hs[4][1] = ui->comboBox_hsa_c5r2->currentData().toInt();
    reagAssign_hs[4][2] = ui->comboBox_hsa_c5r3->currentData().toInt();
    reagAssign_hs[4][3] = ui->comboBox_hsa_c5r4->currentData().toInt();

    reagAssign_hs[5][0] = ui->comboBox_hsa_c6r1->currentData().toInt();
    reagAssign_hs[5][1] = ui->comboBox_hsa_c6r2->currentData().toInt();
    reagAssign_hs[5][2] = ui->comboBox_hsa_c6r3->currentData().toInt();
    reagAssign_hs[5][3] = ui->comboBox_hsa_c6r4->currentData().toInt();


}

//Given a string and QVar, add a reagent (item) to every valid sensor
void MainWindow::addReagentToSensors(QString name, QVariant v)
{

    //Sensor V1/V2 #############################
    //column 13
    ui->comboBox_c13r18->addItem(name,v);
    ui->comboBox_c13r19->addItem(name,v);
    ui->comboBox_c13r21->addItem(name,v);
    ui->comboBox_c13r23->addItem(name,v);
    ui->comboBox_c13r25->addItem(name,v);
    ui->comboBox_c13r27->addItem(name,v);
    ui->comboBox_c13r30->addItem(name,v);
    ui->comboBox_c13r31->addItem(name,v);
    ui->comboBox_c13r32->addItem(name,v);

    //column 14
    ui->comboBox_c14r18->addItem(name,v);
    ui->comboBox_c14r19->addItem(name,v);
    ui->comboBox_c14r22->addItem(name,v);
    ui->comboBox_c14r24->addItem(name,v);
    ui->comboBox_c14r26->addItem(name,v);
    ui->comboBox_c14r28->addItem(name,v);
    ui->comboBox_c14r30->addItem(name,v);
    ui->comboBox_c14r31->addItem(name,v);
    ui->comboBox_c14r32->addItem(name,v);

    //Column 15
    ui->comboBox_c15r18->addItem(name,v);
    ui->comboBox_c15r19->addItem(name,v);
    ui->comboBox_c15r21->addItem(name,v);
    ui->comboBox_c15r23->addItem(name,v);
    ui->comboBox_c15r25->addItem(name,v);
    ui->comboBox_c15r27->addItem(name,v);
    ui->comboBox_c15r30->addItem(name,v);
    ui->comboBox_c15r31->addItem(name,v);
    ui->comboBox_c15r32->addItem(name,v);

    //Column 16
    ui->comboBox_c16r18->addItem(name,v);
    ui->comboBox_c16r19->addItem(name,v);
    ui->comboBox_c16r22->addItem(name,v);
    ui->comboBox_c16r24->addItem(name,v);
    ui->comboBox_c16r26->addItem(name,v);
    ui->comboBox_c16r28->addItem(name,v);
    ui->comboBox_c16r30->addItem(name,v);
    ui->comboBox_c16r31->addItem(name,v);
    ui->comboBox_c16r32->addItem(name,v);

    //Column 17
    ui->comboBox_c17r18->addItem(name,v);
    ui->comboBox_c17r19->addItem(name,v);
    ui->comboBox_c17r21->addItem(name,v);
    ui->comboBox_c17r23->addItem(name,v);
    ui->comboBox_c17r25->addItem(name,v);
    ui->comboBox_c17r27->addItem(name,v);
    ui->comboBox_c17r30->addItem(name,v);
    ui->comboBox_c17r31->addItem(name,v);
    ui->comboBox_c17r32->addItem(name,v);

    //Column 18
    ui->comboBox_c18r18->addItem(name,v);
    ui->comboBox_c18r19->addItem(name,v);
    ui->comboBox_c18r22->addItem(name,v);
    ui->comboBox_c18r24->addItem(name,v);
    ui->comboBox_c18r26->addItem(name,v);
    ui->comboBox_c18r28->addItem(name,v);
    ui->comboBox_c18r30->addItem(name,v);
    ui->comboBox_c18r31->addItem(name,v);
    ui->comboBox_c18r32->addItem(name,v);

    //Column 19
    ui->comboBox_c19r18->addItem(name,v);
    ui->comboBox_c19r19->addItem(name,v);
    ui->comboBox_c19r21->addItem(name,v);
    ui->comboBox_c19r23->addItem(name,v);
    ui->comboBox_c19r25->addItem(name,v);
    ui->comboBox_c19r27->addItem(name,v);
    ui->comboBox_c19r30->addItem(name,v);
    ui->comboBox_c19r31->addItem(name,v);
    ui->comboBox_c19r32->addItem(name,v);

    //Column 20
    ui->comboBox_c20r19->addItem(name,v);

    //Test Spot
    ui->comboBox_testSpot_1->addItem(name,v);
    ui->comboBox_testSpot_2->addItem(name,v);

    //Sensor V3  #############################
    ui->comboBox_v3_1->addItem(name,v);
    ui->comboBox_v3_2->addItem(name,v);
    ui->comboBox_v3_3->addItem(name,v);
    ui->comboBox_v3_4->addItem(name,v);
    ui->comboBox_v3_5->addItem(name,v);
    ui->comboBox_v3_6->addItem(name,v);
    ui->comboBox_v3_7->addItem(name,v);
    ui->comboBox_v3_8->addItem(name,v);

    //Sensor V4  #############################
    ui->comboBox_v4_c1r1->addItem(name,v);
    ui->comboBox_v4_c1r2->addItem(name,v);
    ui->comboBox_v4_c1r3->addItem(name,v);
    ui->comboBox_v4_c1r4->addItem(name,v);
    ui->comboBox_v4_c1r5->addItem(name,v);
    ui->comboBox_v4_c1r6->addItem(name,v);
    ui->comboBox_v4_c1r7->addItem(name,v);
    ui->comboBox_v4_c1r8->addItem(name,v);

    ui->comboBox_v4_c2r1->addItem(name,v);
    ui->comboBox_v4_c2r2->addItem(name,v);
    ui->comboBox_v4_c2r3->addItem(name,v);
    ui->comboBox_v4_c2r4->addItem(name,v);
    ui->comboBox_v4_c2r5->addItem(name,v);
    ui->comboBox_v4_c2r6->addItem(name,v);
    ui->comboBox_v4_c2r7->addItem(name,v);
    ui->comboBox_v4_c2r8->addItem(name,v);

    //HYPERPECTRAL APERTURES  ###############
    ui->comboBox_hsa_c1r1->addItem(name,v);
    ui->comboBox_hsa_c1r2->addItem(name,v);
    ui->comboBox_hsa_c1r3->addItem(name,v);
    ui->comboBox_hsa_c1r4->addItem(name,v);

    ui->comboBox_hsa_c2r1->addItem(name,v);
    ui->comboBox_hsa_c2r2->addItem(name,v);
    ui->comboBox_hsa_c2r3->addItem(name,v);
    ui->comboBox_hsa_c2r4->addItem(name,v);

    ui->comboBox_hsa_c3r1->addItem(name,v);
    ui->comboBox_hsa_c3r2->addItem(name,v);
    ui->comboBox_hsa_c3r3->addItem(name,v);
    ui->comboBox_hsa_c3r4->addItem(name,v);

    ui->comboBox_hsa_c4r1->addItem(name,v);
    ui->comboBox_hsa_c4r2->addItem(name,v);
    ui->comboBox_hsa_c4r3->addItem(name,v);
    ui->comboBox_hsa_c4r4->addItem(name,v);

    ui->comboBox_hsa_c5r1->addItem(name,v);
    ui->comboBox_hsa_c5r2->addItem(name,v);
    ui->comboBox_hsa_c5r3->addItem(name,v);
    ui->comboBox_hsa_c5r4->addItem(name,v);

    ui->comboBox_hsa_c6r1->addItem(name,v);
    ui->comboBox_hsa_c6r2->addItem(name,v);
    ui->comboBox_hsa_c6r3->addItem(name,v);
    ui->comboBox_hsa_c6r4->addItem(name,v);

}

//User clicks on "start automated spotting"
void MainWindow::on_bttn_StartSpot_clicked()
{
    //Read the sensor map into an array
    setReagAssignments();

    //If no reagents are assigned, place error message and quit this routine
    bool nonzero = false;

    qDebug() << "SENSOR V " << sensor_version;

    if ((sensor_version == 1) || (sensor_version==2))
    {
        for (int i=0;i<8;i++)
            for (int j=0;j<15;j++)
                if (reagAssign[i][j] != -1)
                    nonzero = true;
    }
    else if (sensor_version == 3)
    {
        for (int i=0;i<8;i++)
            if (reagAssign_v3[i] != -1)
                nonzero = true;
    }
    else if (sensor_version == 4)
    {
        for (int i=0;i<2;i++)
            for (int j=0; j<8;j++)
                if (reagAssign_v4[i][j] != -1)
                    nonzero = true;
    }
    else if (sensor_version == 5)
    {
        qDebug() << "HS START";
        for (int i=0;i<6;i++)
            for (int j=0; j<4;j++)
                if (reagAssign_hs[i][j] != -1)
                    nonzero = true;
    }

    if (nonzero == false)
    {
        QMessageBox::warning(this,"Error","No reagents were assigned to sensor coordinates");
    }
    //If at least one sensor is assigned a reagent
    else
    {
        //Transmit the sensor coordinates
        emit txSensorVersion(sensor_version);

        //transmit the corner coordinates of the sensor, the dispense mode, as well as the z-position
        emit txStartCoordinates(ui->spin_topLeftX->value(),ui->spin_topLeftY->value(),ui->spin_bottomRightX->value(),ui->spin_bottomRightY->value(),ui->spin_Z->value());

        //Transmit reagent assignments and other configurations
        spotCtrl->rxReagAssignments(reagAssign,reagAssign_v3,reagAssign_v4,reagAssign_hs);
        spotCtrl->rxTestSpots(ui->comboBox_testSpot_1->currentData().toInt(),ui->comboBox_testSpot_2->currentData().toInt());
        spotCtrl->rxScriptLoopCnt(ui->spin_ScriptLoopCount->value());
        spotCtrl->rxSpotConfigV3(ui->spin_horGrid_v3->value(),ui->spin_vertGrid_v3->value(),ui->spin_overshootV3->value());
        spotCtrl->rxSpotConfigV4(ui->spin_horGrid_v4->value(),ui->spin_vertGrid_v4->value(),ui->spin_overshootV4->value());

        //Start Spotting
        emit startSpot();

        //Disable UI Controls that would interfere with automated spotting
        ui->bttn_MoveAbs->setEnabled(false);
        ui->bttn_StartSpot->setEnabled(false);
        ui->combo_SensorVersion->setEnabled(false);
        ui->bttn_autoPopulate->setEnabled(false);
    }
}

//User specifies top left coordinate
void MainWindow::on_bttn_SetTopLeft_clicked()
{
    //Pull from GUI, if this is innaccurate read from stage
    ui->spin_topLeftX->setValue(ui->spin_Xpos->value());
    ui->spin_topLeftY->setValue(ui->spin_Ypos->value());

    topLeftClicked = true;

    //If all test parameters have been set, enable start button
    if (topLeftClicked && bottomRightClicked && zClicked)
        ui->bttn_StartSpot->setEnabled(true);

    //Update Test Coord
    ui->lbl_testCoordStatus->setText(QString::number(ui->spin_topLeftX->value()) + ", " + QString::number(ui->spin_topLeftY->value()));
}

//User specifies bottom right coordinate
void MainWindow::on_bttn_SetBottomRight_clicked()
{
    //Pull from GUI, if this is innaccurate read from stage
    ui->spin_bottomRightX->setValue(ui->spin_Xpos->value());
    ui->spin_bottomRightY->setValue(ui->spin_Ypos->value());

    bottomRightClicked = true;

    //If all test parameters have been set, enable start button
    if (topLeftClicked && bottomRightClicked && zClicked)
        ui->bttn_StartSpot->setEnabled(true);
}

//User specifies Z position
void MainWindow::on_bttn_SetZ_clicked()
{
    //Pull from GUI, if this is innaccurate read from stage
    ui->spin_Z->setValue(ui->spin_Zpos->value());

    zClicked = true;

    //If all test parameters have been set, enable start button
    if (topLeftClicked && bottomRightClicked && zClicked)
        ui->bttn_StartSpot->setEnabled(true);

}

//Robot control class transmits system status
void MainWindow::rxRobotStatus(bool moving)
{
    if (moving == false)
        emit txDoneMoving();
}

//Error encountered in robot movement
void MainWindow::rxRobotError()
{
    emit txRobotError();

    QMessageBox msgBox;
    msgBox.setText("Robot Error");
    msgBox.setInformativeText("Motion control subsystem has encountered an error. Automated sequences have been aborted.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

//Receive progress from spotting automation class, update GUI
void MainWindow::rxProgress(int progress)
{
    QString str = QString::number(progress);
    str.append(" % Complete");

    ui->lbl_AutoStatus->setText(str);

    ui->progressBar->setValue(progress);
}

//Receive complete signal from spotting automation class
void MainWindow::rxCompleteSignal()
{
    //Enable GUi
    ui->bttn_StartSpot->setEnabled(true);

    ui->bttn_MoveAbs->setEnabled(true);

    ui->lbl_AutoStatus->setText("Ready");

    ui->combo_SensorVersion->setEnabled(true);
    ui->bttn_autoPopulate->setEnabled(true);
}

//User has selected to save the current configuration (reagent list, reagent assignments, sensor coordinates)
void MainWindow::on_actionSave_Configuration_triggered()
{

    //Get file name from dialog and attempt to open the file
    QString fname = QFileDialog::getSaveFileName(this,"Specify configuration save file","/","Configuration files (*.cfg)");
    QFile file(fname);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"Error","Unable to open file.");
        return;
    }

    //Set reagent assignments so local variable is filled with GUI data
    setReagAssignments();

    //Write Title
    file.write("SPOTBOT_CONFIG_FILE\n\r");
    char buf[6];

    //Write sensor coordinates
    file.write(itoa(ui->spin_topLeftX->value(),buf,10));
    file.write(",");
    file.write(itoa(ui->spin_topLeftY->value(),buf,10));
    file.write("\n\r");
    file.write(itoa(ui->spin_bottomRightX->value(),buf,10));
    file.write(",");
    file.write(itoa(ui->spin_bottomRightY->value(),buf,10));
    file.write("\n\r");
    file.write(itoa(ui->spin_Z->value(),buf,10));
    file.write("\n\r");

    //Write reagent list
    for (int i =0;i<20;i++)
    {
        file.write(reagList[i].toUtf8());
        file.write("\n\r");
    }

    //Write reagent assignments for V1/V2
    for (int i=0;i<8;i++)
    {
        for (int j=0;j<15;j++)
        {
            file.write(itoa(reagAssign[i][j],buf,10));
            if (j <14)
                file.write(",");
        }
        file.write("\n\r");
    }

    //Write test spots
    file.write(itoa(ui->comboBox_testSpot_1->currentData().toInt(),buf,10));
    file.write("\n\r");
    file.write(itoa(ui->comboBox_testSpot_2->currentData().toInt(),buf,10));
    file.write("\n\r");

    //Write reagent assignments for V3
    for (int i=0;i<8;i++)
    {
        file.write(itoa(reagAssign_v3[i],buf,10));
        if (i <7)
            file.write(",");
    }
    file.write("\n\r");

    //Write spot configuration for V3
    file.write(itoa(ui->spin_horGrid_v3->value(),buf,10));
    file.write(",");
    file.write(itoa(ui->spin_vertGrid_v3->value(),buf,10));
    file.write(",");
    file.write(itoa(ui->spin_overshootV3->value(),buf,10));
    file.write("\n\r");


    //Write reagent assignments for V4
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<8;j++)
        {
            file.write(itoa(reagAssign_v4[i][j],buf,10));
            if (j <7)
                file.write(",");
        }
        file.write("\n\r");
    }

    //Write spot configuration for V4
    file.write(itoa(ui->spin_horGrid_v4->value(),buf,10));
    file.write(",");
    file.write(itoa(ui->spin_vertGrid_v4->value(),buf,10));
    file.write(",");
    file.write(itoa(ui->spin_overshootV4->value(),buf,10));
    file.write("\n\r");

    //Write reagent assignments for hyperspectral apertures
    for (int i=0;i<6;i++)
    {
        for (int j=0;j<4;j++)
        {
            file.write(itoa(reagAssign_hs[i][j],buf,10));
            if (j <3)
                file.write(",");
        }
        file.write("\n\r");
    }

    //Write current sensor version
    file.write(itoa(sensor_version,buf,10));
    file.write("\n\r");

    //File close
    file.close();


}

//User has selected to load a previous configuration (reagent list, reagent assignments, sensor coordinates)
void MainWindow::on_actionLoad_Configuration_triggered()
{
    //Get file name from dialog and attempt to open the file
    QString fname = QFileDialog::getOpenFileName(this,"Specify configuration file","/","Configuration files (*.cfg)");

    QFile file(fname);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,"Error","Unable to open file.");
        return;
    }

    QTextStream in(&file);

    //PARSE FILE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    QString line = in.readLine();   //First line, header

    //Make sure file is spotbot config
    if (0 != QString::compare(line,"SPOTBOT_CONFIG_FILE"))
    {
        QMessageBox::warning(this,"Error","This file is not a Spotbot configuration file.");
        return;
    }

    //Second line topleftx,toplefty
    line = in.readLine();
    QStringList list = line.split(",");
    ui->spin_topLeftX->setValue(list[0].toInt());
    ui->spin_topLeftY->setValue(list[1].toInt());

    ui->lbl_testCoordStatus->setText(QString::number(ui->spin_topLeftX->value()) + ", " + QString::number(ui->spin_topLeftY->value()));
\
    //Third line bottomrightx,bottomrighty
    line = in.readLine();
    list = line.split(",");
    ui->spin_bottomRightX->setValue(list[0].toInt());
    ui->spin_bottomRightY->setValue(list[1].toInt());

    //Fourth line Z
    line = in.readLine();
    ui->spin_Z->setValue(line.toInt());

    //Next 20 lines, reagent list
    QString tempReagList[20];

    for(int i = 0;i<20; i++)
    {
        line = in.readLine();
        tempReagList[i] = line;
    }

    reagSet->setReagents(tempReagList);

    for (int i = 0;i<20;i++)
        reagList[i] = tempReagList[i];

    updateReagAssignments();

    //Next 8 lines, sensor V1/V2 reagent assignments
    for(int i = 0;i<8; i++)
    {
        line = in.readLine();
        QStringList list = line.split(",");

        for (int j=0;j<15;j++)
        {
            reagAssign[i][j] = list[j].toInt();
        }
    }

    //Test spots
    line = in.readLine();
    int testOne = line.toInt();

    line = in.readLine();
    int testTwo = line.toInt();

    //Skip Reading V3/V4 if at end of file (assume this is an older software config)
    if (in.atEnd() == false)
    {

        //Next line, sensor V3 reagent assignments
        line = in.readLine();
        list = line.split(",");

        for (int i=0;i<8;i++)
            reagAssign_v3[i] = list[i].toInt();

        //Next line, sensor V3 spot configuration
        line = in.readLine();
        list = line.split(",");
        ui->spin_horGrid_v3->setValue(list[0].toInt());
        ui->spin_vertGrid_v3->setValue(list[1].toInt());
        ui->spin_overshootV3->setValue(list[0].toInt());

        //Next 2 lines, sensor V4 reagent assignments
        for(int i = 0;i<2; i++)
        {
            line = in.readLine();
            list = line.split(",");

            for (int j=0;j<8;j++)
            {
                reagAssign_v4[i][j] = list[j].toInt();
            }
        }

        //Next line, sensor V4 spot configuration
        line = in.readLine();
        list = line.split(",");
        ui->spin_horGrid_v4->setValue(list[0].toInt());
        ui->spin_vertGrid_v4->setValue(list[1].toInt());
        ui->spin_overshootV4->setValue(list[0].toInt());

        //Read next line, Hyperspectral aperture spot configuration (or end, depending on config version)
        line = in.readLine();
        list = line.split(",");

        if (list.size() == 1)
        {
            QMessageBox::warning(this,"Warning","This configuration file is shorter than expected. Outdated configuration file assumed, hyperspectral aperture parameters will not be loaded.");
            sensor_version = list[0].toInt();
        }
        else
        {
            //Data has more than one token, assumed to be correct formatting for hyperspectral apertures
            for (int j=0;j<4;j++)
            {
                reagAssign_hs[0][j] = list[j].toInt();
            }

            //Next 3 lines, hyperspectral aperture reagent assignments
            for(int i = 0;i<5; i++)
            {
                line = in.readLine();
                list = line.split(",");

                for (int j=0;j<4;j++)
                {
                    reagAssign_hs[i+1][j] = list[j].toInt();
                }
            }

            //Sensor Version
            line = in.readLine();
            sensor_version = line.toInt();
        }

    }
    else
    {
        QMessageBox::warning(this,"Warning","This configuration file is shorter than expected. Outdated configuration file assumed, sensor V3/V4 parameters will not be loaded.");
    }



    file.close();

    //Update Sensor map
    loadReagAssignments();
    updateSpotGraphics();

    //Update screen for sensor version
    ui->combo_SensorVersion->setCurrentIndex(sensor_version-1);
    this->on_combo_SensorVersion_currentIndexChanged(sensor_version-1);

    //Update test spot boxes
    int i = 0;
    while(testOne != ui->comboBox_testSpot_1->itemData(i).toInt())
        i= i+1;
    ui->comboBox_testSpot_1->setCurrentIndex(i);

    i = 0;
    while(testTwo != ui->comboBox_testSpot_2->itemData(i).toInt())
        i= i+1;
    ui->comboBox_testSpot_2->setCurrentIndex(i);

    //Set these vars as true, since these fields are filled by the config
    topLeftClicked = true;
    bottomRightClicked = true;
    zClicked = true;
    if ( robotConnected)
        ui->bttn_StartSpot->setEnabled(true);

}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionHelp_triggered()
{
    dlgHelp *dlg = new dlgHelp();
    dlg->show();
}

void MainWindow::on_bttn_QuickDatumOne_clicked()
{
    if (sensor_version != 5)
    {
        ui->spin_Xpos->setValue(82550);
        ui->spin_Ypos->setValue(30150);
        ui->spin_Zpos->setValue(94000);
    }
    else
    {
        ui->spin_Xpos->setValue(55800);
        ui->spin_Ypos->setValue(26200);
        ui->spin_Zpos->setValue(83000);
    }
}

void MainWindow::on_bttn_QuickDatumTwo_clicked()
{
    ui->spin_Xpos->setValue(82550);
    ui->spin_Ypos->setValue(13350);
    ui->spin_Zpos->setValue(94000);
}

void MainWindow::on_bttn_autoPopulate_clicked()
{
    //User clicks auto-populate. Automatically populate all sensor coordinates with iterating reagents

    if ((sensor_version >2) && (sensor_version != 5))
    {
        QMessageBox::warning(this,"Warning","This feature has only been implemented for sensor V1/V2 and the hyperspectral apertures. No action taken.");
        return;
    }

    if (sensor_version < 2)
        {
        //Get max number of items
        int max = ui->comboBox_c13r18->count();
        int count = 1; //0 is "None", so never use 1

        //column 13
        ui->comboBox_c13r18->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c13r19->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c13r21->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c13r23->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c13r25->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c13r27->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c13r30->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c13r31->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c13r32->setCurrentIndex(count);
        count = incrementReagCount(count, max);

        //column 14
        ui->comboBox_c14r18->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c14r19->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c14r22->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c14r24->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c14r26->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c14r28->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c14r30->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c14r31->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c14r32->setCurrentIndex(count);
        count = incrementReagCount(count, max);

        //Column 15
        ui->comboBox_c15r18->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c15r19->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c15r21->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c15r23->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c15r25->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c15r27->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c15r30->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c15r31->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c15r32->setCurrentIndex(count);
        count = incrementReagCount(count, max);

        //Column 16
        ui->comboBox_c16r18->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c16r19->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c16r22->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c16r24->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c16r26->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c16r28->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c16r30->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c16r31->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c16r32->setCurrentIndex(count);
        count = incrementReagCount(count, max);

        //Column 17
        ui->comboBox_c17r18->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c17r19->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c17r21->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c17r23->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c17r25->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c17r27->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c17r30->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c17r31->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c17r32->setCurrentIndex(count);
        count = incrementReagCount(count, max);

        //Column 18
        ui->comboBox_c18r18->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c18r19->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c18r22->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c18r24->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c18r26->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c18r28->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c18r30->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c18r31->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c18r32->setCurrentIndex(count);
        count = incrementReagCount(count, max);

        //Column 19
        ui->comboBox_c19r18->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c19r19->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c19r21->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c19r23->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c19r25->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c19r27->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c19r30->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c19r31->setCurrentIndex(count);
        count = incrementReagCount(count, max);
        ui->comboBox_c19r32->setCurrentIndex(count);
        count = incrementReagCount(count, max);

        //Column 20
        ui->comboBox_c20r19->setCurrentIndex(count);
        count = incrementReagCount(count, max);
    }
    else if (sensor_version == 5)
    {
        // For the hyperspectral apertures, simply assign all fields equal to r1c1
        int val = ui->comboBox_hsa_c1r1->currentIndex();

        ui->comboBox_hsa_c1r1->setCurrentIndex(val);
        ui->comboBox_hsa_c1r2->setCurrentIndex(val);
        ui->comboBox_hsa_c1r3->setCurrentIndex(val);
        ui->comboBox_hsa_c1r4->setCurrentIndex(val);

        ui->comboBox_hsa_c2r1->setCurrentIndex(val);
        ui->comboBox_hsa_c2r2->setCurrentIndex(val);
        ui->comboBox_hsa_c2r3->setCurrentIndex(val);
        ui->comboBox_hsa_c2r4->setCurrentIndex(val);

        ui->comboBox_hsa_c3r1->setCurrentIndex(val);
        ui->comboBox_hsa_c3r2->setCurrentIndex(val);
        ui->comboBox_hsa_c3r3->setCurrentIndex(val);
        ui->comboBox_hsa_c3r4->setCurrentIndex(val);

        ui->comboBox_hsa_c4r1->setCurrentIndex(val);
        ui->comboBox_hsa_c4r2->setCurrentIndex(val);
        ui->comboBox_hsa_c4r3->setCurrentIndex(val);
        ui->comboBox_hsa_c4r4->setCurrentIndex(val);

        ui->comboBox_hsa_c5r1->setCurrentIndex(val);
        ui->comboBox_hsa_c5r2->setCurrentIndex(val);
        ui->comboBox_hsa_c5r3->setCurrentIndex(val);
        ui->comboBox_hsa_c5r4->setCurrentIndex(val);

        ui->comboBox_hsa_c6r1->setCurrentIndex(val);
        ui->comboBox_hsa_c6r2->setCurrentIndex(val);
        ui->comboBox_hsa_c6r3->setCurrentIndex(val);
        ui->comboBox_hsa_c6r4->setCurrentIndex(val);
    }
}

//User wants to move to test coordinates
void MainWindow::on_bttnTestCoord_clicked()
{
    updateManualCoords = true;
    //emit moveToCoord(ui->spin_topLeftX->value(), ui->spin_topLeftY->value(), ui->spin_TestCoordColumn->value(), ui->spin_TestCoordRow->value());
    emit moveToCoord(ui->spin_topLeftX->value(), ui->spin_topLeftY->value(), ui->spin_TestCoordColumn->value()-13, ui->spin_TestCoordRow->value()-18);
}


void MainWindow::on_spin_vertGrid_v3_valueChanged(int arg1)
{
    updateSpotGraphics();
}

void MainWindow::on_spin_horGrid_v3_valueChanged(int arg1)
{
    updateSpotGraphics();
}

void MainWindow::on_spin_vertGrid_v4_valueChanged(int arg1)
{
    updateSpotGraphics();
}

void MainWindow::on_spin_horGrid_v4_valueChanged(int arg1)
{
    updateSpotGraphics();
}

void MainWindow::on_spin_overshootV3_valueChanged(int arg1)
{
    updateSpotGraphics();
}

void MainWindow::on_spin_overshootV4_valueChanged(int arg1)
{
    updateSpotGraphics();
}

void MainWindow::on_spin_v4_spotSize_valueChanged(int arg1)
{
    updateSpotGraphics();
}

void MainWindow::on_spin_v3_spotSize_valueChanged(int arg1)
{
    updateSpotGraphics();
}

void MainWindow::on_bttn_clearError_clicked()
{

}

void MainWindow::on_combo_SensorVersion_currentIndexChanged(int index)
{
    if ((index ==0)||(index==1))
        ui->stackedLayout->setCurrentIndex(0);
    else if (index==2)
        ui->stackedLayout->setCurrentIndex(1);
    else if (index==3)
        ui->stackedLayout->setCurrentIndex(2);
    else if (index==4)
        ui->stackedLayout->setCurrentIndex(3);

    sensor_version = ui->combo_SensorVersion->currentIndex() + 1;

}

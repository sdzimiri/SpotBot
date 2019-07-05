#include "dlghardwaresettings.h"
#include "ui_dlghardwaresettings.h"
#include <QSerialPortInfo>
#include <QThread>
#include <QMessageBox>
#include <QDebug>

//Default Constructor
dlgHardwareSettings::dlgHardwareSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgHardwareSettings)
{

    ui->setupUi(this);


    //Search for existing com ports
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();

    QStringList portList;
    for (int i =0; i < infoList.size();i++)
    {
        QSerialPortInfo p = infoList[i];
        portList << p.portName();
    }

    //Populate fields with ports
    ui->comboBox_RobotPort->addItems(portList);

    //Initialize member vars
    roboCon = false;
    roboInd = -1;
}

dlgHardwareSettings::~dlgHardwareSettings()
{
    delete ui;
}

//Called to update GUI based upon connection status
void dlgHardwareSettings::updateGui()
{
    //If the robot is connected
    if (roboCon)
    {
        ui->comboBox_RobotPort->setCurrentIndex(roboInd);
        ui->label_RobotStatus->setText("Connected");
        ui->bttn_ConnectRobot->setEnabled(false);
        ui->comboBox_RobotPort->setEnabled(false);
    }
}

void dlgHardwareSettings::on_bttn_Close_clicked()
{
    this->close();
}


//User clicks on Connect to 3-Axis Motion Control Stage
void dlgHardwareSettings::on_bttn_ConnectRobot_clicked()
{
    //Get port name from combobox
    QString portName = ui->comboBox_RobotPort->currentText();
    emit txRobotPortName(portName);
    roboInd = ui->comboBox_RobotPort->currentIndex();

    ui->label_RobotStatus->setText("Connecting...");
    ui->bttn_ConnectRobot->setEnabled(false);

}

//Slot for receiving connection status of robot
void dlgHardwareSettings::rxRobotConnected(bool connected)
{
    if (connected)
    {
        ui->label_RobotStatus->setText("Connected");
        ui->bttn_ConnectRobot->setEnabled(false);
        ui->comboBox_RobotPort->setEnabled(false);
        roboCon = true;
    }
    else
    {
        ui->label_RobotStatus->setText("Disconnected");
        ui->bttn_ConnectRobot->setEnabled(true);
        roboCon = false;
    }
}


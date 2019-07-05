#include "robotcontrol.h"
#include <QtSerialPort/QSerialPort>
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <QTimer>
#include <windows.h>


//Default Constructor
RobotControl::RobotControl() : QObject()
{
    //Initialize serial port()
    serial = new QSerialPort();
    connect(serial, SIGNAL(readyRead()), this, SLOT(ReadData()));
    state = disconnected;

    //Initialize Query Timer
    queryTimer = new QTimer(this);
    connect(queryTimer,SIGNAL(timeout()),this,SLOT(getStatus()));

    //Initialize command timer
    commandTimer = new QTimer(this);
    connect(commandTimer,SIGNAL(timeout()),this,SLOT(commandTimeOut()));

    commandTime = false;
}

//Slot: If user attempts to start connection
void RobotControl::ConnectPort(QString port)
{
    state = disconnected;

    //Configure port for IAI 3-axis stage
    serial->setPortName(port);
    serial->setBaudRate(QSerialPort::Baud38400);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite))
    {
        //Send ACK
        addCheckSumAndWrite("!99200AreYouHere");

        state = waiting;

        //Two second timeout
        QTimer::singleShot(2000,this,SLOT(timeOut()));

    }
    else
    {
       //If port cannot be opened, emit failed signal
       QMessageBox::critical(0,"Serial Error",serial->errorString());
       emit PortConnected(false);
       serial->close();

       state = disconnected;
    }

}

//Slot for moving robot
void RobotControl::moveAbsolute(int x, int y, int z)
{

    //Convert coordinates to 8-byte hex strings
    QString xstr = num2HexStr(x);
    QString ystr = num2HexStr(y);
    QString zstr = num2HexStr(z);

    //Build command string
    QString mssg = "!9923407001E001E0018";
    mssg.append(xstr);
    mssg.append(ystr);
    mssg.append(zstr);
    addCheckSumAndWrite(mssg);

}

//Reset alarm state
void RobotControl::alarmReset()
{
    //Build command string
    QString mssg = "!99252";
    addCheckSumAndWrite(mssg);
}

//Takes message, adds checksum & CRLF, and writes to serial port
void RobotControl::addCheckSumAndWrite(QString str)
{
    int sum = 0;

    for(int i=0;i<str.size();i++)
        sum += str.at(i).unicode();

    QString sumStr = num2HexStr(sum);
    QString checkSum = "";
    checkSum.append(sumStr.at(sumStr.size()-2));
    checkSum.append(sumStr.at(sumStr.size()-1));
    checkSum.append("\n\r");
    str.append(checkSum);

    QByteArray ba = str.toUtf8();
    const char *buff = ba.data();

    if (commandTime == false)
    {
        serial->write(buff);
        serial->flush();
        commandTime = true;

        //Two second timeout
        commandTimer->singleShot(250,this,SLOT(commandTimeOut()));
    }
    else
    {
        while(commandTime == false)
        {

        }
        serial->write(buff);
        serial->flush();
        commandTime = true;
    }
}

//Helper Function
QString RobotControl::num2HexStr(int num)
{
    QString numstr = QString::number(num,16);

    QString retStr = "";
    QByteArray byte;
    byte.resize(1);
    byte[0] = '0';

    for (int i = 0;i<8-numstr.length();i++)
        retStr = retStr.append(QString(byte));

    retStr.append(numstr);
    return retStr;
}

//If connection times out
void RobotControl::timeOut()
{
    //If waiting for ACK, report unsuccessful connection
    if (state == waiting)
    {
        state = disconnected;
        emit PortConnected(false);
        serial->close();
    }
}

//Slot for receiving data from the port
void RobotControl::ReadData()
{
    QByteArray rxMssg = serial->readAll();
    QString rxString(rxMssg);

    //If waiting for connection, use the response to determine the ACK validity
    if (state == waiting)
    {

        if (QString::compare(rxString,"#99200AreYouHere00\r\n") == 0)
        {
            //If the ACK is successful, tell application that connection was successful
            state = connected;
            emit PortConnected(true);

            //Turn on servos
            addCheckSumAndWrite("!99232071");

            Sleep(1000);

            //Home
            addCheckSumAndWrite("!9923307000000000000");

            queryTimer->start(250);
            //queryTimer->stop();

        }
        else
        {
            //If ACK failed, tell application
            state = disconnected;
            emit PortConnected(false);
            serial->close();
        }

    }
    //If waiting for robot status
    else if (state == waitforstat)
    {
        //If moving
        if (rxString.contains("#99212070D"))
            emit txStatus(true);
        else
            emit txStatus(false);

        state = connected;

    }

    if (QString::compare(rxString.at(0),"&") == 0)
    {
        //Error string, print to debug output
        QString error = "Error: ";
        error.append(rxString.at(3));
        error.append(rxString.at(4));
        error.append(rxString.at(5));
        qDebug() << error;

        //Kill process
        emit errorGenerated();
    }

}

//Slot for requesting the robot status
void RobotControl::getStatus()
{
    state = waitforstat;
    //Request Status of system
    addCheckSumAndWrite("!9921207");

}

void RobotControl::commandTimeOut()
{
    commandTime = false;
}

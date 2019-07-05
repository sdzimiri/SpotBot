#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTimer>


class RobotControl : public QObject
{
    Q_OBJECT

public:
    RobotControl();

signals:
    void PortConnected(bool b);
    void CurrentPos(int x, int y, int z);
    void txStatus(bool moving);
    void errorGenerated();

public slots:
    void ConnectPort(QString port);
    void moveAbsolute(int x, int y, int z);
    void alarmReset();
    void getStatus();
private slots:
    void ReadData();
    void timeOut();
    void commandTimeOut();
private:
    QTimer *queryTimer;
    QTimer *commandTimer;
    QString num2HexStr(int num);
    QSerialPort *serial;
    void addCheckSumAndWrite(QString str);
    int state;  //0 = disconnected
                //1 = waiting for ack
                //2 = connected
                //3 = waiting for robot status
    enum state {disconnected, waiting, connected, waitforstat};
    bool commandTime;
};

#endif // ROBOTCONTROL_H

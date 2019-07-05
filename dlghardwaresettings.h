#ifndef DLGHARDWARESETTINGS_H
#define DLGHARDWARESETTINGS_H

#include <QDialog>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class dlgHardwareSettings;
}

class dlgHardwareSettings : public QDialog
{
    Q_OBJECT

public:
    explicit dlgHardwareSettings(QWidget *parent = 0);
    void updateGui();
    ~dlgHardwareSettings();

signals:
    void txRobotPortName(QString str);

public slots:
    void rxRobotConnected(bool connected);

private slots:
    void on_bttn_Close_clicked();
    void on_bttn_ConnectRobot_clicked();

private:
    Ui::dlgHardwareSettings *ui;
    QSerialPortInfo *info;

    bool roboCon;
    int roboInd;


};

#endif // DLGHARDWARESETTINGS_H

/********************************************************************************
** Form generated from reading UI file 'dlghardwaresettings.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGHARDWARESETTINGS_H
#define UI_DLGHARDWARESETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dlgHardwareSettings
{
public:
    QPushButton *bttn_Close;
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBox_RobotPort;
    QPushButton *bttn_ConnectRobot;
    QLabel *label_RobotStatus;

    void setupUi(QDialog *dlgHardwareSettings)
    {
        if (dlgHardwareSettings->objectName().isEmpty())
            dlgHardwareSettings->setObjectName(QStringLiteral("dlgHardwareSettings"));
        dlgHardwareSettings->resize(317, 118);
        bttn_Close = new QPushButton(dlgHardwareSettings);
        bttn_Close->setObjectName(QStringLiteral("bttn_Close"));
        bttn_Close->setGeometry(QRect(220, 80, 75, 23));
        groupBox = new QGroupBox(dlgHardwareSettings);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 10, 281, 61));
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 20, 261, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        comboBox_RobotPort = new QComboBox(horizontalLayoutWidget);
        comboBox_RobotPort->setObjectName(QStringLiteral("comboBox_RobotPort"));

        horizontalLayout->addWidget(comboBox_RobotPort);

        bttn_ConnectRobot = new QPushButton(horizontalLayoutWidget);
        bttn_ConnectRobot->setObjectName(QStringLiteral("bttn_ConnectRobot"));

        horizontalLayout->addWidget(bttn_ConnectRobot);

        label_RobotStatus = new QLabel(horizontalLayoutWidget);
        label_RobotStatus->setObjectName(QStringLiteral("label_RobotStatus"));
        label_RobotStatus->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_RobotStatus);


        retranslateUi(dlgHardwareSettings);

        QMetaObject::connectSlotsByName(dlgHardwareSettings);
    } // setupUi

    void retranslateUi(QDialog *dlgHardwareSettings)
    {
        dlgHardwareSettings->setWindowTitle(QApplication::translate("dlgHardwareSettings", "Hardware Settings", 0));
        bttn_Close->setText(QApplication::translate("dlgHardwareSettings", "Close", 0));
        groupBox->setTitle(QApplication::translate("dlgHardwareSettings", "IAI 3-Axis Stage", 0));
        bttn_ConnectRobot->setText(QApplication::translate("dlgHardwareSettings", "Connect", 0));
        label_RobotStatus->setText(QApplication::translate("dlgHardwareSettings", "Disconnected", 0));
    } // retranslateUi

};

namespace Ui {
    class dlgHardwareSettings: public Ui_dlgHardwareSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGHARDWARESETTINGS_H

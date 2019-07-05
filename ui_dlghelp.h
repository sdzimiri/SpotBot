/********************************************************************************
** Form generated from reading UI file 'dlghelp.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGHELP_H
#define UI_DLGHELP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_dlgHelp
{
public:
    QLabel *label;
    QPushButton *bttn_Close;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *dlgHelp)
    {
        if (dlgHelp->objectName().isEmpty())
            dlgHelp->setObjectName(QStringLiteral("dlgHelp"));
        dlgHelp->resize(798, 329);
        label = new QLabel(dlgHelp);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 310, 299));
        label->setMinimumSize(QSize(310, 299));
        label->setPixmap(QPixmap(QString::fromUtf8("Resources/datum.png")));
        bttn_Close = new QPushButton(dlgHelp);
        bttn_Close->setObjectName(QStringLiteral("bttn_Close"));
        bttn_Close->setGeometry(QRect(710, 290, 75, 23));
        label_2 = new QLabel(dlgHelp);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(340, 10, 191, 16));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);
        label_3 = new QLabel(dlgHelp);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(360, 70, 291, 161));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_3->setWordWrap(true);

        retranslateUi(dlgHelp);

        QMetaObject::connectSlotsByName(dlgHelp);
    } // setupUi

    void retranslateUi(QDialog *dlgHelp)
    {
        dlgHelp->setWindowTitle(QApplication::translate("dlgHelp", "Dialog", 0));
        label->setText(QString());
        bttn_Close->setText(QApplication::translate("dlgHelp", "Close", 0));
        label_2->setText(QApplication::translate("dlgHelp", "Instructions", 0));
        label_3->setText(QApplication::translate("dlgHelp", "How to use this Software:", 0));
    } // retranslateUi

};

namespace Ui {
    class dlgHelp: public Ui_dlgHelp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGHELP_H

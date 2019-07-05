#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T10:37:52
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spotbot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dlghardwaresettings.cpp \
    robotcontrol.cpp \
    dlgabout.cpp \
    dlgreagents.cpp \
    spotcontrol.cpp \
    dlghelp.cpp

HEADERS  += mainwindow.h \
    dlghardwaresettings.h \
    robotcontrol.h \
    dlgabout.h \
    dlgreagents.h \
    spotcontrol.h \
    dlghelp.h

FORMS    += mainwindow.ui \
    dlghardwaresettings.ui \
    dlgabout.ui \
    dlgreagents.ui \
    dlghelp.ui

DISTFILES += \
    Resources/datum.png

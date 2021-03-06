#-------------------------------------------------
#
# Project created by QtCreator 2018-05-17T15:43:04
#
#-------------------------------------------------

QT       += core gui
QT       += serialport   #  添加串口模块
QT       += charts       #  添加图表模块

# Qt4项目迁移到Qt5问题 必须添加 因为Qt5里不再用QtGui模块，而是使用QtWidgets模块。
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPortTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

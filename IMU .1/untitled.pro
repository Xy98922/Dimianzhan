#-------------------------------------------------
#
# Project created by QtCreator 2018-12-24T15:03:43
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += gamepad
QT       += charts
QT       += network
QT       += multimedia
QT       += axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = X-Controller
TEMPLATE = app


SOURCES += main.cpp\
        Handle.cpp \
        SwitchControl.cpp \
        widget.cpp

HEADERS  += widget.h \
    Handle.h \
    SwitchControl.h \
    baiduvoice.h

FORMS    += widget.ui

RC_FILE = logo.rc

RESOURCES += \
    cc.qrc

LIBS +=-LC:\Users\X\Downloads\my_serial_port\my_serial_port\untitled\libs -lHCCore -lHCNetSDK -lPlayCtrl
LIBS +=-LC:\Users\X\Downloads\my_serial_port\my_serial_port\untitled\libs\HCNetSDKCom -lHCAlarm -lHCGeneralCfgMgr -lHCPreview

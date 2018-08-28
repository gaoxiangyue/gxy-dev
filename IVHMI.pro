#-------------------------------------------------
#
# Project created by QtCreator 2018-02-04T14:24:28
#
#-------------------------------------------------

QT       += core gui
QT       += widgets serialport
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IVHMI
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mybutton.cpp \
    vehicle.cpp \
    traffic.cpp \
    control.cpp \
    myswitch.cpp \
    app.cpp \
    setting.cpp \
    masterthread.cpp \
    Astar/Astar.cpp \
    DataStruct/datastruct.cpp \
    ImPortMap/xmlparser.cpp \
    mapwidget.cpp \
    udprxtx.cpp \
    myippartlineedit.cpp \
    myipaddredit.cpp

HEADERS  += widget.h \
    mybutton.h \
    vehicle.h \
    traffic.h \
    control.h \
    myswitch.h \
    app.h \
    setting.h \
    masterthread.h \
    Astar/Astar.h \
    DataStruct/datastruct.h \
    ImPortMap/xmlparser.h \
    mapwidget.h \
    udprxtx.h \
    myippartlineedit.h \
    myipaddredit.h

FORMS    +=

RESOURCES += \
    resource.qrc \
    mapres.qrc

RC_FILE += myico.rc

DISTFILES +=





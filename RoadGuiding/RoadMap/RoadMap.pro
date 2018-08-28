#-------------------------------------------------
#
# Project created by QtCreator 2018-01-18T13:26:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapRouting toolv1.0
TEMPLATE = app


SOURCES += main.cpp\
    DataStruct/datastruct.cpp \
    ImPortMap/xmlparser.cpp \
    Astar/Astar.cpp \
    mapwidget.cpp

HEADERS  += \
    DataStruct/datastruct.h \
    ImPortMap/xmlparser.h \
    Astar/Astar.h \
    mapwidget.h

FORMS    += \
    mapwidget.ui

RESOURCES += \
    mapres.qrc



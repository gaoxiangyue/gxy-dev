QT += widgets serialport

TARGET = SerialCI
TEMPLATE = app

HEADERS += \
    masterthread.h \
    widget.h

SOURCES += \
    main.cpp \
    masterthread.cpp \
    widget.cpp

FORMS +=


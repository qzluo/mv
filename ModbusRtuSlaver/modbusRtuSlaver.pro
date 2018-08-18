#-------------------------------------------------
#
# Project created by QtCreator 2017-03-27T08:42:40
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialCommSrv
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    serialcomm.cpp \
    commcontroller.cpp

HEADERS  += widget.h \
    serialcomm.h \
    commcontroller.h \
    public.h

FORMS    += widget.ui

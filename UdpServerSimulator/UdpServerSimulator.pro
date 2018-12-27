#-------------------------------------------------
#
# Project created by QtCreator 2018-12-27T17:14:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UdpServerSimulator
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    qudpserver.cpp

HEADERS  += widget.h \
    qudpserver.h

FORMS    += widget.ui

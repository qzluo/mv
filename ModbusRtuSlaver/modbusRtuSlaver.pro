#-------------------------------------------------
#
# Project created by QtCreator 2017-03-27T08:42:40
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serialCommSrv
TEMPLATE = app

system("echo QTSERIALPORT_BUILD_ROOT = $$OUT_PWD>> $$OUT_PWD/.qmake.cache")

SOURCES += main.cpp\
        widget.cpp \
    serialcomm.cpp \
    commcontroller.cpp

HEADERS  += widget.h \
    serialcomm.h \
    commcontroller.h \
    public.h

FORMS    += widget.ui

#install_it.path = $$OUT_PWD
#install_it.files += *.exe

#INSTALLS += install_it

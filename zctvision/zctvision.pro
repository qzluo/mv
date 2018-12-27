#-------------------------------------------------
#
# Project created by QtCreator 2018-06-26T11:35:58
#
#-------------------------------------------------

QT       += core gui xml serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zctvision
TEMPLATE = app

RC_ICONS = main_icon.ico

include($$PWD\..\source\opencv.pri)
include($$PWD\..\source\base\base.pri)
include($$PWD\..\source\spdlog\spdlog.pri)
include($$PWD\..\source\camera\camera.pri)
include($$PWD\..\source\inspect\inspect.pri)
include($$PWD\..\source\comm\comm.pri)
include($$PWD\..\source\algorithm\algorithm.pri)

INCLUDEPATH += $$PWD/ui
INCLUDEPATH += $$PWD/dataUpdate

SOURCES += main.cpp\
        mainwindow.cpp \
    graphicswidget.cpp \
    mainresource.cpp \
    projectsysinfo.cpp \
    qsysdefine.cpp \
    ui/qselectcameratypedlg.cpp \
    ui/qframecalinfosetupdlg.cpp \
    ui/qalgparassetupdlg.cpp \
    ui/qrtuoperatordlg.cpp \
    ui/qcommsetupdlg.cpp \
    ui/qrealtimeresultwidget.cpp \
    ui/qinspectparassetupdlg.cpp \
    ui/qgradeparassetupdlg.cpp \
    ui/qsysparassetupdlg.cpp \
    ui/qcameraparassetupdlg.cpp \
    ui/qctrlboardparassetupdlg.cpp \
    ui/qlogindlg.cpp \
    ui/qmodifypassworddlg.cpp \
    ui/qclasssetupdlg.cpp \
    dataUpdate/qudpserver.cpp \
    ui/qipsegmentctl.cpp

HEADERS  += mainwindow.h \
    graphicswidget.h \
    mainresource.h \
    projectsysinfo.h \
    qsysdefine.h \
    ui/qselectcameratypedlg.h \
    ui/qframecalinfosetupdlg.h \
    ui/qalgparassetupdlg.h \
    ui/qrtuoperatordlg.h \
    ui/qcommsetupdlg.h \
    ui/qrealtimeresultwidget.h \
    ui/qinspectparassetupdlg.h \
    ui/qgradeparassetupdlg.h \
    ui/qsysparassetupdlg.h \
    ui/qcameraparassetupdlg.h \
    ui/qctrlboardparassetupdlg.h \
    ui/qlogindlg.h \
    ui/qmodifypassworddlg.h \
    ui/qclasssetupdlg.h \
    dataUpdate/qudpserver.h \
    ui/qipsegmentctl.h

FORMS    += mainwindow.ui \
    ui/qclasssetupdlg.ui

RESOURCES += \
    images.qrc

TRANSLATIONS = zctvision_zh_cn.ts

DISTFILES += \
    zctvision_zh_cn.qm \
    images/serialcomm1.png \
    main_icon.ico \
    zctvision_zh_cn.ts

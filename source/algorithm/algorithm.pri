INCLUDEPATH += $$PWD\
DEPENDPATH += $$PWD\

HEADERS += \
    $$PWD/algorithmmanager.h \
    $$PWD/qzaoinspectalgapp.h \
    $$PWD/qzaoinspectalgapptester.h \
    $$PWD/qframecalinfo.h \
    $$PWD/qzaoinspectalgparas.h \
    $$PWD/external/include/HQEDetector.h \
    $$PWD/external/include/HQERecognizer.h

SOURCES += \
    $$PWD/algorithmmanager.cpp \
    $$PWD/qzaoinspectalgapp.cpp \
    $$PWD/qzaoinspectalgapptester.cpp \
    $$PWD/qframecalinfo.cpp \
    $$PWD/qzaoinspectalgparas.cpp

INCLUDEPATH+=$$PWD/external/include/

LIBS += -L$$PWD/external/lib\
    -lHQEDetector\
    -lHQERecognizer\

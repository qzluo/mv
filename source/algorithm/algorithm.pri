INCLUDEPATH += $$PWD\
DEPENDPATH += $$PWD\

HEADERS += \
    $$PWD/qzaoinspectalgapp.h \
    $$PWD/qzaoinspectalgapptester.h \
    $$PWD/qframecalinfo.h \
    $$PWD/qzaoinspectalgparas.h \
    $$PWD/external/include/HQEDetector.h \
    $$PWD/external/include/HQERecognizer.h

SOURCES += \
    $$PWD/qzaoinspectalgapp.cpp \
    $$PWD/qzaoinspectalgapptester.cpp \
    $$PWD/qframecalinfo.cpp \
    $$PWD/qzaoinspectalgparas.cpp

INCLUDEPATH+=$$PWD/external/include/

CONFIG(debug,debug|release){
LIBS += -L$$PWD/external/lib/deb\
    -lHQEDetector\
    -lHQERecognizer\
}else{
LIBS += -L$$PWD/external/lib/rel\
    -lHQEDetector\
    -lHQERecognizer\
}

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += $$PWD\..\..\external\Vimba\
INCLUDEPATH += $$PWD\..\..\external\DaHengImage\inc\

contains(DEFINES,X86){
LIBS += -L$$PWD\..\..\external\Vimba\VimbaC\Lib\Win32\
    -lVimbaC\
LIBS += -L$$PWD\..\..\external\Vimba\VimbaCPP\Lib\Win32\
    -lVimbaCPP\
LIBS += -L$$PWD\..\..\external\Vimba\VimbaImageTransform\Lib\Win32\
    -lVimbaImageTransform\

LIBS += -L$$PWD\..\..\external\DaHengImage\lib\x86\
    -lGxIAPICPP\
}
else{
LIBS += -L$$PWD\..\..\external\Vimba\VimbaC\Lib\Win64\
    -lVimbaC\

LIBS += -L$$PWD\..\..\external\Vimba\VimbaCPP\Lib\Win64\
    -lVimbaCPP\

LIBS += -L$$PWD\..\..\external\Vimba\VimbaImageTransform\Lib\Win64\
    -lVimbaImageTransform\

LIBS += -L$$PWD\..\..\external\DaHengImage\lib\x64\
    -lGxIAPICPP\
}

HEADERS  += \
    $$PWD/QDShowFuns.h \
    $$PWD/qopencvcapturectl.h \
    $$PWD/cameractl.h \
    $$PWD/qvirtualcameractl.h \
    $$PWD/vimbatester.h \
    $$PWD/qvmbcameraclt.h \
    $$PWD/FrameObserver.h \
    $$PWD/qdahengcameractl.h

SOURCES +=\
    $$PWD/QDShowFuns.cpp \
    $$PWD/qopencvcapturectl.cpp \
    $$PWD/cameractl.cpp \
    $$PWD/qvirtualcameractl.cpp \
    $$PWD/vimbatester.cpp \
    $$PWD/qvmbcameraclt.cpp \
    $$PWD/FrameObserver.cpp \
    $$PWD/qdahengcameractl.cpp


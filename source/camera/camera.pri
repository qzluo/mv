INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += $$PWD\..\..\external\Vimba\

contains(DEFINES,X86){
LIBS += -L$$PWD\..\..\external\Vimba\VimbaC\Lib\Win32\
    -lVimbaC\
LIBS += -L$$PWD\..\..\external\Vimba\VimbaCPP\Lib\Win32\
    -lVimbaCPP\
LIBS += -L$$PWD\..\..\external\Vimba\VimbaImageTransform\Lib\Win32\
    -lVimbaImageTransform\
}
else{
LIBS += -L$$PWD\..\..\external\Vimba\VimbaC\Lib\Win64\
    -lVimbaC\

LIBS += -L$$PWD\..\..\external\Vimba\VimbaCPP\Lib\Win64\
    -lVimbaCPP\

LIBS += -L$$PWD\..\..\external\Vimba\VimbaImageTransform\Lib\Win64\
    -lVimbaImageTransform\
}

HEADERS  += \
    $$PWD/QDShowFuns.h \
    $$PWD/qopencvcapturectl.h \
    $$PWD/cameractl.h \
    $$PWD/qvirtualcameractl.h \
    $$PWD/vimbatester.h \
    $$PWD/qvmbcameraclt.h \
    $$PWD/FrameObserver.h

SOURCES +=\
    $$PWD/QDShowFuns.cpp \
    $$PWD/qopencvcapturectl.cpp \
    $$PWD/cameractl.cpp \
    $$PWD/qvirtualcameractl.cpp \
    $$PWD/vimbatester.cpp \
    $$PWD/qvmbcameraclt.cpp \
    $$PWD/FrameObserver.cpp


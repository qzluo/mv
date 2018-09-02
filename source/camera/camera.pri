INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

INCLUDEPATH += $$PWD\..\..\external\Vimba\

#LIBS += -L$$PWD\..\..\external\Vimba\VimbaC\Lib\Win64\
#    -lVimbaC\

#LIBS += -L$$PWD\..\..\external\Vimba\VimbaCPP\Lib\Win64\
#    -lVimbaCPP\
contains(DEFINES,X86){
LIBS += -L$$PWD\..\..\external\Vimba\VimbaC\Lib\Win32\
    -lVimbaC\
LIBS += -L$$PWD\..\..\external\Vimba\VimbaCPP\Lib\Win32\
    -lVimbaCPP\
}
else{
LIBS += -L$$PWD\..\..\external\Vimba\VimbaC\Lib\Win64\
    -lVimbaC\

LIBS += -L$$PWD\..\..\external\Vimba\VimbaCPP\Lib\Win64\
    -lVimbaCPP\
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


INCLUDEPATH += $$PWD\..\external\OpenCV2.4.13\include\
INCLUDEPATH += $$PWD\..\external\OpenCV2.4.13\include\opencv\
INCLUDEPATH += $$PWD\..\external\OpenCV2.4.13\include\opencv2\

contains(DEFINES,X86){

CONFIG(debug,debug|release){
LIBS += -L$$PWD\..\external\OpenCV2.4.13\lib\x86\vc12\
    -lopencv_core2413d\
    -lopencv_highgui2413d\
    -lopencv_imgproc2413d\
    -lopencv_features2d2413d\
    -lopencv_calib3d2413d\
    -lopencv_flann2413d\
    -lopencv_video2413d\
    -lopencv_nonfree2413d\
}else{
LIBS += -L$$PWD\..\external\OpenCV2.4.13\lib\x86\vc12\
    -lopencv_core2413\
    -lopencv_highgui2413\
    -lopencv_imgproc2413\
    -lopencv_features2d2413\
    -lopencv_calib3d2413\
    -lopencv_flann2413\
    -lopencv_video2413\
    -lopencv_nonfree2413\
}
}
else{

CONFIG(debug,debug|release){
LIBS += -L$$PWD\..\external\OpenCV2.4.13\lib\x64\vc12\
    -lopencv_core2413d\
    -lopencv_highgui2413d\
    -lopencv_imgproc2413d\
    -lopencv_features2d2413d\
    -lopencv_calib3d2413d\
    -lopencv_flann2413d\
    -lopencv_video2413d\
    -lopencv_nonfree2413d\
}else{
LIBS += -L$$PWD\..\external\OpenCV2.4.13\lib\x64\vc12\
    -lopencv_core2413\
    -lopencv_highgui2413\
    -lopencv_imgproc2413\
    -lopencv_features2d2413\
    -lopencv_calib3d2413\
    -lopencv_flann2413\
    -lopencv_video2413\
    -lopencv_nonfree2413\
}
}

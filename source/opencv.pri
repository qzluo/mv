contains(DEFINES,X86){
INCLUDEPATH+=D:\OpenCV2.4.13\opencv\build\include\
             D:\OpenCV2.4.13\opencv\build\include\opencv\
             D:\OpenCV2.4.13\opencv\build\include\opencv2\

CONFIG(debug,debug|release){
LIBS += -LD:\OpenCV2.4.13\opencv\build\x86\vc12\lib\
    -lopencv_core2413d\
    -lopencv_highgui2413d\
    -lopencv_imgproc2413d\
    -lopencv_features2d2413d\
    -lopencv_calib3d2413d\
    -lopencv_flann2413d\
    -lopencv_video2413d\
    -lopencv_nonfree2413d\
}else{
LIBS += -LD:\OpenCV2.4.13\opencv\build\x86\vc12\lib\
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
INCLUDEPATH+=D:\OpenCV2.4.13\opencv\build\include\
             D:\OpenCV2.4.13\opencv\build\include\opencv\
             D:\OpenCV2.4.13\opencv\build\include\opencv2\

CONFIG(debug,debug|release){
LIBS += -LD:\OpenCV2.4.13\opencv\build\x64\vc12\lib\
    -lopencv_core2413d\
    -lopencv_highgui2413d\
    -lopencv_imgproc2413d\
    -lopencv_features2d2413d\
    -lopencv_calib3d2413d\
    -lopencv_flann2413d\
    -lopencv_video2413d\
    -lopencv_nonfree2413d\
}else{
LIBS += -LD:\OpenCV2.4.13\opencv\build\x64\vc12\lib\
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

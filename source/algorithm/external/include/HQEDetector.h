#ifndef HQEDETECTOR_H
#define HQEDETECTOR_H
#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;


//---opencv的库文件--------
#ifdef _DEBUG
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#pragma comment(lib, "opencv_imgproc2413d.lib")
#else
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
#endif






typedef struct tagHQEDetectorParams
{
	uchar Imin;//亮度,Y
	uchar Bmin;//Cb
	uchar Rmin;//Cr
	uchar Imax;
	uchar Bmax;
	uchar Rmax;
	int   Wmin;//min width of an object
	int   Hmin;//min height of an object;
	float RatioMax;//max long_edge/short_edge 


    tagHQEDetectorParams()
    {

		//((y>=50)  && (y<=200)) &&  ((cb>=55)&& (cb<=130))  && ((cr>=140)&& (cr<=235))
		Imin=50;
		Bmin=55;
		Rmin=140;

		Imax=200;
		Bmax=130;
		Rmax=235;
		Wmin=50;
		Hmin=50;

		RatioMax=3.0f;

    }

}HQEDetectorParams;


void* HQECreateDetector(int w, int h);

void HQEDestoryDetector(void* handle);

void HQEDetection(void* handle,Mat img,Mat &mask, vector<Mat> &img_roi_vec, vector<Rect> &rect_vec);

void HQESetParams(void*handle,HQEDetectorParams params);




//void *HQCreateObjectExtractor();
//
//void HQDestroyObjectExtractor(void* hanlde);
//
//int HQSetParams(void*handle,HQObjectExtrctorParams params);
//
//void HQInitBackground(void* handle,CvCapture *pvc,int N);
//
//int HQCountMovingObject(void* handle,unsigned char* img,int frameid);
//
//void HQExtractByThreeLine(void* handle,unsigned char* img,int frameid);
//
//void HQExtractByRect(void* handle,unsigned char* img,int frameid);
//
//int HQGetFreeMemory();
//
//void HQObjectExtract(void* handle,unsigned char *img,int frameid);







#endif
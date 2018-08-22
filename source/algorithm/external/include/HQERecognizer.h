#ifndef HQERECOGNIZER_H
#define HQERECOGNIZER_H
#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;


//---opencvµÄ¿âÎÄ¼þ--------
#ifdef _DEBUG
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#pragma comment(lib, "opencv_imgproc2413d.lib")
#pragma comment(lib, "opencv_ml2413d.lib")
//opencv_calib3d2413d.lib
//opencv_contrib2413d.lib
//opencv_core2413d.lib
//opencv_features2d2413d.lib
//opencv_flann2413d.lib
//opencv_gpu2413d.lib
//opencv_highgui2413d.lib
//opencv_imgproc2413d.lib
//opencv_legacy2413d.lib
//opencv_ml2413d.lib
//opencv_nonfree2413d.lib
//opencv_objdetect2413d.lib
//opencv_ocl2413d.lib
//opencv_photo2413d.lib
//opencv_stitching2413d.lib
//opencv_superres2413d.lib
//opencv_ts2413d.lib
//opencv_video2413d.lib
//opencv_videostab2413d.lib
#else
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
#pragma comment(lib, "opencv_ml2413d.lib")
#endif

#endif



typedef struct tagRecognizerParams
{
	//float RW;//regularization weight
	CvSVMParams params;

	tagRecognizerParams()
	{
		params.svm_type = SVM::C_SVC;
		params.C = 0.1;
		params.kernel_type = SVM::LINEAR;
		params.term_crit = TermCriteria(CV_TERMCRIT_ITER, (int)1e7, 1e-6);

	}

}HQERecognizerParams;


typedef struct tagFeatureExtractorParams
{
	int r;//°ë¾¶
	//int num;//È¦ÄÚ²ÉÑùµãÊý
	bool uniform;//true-59, false-256, if uniform==true, num must=8

	int xgrid;

	int ygrid;

	int width;

	int height;

	int dim;
	tagFeatureExtractorParams()
	{
		r=1;
		//num=8;
		uniform=true;

		height=100;

		width=100;

		xgrid=4;

		ygrid=4;

		dim=xgrid*ygrid*59;

	}

}HQEFeatureExtractorParams;




void* HQECreateRecognizer();



void HQEDestoryRecognizer(void*handle);


//feature 
void HQESetFeatureExtractorParams(void *handle,HQEFeatureExtractorParams params);

//recognizer
void HQESetRecognizerParams(void *handle,HQERecognizerParams params);

void HQERecognization(void *handle, vector<Mat> &img_vec, vector<int> &results);//1-ºÃ;2-Æ¤Æ¤;3-ÆÆÆ¤;4-ÁÑºÛ;5-ºÚÔæ


void HQERecognizerTrain(void*handle,char*filepath,char*savepath,char* savename);//1-ºÃ;2-Æ¤Æ¤;3-ÆÆÆ¤;4-ÁÑºÛ;5-ºÚÔæ

void HQEPerformanceReport(void*handle, char*filepath); 


void HQELoadModel(void*handle, char*filepath, char*filename);



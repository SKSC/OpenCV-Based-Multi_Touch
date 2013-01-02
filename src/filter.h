#ifndef FILTER_MT
#define FILTER_MT

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include "stdio.h"

class Filter{
private:
	CvMemStorage *stor;
	char a;
	char c;
public:
	//step.1:resize carmera shot image to 320*240 in pixels
	IplImage* ResizeSmaller(IplImage* source,IplImage* destination);
	//step.2:convert carmera-shot image from RGB model to gray-scale model
	IplImage* RGB2Gray(IplImage* source,IplImage* destination);     
	//step.3:invert the src_grey image	
	IplImage* Invert(IplImage* source,IplImage* destination);    
	//step.4:only bright spot can get in through the image
	IplImage* Highpass(IplImage* source,IplImage* destination);    
	//step.5:filter out the noise using a median filter
	IplImage* Median(IplImage* source,IplImage* destination);		
	//step.6:make bright spots become brighter
	IplImage* Scaler(IplImage* source,IplImage* destination);       
	//step.7:threshold the image to get finger point only
	IplImage* Threshold(IplImage* source,IplImage* destination);    
	//step.8:resize the image to 640*480
	IplImage* ResizeBigger(IplImage* source,IplImage* destination);
	//step.9:smooth the image
	IplImage* Smoothing(IplImage* source,IplImage* destination);   
    //recieve an orignal input image and then return finger points information
	CvSeq*    FilterFactory(IplImage* OrignalFrame);	           
};
#endif
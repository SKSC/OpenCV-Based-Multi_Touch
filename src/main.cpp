#include <stdafx.h>       // pre-compiled headers
#pragma comment(linker, "/NODEFAULTLIB:atlthunk.lib")
#include <videoInput.h>// third-party package, can be obtained from https://github.com/ofTheo/videoInput

#include "filter.h"
#include "recognition.h"

int main( int argc, char** argv ) {

	Filter filter;//initialize a filter object
	Recognition recognition;//initialize a recognition object
	videoInput VideoInput;//create a videoInput object, videoInput is a third-party package

	IplImage *pRgb=cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);// create an image
	
	VideoInput.setupDevice(0, 640,480);//setup device with specified video size
    // 
	while(1)
	{
		if(VideoInput.isFrameNew(0))//to get the data from the device first check if the data is new
		{
			//bool getPixels(int id, unsigned char * pixels, bool flipRedAndBlue, bool flipImage)
			VideoInput.getPixels(0, (unsigned char *)pRgb->imageData, false, true);//fills pixels as a RGB (for openGL) unsigned char array
			CvSeq* TouchBlobList=filter.FilterFactory(pRgb);//transfer this new frame into filter module and get TouchBlobList information
			recognition.FunctionSwitch(TouchBlobList);//
		}
	}
}
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include "stdio.h"
#include "filter.h"

//step.1:resize carmera shot image to 320*240 in pixels
IplImage* Filter::ResizeSmaller(IplImage* source,IplImage* destination)
{
	if(!destination)
    {
		//create a destination image to 320*240 in pixels with source's depth and channels
		destination = cvCreateImage(cvSize(320,240),source->depth,source->nChannels);
		destination->origin = 1;
		source->origin=1;  // same vertical flip as source
    }
	//resize source to destination using bicubic interpolation algorithm which provides best result of four choices but require most resource 
	cvResize(source,destination,CV_INTER_CUBIC);
	return destination;
}
//step.2:convert carmera-shot image from RGB model to gray-scale model
IplImage* Filter::RGB2Gray(IplImage* source,IplImage* destination)
{
	if( !destination )
    {
		//allocate a 1-channel byte image which means grey-scale
        destination = cvCreateImage(cvSize(source->width,source->height), source->depth, 1);
        destination->origin = source->origin;  // same vertical flip as source
    }
	//convert an image from RGB color model to gray
	cvCvtColor(source, destination, CV_BGR2GRAY);
	return destination;
}
//step.3:invert the grey-scale image
IplImage* Filter::Invert(IplImage* source,IplImage* destination)
{
	if( !destination )
    {
        destination = cvCreateImage(cvSize(source->width,source->height), source->depth, 1);
        destination->origin = source->origin;  // same vertical flip as source
    }
	//Performs per-element bit-wise inversion of array elements
	cvNot(source, destination);	
	return destination;
}
//step.4:blur filter,so that only bright spot can get in through the image
IplImage* Filter::Highpass(IplImage* source,IplImage* destination)
{
	 int blurParameter=45;//define blur paramenter equal to 45
	 if(!destination)
    {
        destination = cvCreateImage(cvSize(source->width,source->height), source->depth, 1);
        destination->origin = source->origin;  // same vertical flip as source
    }
	 //Smooths the image in CV_BLUR way which linear convolution with  box kernel (all 1¡¯s) with subsequent scaling by 1/(parma 1©qparma 2)
	 //fitsr blueParameter is smoothing operation, the aperture width;second is aperture height
	 cvSmooth(source, destination, CV_BLUR,blurParameter, blurParameter);
	 //Computes the per-element difference between first two parameters,result array will be stored in third parameter
	 //All the arrays must have the same type and the same size
	 cvSub(source, destination, destination);
	 return destination;
}
//step.5:filter out the noise using a median filter
IplImage* Filter::Median(IplImage* source,IplImage* destination)
{
	 int noiseParameter=13;//define noiseParameter equal to 13
	 if( !destination )
    {
        destination = cvCreateImage(cvSize(source->width,source->height), source->depth, 1);
        destination->origin = source->origin;  // same vertical flip as source
    }
	 //median filter with a param 1 x param 2  square aperture
	 cvSmooth(source, destination, CV_MEDIAN, noiseParameter, noiseParameter);
	 return destination;
}
//step6: scaler make bright spots brighter
IplImage* Filter::Scaler(IplImage* source,IplImage* destination)
{
	  float level=20;
	  if( !destination )
	  {
        destination = cvCreateImage(cvSize(source->width,source->height), source->depth, 1);
        destination->origin = source->origin;  // same vertical flip as source
	  }
	 //Calculates the per-element product of two source arrays.All the arrays must have the same type and the same size
     cvMul(source, source, destination,(float)level /128.0f);
	 return destination;
}
//step.7: threshold the highpass image
IplImage* Filter::Threshold(IplImage* source,IplImage* destination)
{
     int threshold=210;
	 if( !destination )
    {
        destination = cvCreateImage(cvSize(source->width,source->height), source->depth, 1);
        destination->origin = source->origin;  // same vertical flip as source
    }
 	 //Apply a fixed-level threshold to array elements.
	 cvThreshold(source, destination, threshold, 255, CV_THRESH_BINARY);	
	 return destination;
}
////step8 resize the image to 640*480
IplImage* Filter::ResizeBigger(IplImage* source,IplImage* destination)
{
	if( !destination )
    {
        destination = cvCreateImage(cvSize(640,480), source->depth, 1);
        destination->origin = source->origin;  // same vertical flip as source
    }
	//resize source to destination using bicubic interpolation algorithm which provides best result of four choices but require most resource 
	 cvResize(source,destination,CV_INTER_CUBIC);
	 return destination;
}
//step9 smooth the image
IplImage* Filter::Smoothing(IplImage* source,IplImage* destination)
{
	if( !destination )
    {
        destination = cvCreateImage(cvSize(source->width,source->height), source->depth, 1);
        destination->origin = source->origin;  // same vertical flip as source
    } 
	cvSmooth(source,destination, CV_MEDIAN,5,5);
	return destination;
}
//main function to filter image and return points information
CvSeq*    Filter::FilterFactory(IplImage* Frame)
{
	a=cvWaitKey(1);
	cvShowImage( "Video", Frame );//show image in a window named Video

	IplImage* destination = NULL;

	Frame = ResizeSmaller(Frame,destination);

	Frame = RGB2Gray(Frame,destination);//gray-scaled image

	Frame = Invert(Frame,destination);//inverted image

	Frame = Highpass(Frame,destination);//highpassed image

	Frame = Median(Frame,destination);
	
	Frame = Scaler(Frame,destination);
	
	Frame = Threshold(Frame,destination);
	
	Frame = ResizeBigger(Frame,destination);
	
	Frame = Smoothing(Frame,destination);
	cvReleaseImage( &destination );	
	
	c=cvWaitKey(1);
	cvShowImage( "MTframe",Frame );//show image in a window named MTframe
	if(a==27 || c==27){
		cvReleaseMemStorage(&stor);
		cvReleaseImage( &Frame );
		exit(1);  // ESC pressed, exit and end this programme
	}
    stor = cvCreateMemStorage(0);//create momery storage with default value which is 64K
	CvSeq* TouchBlobList = cvHoughCircles( Frame, stor, CV_HOUGH_GRADIENT, 2, 25, 300, 30 , 3, 35);//read circles information throght this black-white image
	cvReleaseImage( &Frame );	//release memory
	return TouchBlobList;
}
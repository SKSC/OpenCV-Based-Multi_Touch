#ifndef COMPLEXGESTURE_MT
#define COMPLEXGESTURE_MT

#include "stdafx.h"
#include "cv.h"			  //for Image processing
#include "highgui.h"	  //for opencv window
#include "cxcore.h"
#include "stdio.h"
struct LINE{
	POINT StartPoint;
	POINT PreviousPoint;
	POINT MiddlePoint;
	POINT EndPoint;
};
//In this class, currently I can only perform only one operation, that is draw a cross to close the window
class Pattern{
private:
	LINE LineOne;
	LINE LineTwo;
	int Counter;
	int LineNumber;
	int ReStart;
public:
	Pattern(){
		Counter=0;
		LineNumber=0;
		ReStart=0;
	};
	void estimation(CvSeq* circles);//
	LINE initialLine(LINE Line,CvSeq* circles);//
	LINE emptyLine(LINE Line);//
	LINE processLine(LINE Line,CvSeq* circles);
	void closeWindow();//use shortcut to close the window
	bool isIntersect(LINE LineOne,LINE LineTwo);//evalute whether two lines are intersected
	bool inSlope(LINE Line);//
	bool inRange(LINE Line,CvSeq* circles);//
	LINE exchange(LINE Line);//
	int VectorMul(LINE LineOne,LINE LineTwo);//
    int mul(POINT A,POINT B,POINT C);//
    int Max(int a,int b);//
    int Min(int a,int b);//
}; 
#endif
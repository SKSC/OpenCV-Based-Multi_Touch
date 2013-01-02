#ifndef GESTURE_MT
#define GESTURE_MT

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
struct CROSS{
	LINE LineOne;
	LINE LineTwo;
};
class Gesture{
private:
	CROSS Cross;//initialize a cross object
	int Counter;
	int LineNumber;//two lines in a cross, line one and lien two
	int ReStart;//
public:
	Gesture(){
		Counter=0;
		LineNumber=0;
		ReStart=0;
	};
	//the main function for cross close
	void estimation(CvSeq* TouchBlobList);
	//initial Line when first point appears
	LINE initialLine(LINE Line,CvSeq* TouchBlobList);
	//evaluate every point variable with zero
	LINE emptyLine(LINE Line);
	//evaluate 
	LINE processLine(LINE Line,CvSeq* TouchBlobList);
	//the function that close window
	void closeWindow();
	//recognize whether two lines are intersect
	bool isIntersect(CROSS Cross);
	//recognize whether the slopes fluctuate bewteen a certain range
	bool inSlope(LINE Line);
	bool length(CROSS Cross);
	//recognize whether the point is moving or not
	bool notMoving(LINE Line,CvSeq* TouchBlobList);
	//vector doc product
    int  mul(POINT A,POINT B,POINT C,POINT D);
	//return the Max one
    int  max(int a,int b);
	//compare variables A and B,return the min one
    int  min(int a,int b);

	//picture zoom in 
	void ZoomIn();
	//picture zoom out
	void ZoomOut();
	//picture rotates in clock wise
	void RotateClockWise();
	//picture rotates in anticlock wise
	void RotateAntiClockWise();
	//for browser tag back
	void TagBack();
	//for browser tag forward
	void TagForward();
	//minimize all windows and show desktop
	void MinimizeAll();
	//undo minimize all
	void UndoMinimizeAll();
	//for picture viewer, show next picture
	void NextPicture();
	//for pictre viewer,show previous picrure
	void PreviousPicture();
}; 
#endif
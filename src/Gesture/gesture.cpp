#include "stdafx.h"
#include "cv.h"			  //for Image processing
#include "highgui.h"	  //for opencv window
#include "cxcore.h"
#include "gesture.h"
#include "windows.h"
#include "stdio.h"
#include "math.h"

//main function
//Each operation is done by shortcut
void Gesture::estimation(CvSeq* TouchBlobList){
	if(TouchBlobList->total!=1){//some actions will be done when not one finger input
		Counter=0;
		ReStart++;//count the frames between drawing last point of line one and the first point of line two
		if(LineNumber==2){//if two lines are drawn, 
			if(isIntersect(Cross)){//if two lines intersect
				closeWindow();
				Cross.LineOne=emptyLine(Cross.LineOne);//clear for next use
		        Cross.LineTwo=emptyLine(Cross.LineTwo);//clear for next use
			}
			LineNumber=0;//initialize for next use
		}
		if(ReStart>=5)//if the time between drawing two lines is too long ,cancel this, and for begin a new one
			LineNumber=0;
	}else{
		ReStart=0;
		Counter++;
		//when first point comes,initialize all points on the line
		if(Counter==1){
			LineNumber++;
			if(LineNumber==1){
				Cross.LineOne=initialLine(Cross.LineOne,TouchBlobList);
			}
			else
				Cross.LineTwo=initialLine(Cross.LineTwo,TouchBlobList);
			//second point comes, evaluate
		}else if(LineNumber==1 && Counter==2 ){
				Cross.LineOne=processLine(Cross.LineOne,TouchBlobList);
		}else if(LineNumber==2 && Counter==2 ){
				Cross.LineTwo=processLine(Cross.LineTwo,TouchBlobList);
		}
		//when following points come,evalute and check whether it is in this line
		if(LineNumber==1 && Counter>2 && notMoving(Cross.LineOne,TouchBlobList)){
			Cross.LineOne=processLine(Cross.LineOne,TouchBlobList);
			if(inSlope(Cross.LineOne)){//if not in this slope range, clear for next use
				LineNumber=0;
			    Cross.LineOne=emptyLine(Cross.LineOne);
		        Cross.LineOne=emptyLine(Cross.LineTwo);
			    return;
			}
		}else if(LineNumber==2 && Counter>2 && notMoving(Cross.LineTwo,TouchBlobList)){
			Cross.LineTwo=processLine(Cross.LineTwo,TouchBlobList);
		    if(inSlope(Cross.LineTwo)){//if not in this slope range ,clear for next use
				LineNumber=0;
			    Cross.LineOne=emptyLine(Cross.LineOne);
		        Cross.LineTwo=emptyLine(Cross.LineTwo);
			    return;
			}
		}
	}
}
//initial line when first point comes
LINE Gesture::initialLine(LINE Line,CvSeq* TouchBlobList){
	float* p=(float*)cvGetSeqElem(TouchBlobList,0);
	Line.StartPoint.x=cvRound(p[0]);
	Line.StartPoint.y=cvRound(p[1]);
	Line.PreviousPoint.x=Line.StartPoint.x;
	Line.PreviousPoint.y=Line.StartPoint.y;
	Line.MiddlePoint.x=Line.StartPoint.x;
	Line.MiddlePoint.y=Line.StartPoint.y;
	Line.EndPoint.x=Line.StartPoint.x;
	Line.EndPoint.y=Line.StartPoint.y;
	return Line;
}
//evaluate points 
LINE Gesture::processLine(LINE Line,CvSeq* TouchBlobList){
	float* p=(float*)cvGetSeqElem(TouchBlobList,0);
	Line.PreviousPoint.x=Line.MiddlePoint.x;
	Line.PreviousPoint.y=Line.MiddlePoint.y;
	Line.MiddlePoint.x=Line.EndPoint.x;
	Line.MiddlePoint.y=Line.EndPoint.y;
	Line.EndPoint.x=cvRound(p[0]);
	Line.EndPoint.y=cvRound(p[1]);
	return Line;
}
//empty the line points with zero before a new use start
LINE Gesture::emptyLine(LINE Line){
	Line.StartPoint.x=0;
	Line.StartPoint.y=0;
	Line.PreviousPoint.x=0;
	Line.PreviousPoint.y=0;
	Line.MiddlePoint.x=0;
	Line.MiddlePoint.y=0;
	Line.EndPoint.x=0;
	Line.EndPoint.y=0;
	return Line;
}
//function to close window
void Gesture::closeWindow(){//Alt+F4 to close windows
	keybd_event(VK_MENU,0,0,0);//win button
    keybd_event(VK_F4,0,0,0);//M button
    keybd_event(VK_F4,0,KEYEVENTF_KEYUP,0);
    keybd_event(VK_MENU,0,KEYEVENTF_KEYUP,0);
}
//recognize whether two lines are intersected
bool Gesture::isIntersect(CROSS Cross){
	if( ( max(Cross.LineOne.StartPoint.x,Cross.LineOne.EndPoint.x) >= min(Cross.LineTwo.StartPoint.x,Cross.LineTwo.EndPoint.x) )
        &&( max(Cross.LineTwo.StartPoint.x,Cross.LineTwo.EndPoint.x) >= min(Cross.LineOne.StartPoint.x,Cross.LineOne.EndPoint.x) )
        &&( max(Cross.LineOne.StartPoint.y,Cross.LineOne.EndPoint.y) >= min(Cross.LineTwo.StartPoint.y,Cross.LineTwo.EndPoint.y) )
        &&( max(Cross.LineTwo.StartPoint.y,Cross.LineTwo.EndPoint.y) >= min(Cross.LineOne.StartPoint.y,Cross.LineOne.EndPoint.y) )    
        &&(mul(Cross.LineOne.StartPoint,Cross.LineOne.EndPoint,Cross.LineTwo.StartPoint,Cross.LineTwo.EndPoint)<=0)
		&&(mul(Cross.LineTwo.StartPoint,Cross.LineTwo.EndPoint,Cross.LineOne.StartPoint,Cross.LineOne.EndPoint)<=0)
		&& length(Cross)
		)
		return true;
	else
		return false;
}
//vertor doc prodect
int  Gesture::mul(POINT A,POINT B,POINT C,POINT D){
	return (2*C.x-A.x-B.x)*(2*D.x-A.x-B.x)-(2*C.y-A.y-B.y)*(2*D.y-A.y-B.y);
}
//calculate the line length,only beyond a certain length will work for cross close
bool Gesture::length(CROSS Cross){
	if((pow(double(Cross.LineOne.StartPoint.x-Cross.LineOne.EndPoint.x),2) + pow(double(Cross.LineOne.StartPoint.y-Cross.LineOne.EndPoint.y),2) )>10000
		&&(pow(double(Cross.LineTwo.StartPoint.x-Cross.LineTwo.EndPoint.x),2) +pow(double(Cross.LineTwo.StartPoint.y-Cross.LineTwo.EndPoint.y),2) )>10000 )
		return true;
	else 
		return false;
}
//recognize whether the slope float between a certain range
bool Gesture::inSlope(LINE Line){
	if(Line.EndPoint.x-Line.MiddlePoint.x==0 || Line.MiddlePoint.x-Line.PreviousPoint.x==0)
		return false;
	if( abs( (Line.EndPoint.y-Line.MiddlePoint.y)/(Line.EndPoint.x-Line.MiddlePoint.x) - (Line.MiddlePoint.y-Line.PreviousPoint.y)/(Line.MiddlePoint.x-Line.PreviousPoint.x)  )>2)
		return true;
	else 
		return false;
}
//recognize whether a point is moving or not
bool Gesture::notMoving(LINE Line,CvSeq* TouchBlobList){
	float* p=(float*)cvGetSeqElem(TouchBlobList,0);
	if(abs(cvRound(p[0])-Line.EndPoint.x)>10 &&  abs(cvRound(p[1])-Line.EndPoint.y)>10)// range 10 pix
		return true;
	else 
		return false;
}
//return the max one
int  Gesture::max(int a,int b){
    return a > b ? a : b ;
}
//return the min one
int  Gesture::min(int a,int b){
    return a < b ? a : b ;
}
//for picture zoom in
void Gesture::ZoomIn(){//for windows pcture viewer,'-'
	keybd_event(109,0,0,0);//'-' button
    keybd_event(109,0,KEYEVENTF_KEYUP,0);
}
//for picture zoom out
void Gesture::ZoomOut(){//for windows picture viewer,shift+ '+'
	keybd_event(16,0,0,0);//shift button
    keybd_event(107,0,KEYEVENTF_KEYUP,0);//button '+'
	keybd_event(107,0,0,0);
    keybd_event(16,0,KEYEVENTF_KEYUP,0);
}
//for picture rotateclock wise
void Gesture::RotateClockWise(){//for windows picture viewer,Ctrl+K
	keybd_event(17,0,0,0);//ctrl
    keybd_event(75,0,0,0);//K button
    keybd_event(75,0,KEYEVENTF_KEYUP,0);
    keybd_event(17,0,KEYEVENTF_KEYUP,0);
}
//for picture rotate anticlockwise
void Gesture::RotateAntiClockWise(){//for windows picture viewer,Ctrl+L
	keybd_event(17,0,0,0);//ctrl
	keybd_event(76,0,0,0);//L button
	keybd_event(76,0,KEYEVENTF_KEYUP,0);
	keybd_event(17,0,KEYEVENTF_KEYUP,0);
}
//for browser,tag back
void Gesture::TagBack(){//for browser like IE, Firefox,Chrome,Ctrl+PageUp
	keybd_event(17,0,0,0);//Ctrl
    keybd_event(33,0,0,0);//PageUp
    keybd_event(33,0,KEYEVENTF_KEYUP,0);
    keybd_event(17,0,KEYEVENTF_KEYUP,0);
}
//for browser,tag forward
void Gesture::TagForward(){//for browser like IE or Firefox or Chrome, Ctrl+PageDown
	keybd_event(17,0,0,0);//Ctrl button 
    keybd_event(34,0,0,0);//PageDown button
    keybd_event(34,0,KEYEVENTF_KEYUP,0);
    keybd_event(17,0,KEYEVENTF_KEYUP,0);
}
//minimize all windows and show desktop
void Gesture::MinimizeAll(){//win+M
	keybd_event(VK_RWIN,0,0,0);//win button
    keybd_event(0x4D,0,0,0);//M button
    keybd_event(0x4D,0,KEYEVENTF_KEYUP,0);
    keybd_event(VK_RWIN,0,KEYEVENTF_KEYUP,0);
}
//undo minimize all,show windows
void Gesture::UndoMinimizeAll(){//Win+shift+M
	keybd_event(VK_LWIN,0,0,0);//window button
    keybd_event(16,0,0,0);//shift button
    keybd_event(77,0,0,0);//M button
    keybd_event(77,0,KEYEVENTF_KEYUP,0);
    keybd_event(16,0,KEYEVENTF_KEYUP,0);
    keybd_event(VK_LWIN,0,KEYEVENTF_KEYUP,0);
}
//for picture viewer,show next picture
void Gesture::NextPicture(){//right arrow button
	keybd_event(VK_RIGHT,0,0,0);
	keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,0);//right arrow button down and up
}
//for picture viewer, show previous picture
void Gesture::PreviousPicture(){//left arrow
	keybd_event(VK_LEFT,0,0,0);
	keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,0);//left arrow button down and up
}
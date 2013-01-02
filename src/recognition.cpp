#include "stdafx.h"       // pre-compiled headers
#include <afx.h>          // MFC header
#include "cv.h"			  //for Image processing
#include "highgui.h"	  //for opencv window
#include "cxcore.h"
#include "stdio.h"
#include "math.h"

#include "mouse.h"
#include "keyboard.h"
#include "recognition.h"
#include "gesture.h"

const int FlagForMouseDrag = 3600;
//functiown switch among mouse gesture and keyboard automatically
void Recognition::FunctionSwitch(CvSeq* TouchBlobList){
	/*
	Single key                 SwitchCase=1;
	MouseMove                  SwitchCase=2;
	Combination                SwitchCase=3;
	TwoFingersMouse            SwitchCase=4;
	ThreeFingerMouse           SwitchCase=5;
	ThreeKeysCombination       SwitchCase=6;
	*/
	//less fingers number's action can jump to more fingers numbers' action, but it can not jump back
	for(int i=0;i<TouchBlobList->total;i++){
		float* p=(float*)cvGetSeqElem(TouchBlobList,i);
		if(cvRound(p[0])<65 || cvRound(p[0])>590 ||cvRound(p[1])<35||cvRound(p[1])>410){
			MouseGestureSwitch(TouchBlobList);
			SwitchCounter=0;
			SwitchCase=0;
			return;
		}
	}
	SwitchCounter++;//the total number of how many frames come
	if(SwitchCounter==1){//initialize when first frame comes
		PreviousTouchBlobList=cvCloneSeq(TouchBlobList,NULL);
	}
	switch (TouchBlobList->total){//To switch among different numbers of circles
		case 0://if zero finger ,pass this to all functions and they will have corresponding actions
			gesture.estimation(TouchBlobList);
			MouseMoveToLeftClick++;
			keyboard.OutputKey(TouchBlobList);
			MouseGestureSwitch(TouchBlobList);
			SwitchCounter=0;//clear
			SwitchCase=0;//clear
			if(MouseMoveToLeftClick>5){//count for left click,the time between two left click should be limited 
				MouseMoveToLeftClick=0;
				SingleClick=false;//forget last click,begin a new one
			}
			keyboard.UpKeys();
			break;
		case 1:
			gesture.estimation(TouchBlobList);//for cross close window
			POINT PreviousPoint;//initilize a point for previousPoint object
			POINT Point;// initilize a point for current point object
			if(SwitchCase==2){//if previous action is mouse movement action
				MouseGestureSwitch(TouchBlobList);
				break;
			}else if(SingleClick==true && MouseMoveToLeftClick!=0){
				MouseGestureSwitch(TouchBlobList);
				MouseMoveToLeftClick=0;
				keyboard.ClearVirtualKey();
				break;
				//if none of these matches, evaluate for next use
			}else if(PreviousTouchBlobList->total==1 && TouchBlobList->total==1){
				float* p=(float*)cvGetSeqElem(PreviousTouchBlobList,0);
				PreviousPoint.x=cvRound(p[0]);
				PreviousPoint.y=cvRound(p[1]);
			    float* q=(float*)cvGetSeqElem(TouchBlobList,0);
				Point.x=cvRound(q[0]);
				Point.y=cvRound(q[1]);
			}
			//
			if(SwitchCase==0 || SwitchCase==1){
				if(abs(Point.x-PreviousPoint.x)<10 && abs(Point.y-PreviousPoint.y)<10){
					SwitchCase=1;
					keyboard.OutputKey(TouchBlobList);
					break;
				}else if((abs(Point.x-PreviousPoint.x)>10 || abs(Point.y-PreviousPoint.y)>10)){
					SwitchCase=2;
					SingleClick=true;
					keyboard.ClearVirtualKey();
					MouseGestureSwitch(TouchBlobList);
					break;
				}
			}
			break;
		case 2:
			if(SwitchCase==3){
	    		keyboard.OutputKey(TouchBlobList);
				break;
			}else if (SwitchCase==4){
				MouseGestureSwitch(TouchBlobList);
				break;
			}
			//in case previous action is not zero finger or one finger
			if(SwitchCase==0 || SwitchCase==1){
				if(PreviousTouchBlobList->total==1 && TouchBlobList->total==2){
					keyboard.OutputKey(TouchBlobList);
					SwitchCase=3;
					break;
				}else{
					MouseGestureSwitch(TouchBlobList);
					SwitchCase=4;
					break;
				}
			}
			break;
			//three fingers action
		case 3:
			if(SwitchCase==6){
				keyboard.OutputKey(TouchBlobList);
				break;
			}else if (SwitchCase==5){
				MouseGestureSwitch(TouchBlobList);
				break;
			}
			//
			if(SwitchCase==0 || SwitchCase==3 || SwitchCase==4){
				if(PreviousTouchBlobList->total==2&& TouchBlobList->total==3){
					keyboard.OutputKey(TouchBlobList);
					SwitchCase=6;
				}else{
					MouseGestureSwitch(TouchBlobList);
					SwitchCase=5;
				}
			}
			break;
		default://more then three fingers are all mouse actions
			MouseGestureSwitch(TouchBlobList);// only mouse & gestue has four fingers action
		    SwitchCounter=0;//clean for next use
			SwitchCase=0;//clean for next use
			break;
	}
	PreviousTouchBlobList=cvCloneSeq(TouchBlobList,NULL);//copy current TouchBlobList to previous one
}
//initial variables used for mouse and gesture switch
void Recognition::Initial(){
	//every time when change between among different numbers of fingers,run this initial to clear used variables
	 Counter=0;//a counter for counting frame number
	 InitialDistance=0;
	 ProcessedDistance=0;
	 CaseFlag=0;//each finger number has a Caseflag number
	 WindowType=0;//different number means different window type
}
//the recognize which window type it is,browser or picture viewer
int  Recognition::WindowsRecognition(){
	//tell whether a window is a browser or a picture viwer according to window text,used for three fingers action
	CString WindowText;//CString is usually used in MFC
	HWND  hwnd=GetForegroundWindow();//get current window's handle
	char* myText=new char[300];
   ::GetWindowTextA(hwnd,myText,300);//get current window's text accoring to handle
   WindowText=myText;//convert char* to CSting for next step comparision
   delete []myText;//delete used space in momery
   if(WindowText.Right(22)=="Picture and Fax Viewer")//tell whether current window is picture viewer 
	   return 1;
   else if(WindowText.Right(8)=="Explorer" || WindowText.Right(7)=="Firefox"||WindowText.Right(6)=="Chrome")
	   //tell whether current window is a browser
	   return 2;
   //if neither is matched
   return 0;
}
//switch between mouse and gesture
void Recognition::MouseGestureSwitch(CvSeq* TouchBlobList){
	switch (TouchBlobList->total){
		//initial, button release,left click,two left click is double click
			case 0:
				OneFinger++;// a counter
				//for mouse drag,clear variables after being used
				if(OneFinger==3){
					MouseDragFlag=0;
					if(LeftButton){
					mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);//release left button
					LeftButton=false;//flag, left button not pressed
					}
					OneFinger=0;
				}
				//left click or double clicks action in interval based on one finger relative action
				if(Counter==1 || Counter==2){
					mouse.LeftClick();
					MouseDragFlag=FlagForMouseDrag;//for mouse drag Recognition 
				}
	            Initial();
				break;
				//mouse drag,mouse move
			case 1:
				//if switch from another case,first initialize variables
				if(CaseFlag!=1)
					Initial();
				CaseFlag=1;//set CaseFlag equal to 1 
				for(int i=0;i<TouchBlobList->total;i++){
					float* p=(float*)cvGetSeqElem(TouchBlobList,0);
					PointForCaseOne.x=cvRound(p[0]);
					PointForCaseOne.y=cvRound(p[1]);
				}//evaluate
				Counter++;
				//when first point comes,cursor location is current point location
				if(Counter==1){
					CursorInitialPosition.x=PointForCaseOne.x;
					CursorInitialPosition.y=PointForCaseOne.y;
				}
				//this is for mouse drag
				MouseDragFlag++;
				if(MouseDragFlag==(FlagForMouseDrag+1)){
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);//press left button and not release
					LeftButton=true;
					GetCursorPos(&pt);//get current mouse pointer location
					//set cursorlocation according to point relative movement
					SetCursorPos(pt.x+PointForCaseOne.x-CursorInitialPosition.x,pt.y+CursorInitialPosition.y-PointForCaseOne.y); 
					CursorInitialPosition.x=PointForCaseOne.x;
					CursorInitialPosition.y=PointForCaseOne.y;//replace previous location with current one
					MouseDragFlag=FlagForMouseDrag;
			    	break;
				}
				else{
					//mouse movement, set sursor location according to point relatice movement
					GetCursorPos(&pt);
					SetCursorPos(pt.x+PointForCaseOne.x-CursorInitialPosition.x,pt.y+CursorInitialPosition.y-PointForCaseOne.y);
					CursorInitialPosition.x=PointForCaseOne.x;
					CursorInitialPosition.y=PointForCaseOne.y;
					MouseDragFlag=0;
			    	break;
				}
				//right click,clockwise,anticlockwise,zoom in,zoom out
			case 2:
				if(CaseFlag!=2)//if switch from another case,first initialize variables
					Initial();
				CaseFlag=2;
				for(int i=0;i<TouchBlobList->total;i++){
					float* p=(float*)cvGetSeqElem(TouchBlobList,i);
					PointForCaseTwo[i].x=cvRound(p[0]);
					PointForCaseTwo[i].y=cvRound(p[1]);
				}//evaluate
				Counter++;
				//when first point comes,initialize,calculate the distance between two points and the angle relative to X axis
				if(Counter==1){
					InitialDistance=(double)(pow(double(PointForCaseTwo[0].x-PointForCaseTwo[1].x),2)+pow(double(PointForCaseTwo[0].y-PointForCaseTwo[1].y),2));
					InitialAngle=(PointForCaseTwo[0].y+PointForCaseTwo[1].y)/(PointForCaseTwo[0].x+PointForCaseTwo[1].x);
					InitialAngle=atan(InitialAngle);//angle
					InitialDistance=sqrt(InitialDistance);//distance
				}
				else{
					double ProcessedDistance=(double)(pow(double(PointForCaseTwo[0].x-PointForCaseTwo[1].x),2)+pow(double(PointForCaseTwo[0].y-PointForCaseTwo[1].y),2));
					//calculate the distance of two points in following frame
					ProcessedDistance=sqrt(ProcessedDistance);
					ProcessedAngle=(PointForCaseTwo[0].y+PointForCaseTwo[1].y)/(PointForCaseTwo[0].x+PointForCaseTwo[1].x);
					ProcessedAngle=atan(ProcessedAngle);//angle
					//calculate two angles in vector,only beyond a certain angle will work
					if((InitialAngle-ProcessedAngle)>(3.1415926/40)){
						gesture.RotateClockWise();
						InitialAngle=ProcessedAngle;
						break;
					}else if((ProcessedAngle-InitialAngle)>(3.1415926/40)){//calculate two angles in vector,picture clockwise
						gesture.RotateAntiClockWise();
						InitialAngle=ProcessedAngle;
						break;
					}else if((ProcessedDistance+160)<InitialDistance){//picture zoom out
					//calclate the distance, only when difference value greater than a certain number will following codes run
						gesture.ZoomIn();
			    		InitialDistance=ProcessedDistance;
						break;
					}else if((ProcessedDistance-160)>InitialDistance){//picture zoom in
						gesture.ZoomOut();
					    InitialDistance=ProcessedDistance;
						break;
					}else if(abs(ProcessedDistance-InitialDistance)<5 && Counter==6){//double click
						mouse.RightClick();
						break;
					}
				}
				break;
				//prevous or next picture,tag back and forwards 
			case 3:
				if(CaseFlag!=3)
					Initial();//clear all used variable when switch among different fingers action
				CaseFlag=3;
				ProcessedDistance=0;
				for(int i=0;i<TouchBlobList->total;i++){
					float* p = (float*)cvGetSeqElem(TouchBlobList,i);//get each point's coordinate
					ProcessedDistance=ProcessedDistance+cvRound(p[0]);//sum three fingers' X asix value
				}
				Counter++;
				if (Counter==1){
					InitialDistance=ProcessedDistance;//initial
				}
				else{
					WindowType=WindowsRecognition();//Recognition what kind of this window is
					if(WindowType==2){//if browser
						if((ProcessedDistance-180)>InitialDistance){//tag forward
							gesture.TagForward();
						    InitialDistance=ProcessedDistance;//replace initial sum with current sum of x axis
						    break;
						}
	                 	if((ProcessedDistance+180)<InitialDistance){//tag back
					    	gesture.TagBack();
							InitialDistance=ProcessedDistance;
					    	break;
						}
					}
					//if current window is picture viewer
					else if(WindowType==1){
						if((ProcessedDistance-180)>InitialDistance){//change to next picture
							gesture.NextPicture();
						    InitialDistance=ProcessedDistance;
						    break;
						}
	                 	if((ProcessedDistance+180)<InitialDistance){//change to previous picture
							gesture.PreviousPicture();
							InitialDistance=ProcessedDistance;
					    	break;
						}
					}
				}
				break;
			case 4:
				//if jump from another case,fist initialize
				if(CaseFlag!=4)
					Initial();
				CaseFlag=4;//set CaseFlag equal to 4
				ProcessedDistance=0;
				for(int i=0;i<TouchBlobList->total;i++){
					float* p = (float*)cvGetSeqElem(TouchBlobList,i);
					ProcessedDistance=ProcessedDistance+cvRound(p[1]);//sum up four fingers' Y axis value
				}
				Counter++;
				if (Counter==1){
					InitialDistance=ProcessedDistance;//initialize at begin
				}
				else{
					//movement should greater than a certain distance from bottom to top
					if((ProcessedDistance-160)>InitialDistance){
						gesture.UndoMinimizeAll();//call certain function
						InitialDistance=ProcessedDistance;
						break;
					}
					//move from top to bottom
					if((ProcessedDistance+160)<InitialDistance){
						InitialDistance=ProcessedDistance;
						gesture.MinimizeAll();//call certain function
						break;
					}
				}
				break;
			//do not support more than 4 fingers' actions
			default:
				Initial();
				break;
		}		
}
#ifndef Recognition_MT
#define Recognition_MT

#include "stdafx.h"      // pre-compiled headers
#include "cv.h"			  //for Image processing
#include "highgui.h"	  //for opencv window
#include "cxcore.h"       //unnecessary, included in cv.h
#include "stdio.h"

#include "mouse.h"
#include "gesture.h"
#include "keyboard.h"

class Recognition{
private:
	Mouse mouse;// create a mouse object
	Keyboard keyboard;//create a keboard object
	Gesture gesture;//create a gesture object

	int SwitchCounter;
	int SwitchCase;
	CvSeq* PreviousTouchBlobList;
	bool SingleClick;
	int MouseMoveToLeftClick;

	double InitialDistance;
	double ProcessedDistance;
	POINT PointForCaseOne;//used in switching between mouse and gesture
	POINT PointForCaseTwo[2];//used in switching between mouse and gesture
	POINT CursorInitialPosition;
	int Counter;
	float InitialAngle;
	float ProcessedAngle;
	int CaseFlag;//used when switch between mouse and gesture
	POINT pt;//cursor location
	int WindowType;//window type, for three fingers actions, gesture
	int OneFinger;//counter for cleaning mouse drag variables
	int MouseDragFlag;//a counter for mouse drag
	bool LeftButton;//show the status whether left button is down or up, for mouse drag
public:
	Recognition(){
		SwitchCounter=0;
		SwitchCase=0;
		MouseMoveToLeftClick=0;
		SingleClick=false;

		CursorInitialPosition.x=0;
		CursorInitialPosition.y=0;
	    Counter=0;
	    InitialAngle=0;
	    ProcessedAngle=0;
	    WindowType=0;
	    OneFinger=0;
        MouseDragFlag=0;
	    LeftButton=false;
	};
	void FunctionSwitch(CvSeq* TouchBlobList);    //identify between keyboard action and mouse action
	void MouseGestureSwitch(CvSeq* TouchBlobList);//switch between mouse and gesture
	int  WindowsRecognition();              //To tell whether an active window is a browser or a picture window
	void Initial();                         //initialize during switch among different mouse actions
};
#endif
#ifndef Determine_MT
#define Determine_MT

#include "stdafx.h"      // pre-compiled headers
//#include <afx.h>
#include <windows.h>	  //for Windows Api
#include <Winuser.h>
#include "cv.h"			  //for Image processing
#include "highgui.h"	  //for opencv window
#include "cxcore.h"
#include "stdio.h"

#include "math.h"
#include "iostream"

#include "mouse.h"
#include "keyboard.h"
#include "gesture.h"

class Determine{
private:
	double InitialDistance;
	double ProcessedDistance;
//	int Test;
	double X1[2];
	double X2[2][2];
	double CooX;
	double CooY;
	int Temp;
	float InitialAngle;
	float ProcessedAngle;
	int CaseFlag;
	Mouse mouse;
	Gesture gesture;
	Keys keyboard;
	POINT pt;
	int Judge;
	int OneFinger;
	int MouseMoveFlag;
	bool LeftButton;

	int SwitchTemp;
	int SwitchCase;
	CvSeq* circlesTemp;
	bool SingleClick;
	int MouseMoveToLeftClick;
public:
	Determine(){
		SwitchTemp=0;
		SwitchCase=0;
//		Test=0;
		CooX=0;
	    CooY=0;
	    Temp=0;
	    InitialAngle=0;
	    ProcessedAngle=0;
	    Judge=0;
	    OneFinger=0;
        MouseMoveFlag=0;
	    LeftButton=false;

		MouseMoveToLeftClick=0;
		SingleClick=false;
	};
	int WindowsJudge();//To tell whether an active window is a browser or a picture window
	void JudgementMouse(CvSeq* circles);//identify different mouse actions
	void Initial();//initialize during switch among different mouse actions
	void FunctionSwitch(CvSeq* circles);//identify between keyboard action and mouse action
};
#endif
#ifndef DISTINGUISH_MT
#define DISTINGUISH_MT

#include "stdafx.h"      // pre-compiled headers
#include "cv.h"			  //for Image processing
#include "highgui.h"	  //for opencv window
#include "cxcore.h"
#include "stdio.h"
#include "mouse.h"
#include "keyboard.h"
#include "pattern.h"

class Distinguish{
private:
	Mouse mouse;
	Keyboard keyboard;
	Pattern pattern;
	int SwitchCounter;
	int SwitchCase;
	CvSeq* circlesTemp;
	bool SingleClick;
	int MouseMoveToLeftClick;
public:
	Distinguish(){
		SwitchCounter=0;
		SwitchCase=0;
		MouseMoveToLeftClick=0;
		SingleClick=false;
	};
	void FunctionSwitch(CvSeq* circles);//identify between keyboard action and mouse action
};
#endif
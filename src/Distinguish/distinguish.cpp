#include "stdafx.h"       // pre-compiled headers
#include "cv.h"			  //for Image processing
#include "highgui.h"	  //for opencv window
#include "cxcore.h"
#include "stdio.h"
#include "math.h"
//#include "iostream"

#include "mouse.h"
#include "keyboard.h"
#include "distinguish.h"
#include "pattern.h"

void Distinguish::FunctionSwitch(CvSeq* circles){
	/*
	ThreeButtonsCombination SwitchCase=1;
	MouseMove               SwitchCase=2;
	Combination             SwitchCase=3;
	TwoFingersMouse         SwitchCase=4;
	ThreeFingerMouse        SwitchCase=5;
	*/
	for(int i=0;i<circles->total;i++){//if fingers are not in keyboard area,that must be mouse action
		float* p=(float*)cvGetSeqElem(circles,0);
		if(cvRound(p[0])<65 || cvRound(p[0])>590 ||cvRound(p[1])<35||cvRound(p[1])>410){
			mouse.MouseGesture(circles);
			SwitchCounter=0;
			SwitchCase=0;
			return;
		}
	}
	SwitchCounter++;
	if(SwitchCounter==1){//initialize 
		circlesTemp=cvCloneSeq(circles,NULL);
	}
	switch (circles->total){//To switch amont different numbers of circles
		case 0://if zero finger circle,pass this to both and they will have corresponding actions
			pattern.estimation(circles);
			MouseMoveToLeftClick++;
			keyboard.OutputKey(circles);
			mouse.MouseGesture(circles);
			SwitchCounter=0;
			SwitchCase=0;
			if(MouseMoveToLeftClick>=10){//count for left click
				MouseMoveToLeftClick=0;
				SingleClick=false;
			}
			break;
		case 1:
			pattern.estimation(circles);
			int PointTemp[2];
			int Point[2];
			if(SwitchCase==2){
				mouse.MouseGesture(circles);
				break;
			}
			if(SingleClick==true && MouseMoveToLeftClick!=0){
				mouse.MouseGesture(circles);
				MouseMoveToLeftClick=0;
				break;
			}
			if(circlesTemp->total==1 && circles->total==1){
				float* p=(float*)cvGetSeqElem(circlesTemp,0);
				PointTemp[0]=cvRound(p[0]);
				PointTemp[1]=cvRound(p[1]);
			    float* q=(float*)cvGetSeqElem(circles,0);
				Point[0]=cvRound(q[0]);
				Point[1]=cvRound(q[1]);
			}
			if(abs(Point[0]-PointTemp[0])<10 && abs(Point[1]-PointTemp[1])<10){
				SwitchCase=1;
				keyboard.OutputKey(circles);
				break;
			}else if((abs(Point[0]-PointTemp[0])>10 || abs(Point[1]-PointTemp[1])>10)){
				SwitchCase=2;
				SingleClick=true;
				mouse.MouseGesture(circles);
				break;
			}
		case 2:
			if(SwitchCase==3){
				keyboard.OutputKey(circles);
				break;
			}else if (SwitchCase==4){
				mouse.MouseGesture(circles);
				break;
			}
			if(circlesTemp->total==1&& circles->total==2){
				keyboard.OutputKey(circles);
				SwitchCase=3;
				break;
			}else{
				mouse.MouseGesture(circles);
				SwitchCase=4;
				break;
			}
		case 3:
			if(SwitchCase==1){
				keyboard.OutputKey(circles);
				break;
			}else if (SwitchCase==5){
				mouse.MouseGesture(circles);
				break;
			}
			if(circlesTemp->total==2&& circles->total==3){
				keyboard.OutputKey(circles);
				SwitchCase=1;
				break;
			}else{
				mouse.MouseGesture(circles);
				SwitchCase=5;
				break;
			}
		default://more then three fingers are all mouse actions
			mouse.MouseGesture(circles);
		    SwitchCounter=0;
			SwitchCase=0;
			break;
	}
	circlesTemp=cvCloneSeq(circles,NULL);
}
#include "stdafx.h"
#include "cv.h"			  //for Image processing
#include "highgui.h"	  //for opencv window
#include "cxcore.h"
#include "pattern.h"
#include "stdio.h"
#include <iostream>

void Pattern::estimation(CvSeq* circles){
	if(circles->total==0){
		Counter=0;
		ReStart++;
		if(LineNumber==2){
			if(isIntersect(LineOne,LineTwo)){
				closeWindow();
				LineOne=emptyLine(LineOne);
		        LineTwo=emptyLine(LineTwo);
			}
			LineNumber=0;
		}
		if(ReStart>=10)
			LineNumber=0;
	}else{
		ReStart=0;
		Counter++;
		if(Counter==1){
			LineNumber++;
			if(LineNumber==1){
				LineOne=initialLine(LineOne,circles);
			}
			else
				LineTwo=initialLine(LineTwo,circles);
		}else if(LineNumber==1 && Counter==2 ){
				LineOne=processLine(LineOne,circles);
		}else if(LineNumber==2 && Counter==2 ){
				LineTwo=processLine(LineTwo,circles);
		}
		if(LineNumber==1 && Counter>2 && inRange(LineOne,circles)){
			LineOne=processLine(LineOne,circles);
			if(inSlope(LineOne)){
				LineNumber=0;
			    LineOne=emptyLine(LineOne);
		        LineOne=emptyLine(LineTwo);
			    return;
			}
			
		}else if(LineNumber==2 && Counter>2 && inRange(LineTwo,circles)){
			LineTwo=processLine(LineTwo,circles);
		    if(inSlope(LineTwo)){
				LineNumber=0;
			    LineOne=emptyLine(LineOne);
		        LineTwo=emptyLine(LineTwo);
			    return;
			}
		}
	}
}
LINE Pattern::initialLine(LINE Line,CvSeq* circles){
	float* p=(float*)cvGetSeqElem(circles,0);
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
LINE Pattern::processLine(LINE Line,CvSeq* circles){
	float* p=(float*)cvGetSeqElem(circles,0);
	Line.PreviousPoint.x=Line.MiddlePoint.x;
	Line.PreviousPoint.y=Line.MiddlePoint.y;
	Line.MiddlePoint.x=Line.EndPoint.x;
	Line.MiddlePoint.y=Line.EndPoint.y;
	Line.EndPoint.x=cvRound(p[0]);
	Line.EndPoint.y=cvRound(p[1]);
	return Line;
}
LINE Pattern::emptyLine(LINE Line){
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
LINE Pattern::exchange(LINE Line){
	if(Line.StartPoint.x<Line.EndPoint.x){
		Line.PreviousPoint.x=Line.StartPoint.x;
		Line.PreviousPoint.y=Line.StartPoint.y;
		Line.MiddlePoint.x=Line.EndPoint.x;
		Line.MiddlePoint.y=Line.EndPoint.y;
	}else if(Line.StartPoint.x>Line.EndPoint.x){
		Line.PreviousPoint.x=Line.EndPoint.x;
		Line.PreviousPoint.y=Line.EndPoint.y;
		Line.MiddlePoint.x=Line.StartPoint.x;
		Line.MiddlePoint.y=Line.StartPoint.y;
	}
	return Line;
}
void Pattern::closeWindow(){//Alt+F4 to close windows
	keybd_event(VK_MENU,0,0,0);//win button
    keybd_event(VK_F4,0,0,0);//M button
    keybd_event(VK_F4,0,KEYEVENTF_KEYUP,0);
    keybd_event(VK_MENU,0,KEYEVENTF_KEYUP,0);
}
//This is to evalute 
bool Pattern::isIntersect(LINE LineOne,LINE LineTwo){
	if( ( Max(LineOne.StartPoint.x,LineOne.EndPoint.x) >= Min(LineTwo.StartPoint.x,LineTwo.EndPoint.x) )
        &&( Max(LineTwo.StartPoint.x,LineTwo.EndPoint.x) >= Min(LineOne.StartPoint.x,LineOne.EndPoint.x) )
        &&( Max(LineOne.StartPoint.y,LineOne.EndPoint.y) >= Min(LineTwo.StartPoint.y,LineTwo.EndPoint.y) )
        &&( Max(LineTwo.StartPoint.y,LineTwo.EndPoint.y) >= Min(LineOne.StartPoint.y,LineOne.EndPoint.y) )
		&&( mul(LineOne.StartPoint,LineTwo.EndPoint,LineTwo.StartPoint) * mul(LineTwo.EndPoint,LineOne.EndPoint,LineTwo.StartPoint)>=0)
		&&( mul(LineTwo.StartPoint,LineOne.EndPoint,LineOne.StartPoint) * mul(LineOne.EndPoint,LineTwo.EndPoint,LineOne.StartPoint)>=0)
		)
	{
		std::cout<<"true"<<std::endl;
		return true;
	}
	else {
		std::cout<<"false"<<std::endl;
		return false;
	}
}
//To make sure this is a stragiht line, I can no
bool Pattern::inSlope(LINE Line){
	if(Line.EndPoint.x-Line.MiddlePoint.x==0 || Line.MiddlePoint.x-Line.PreviousPoint.x==0)
		return false;
	if( abs(  (Line.EndPoint.y-Line.MiddlePoint.y)/(Line.EndPoint.x-Line.MiddlePoint.x) - (Line.MiddlePoint.y-Line.PreviousPoint.y)/(Line.MiddlePoint.x-Line.PreviousPoint.x)  )>1)
		return true;
	else 
		return false;
}
bool Pattern::inRange(LINE Line,CvSeq* circles){
	float* p=(float*)cvGetSeqElem(circles,0);
	if(abs(cvRound(p[0])-Line.EndPoint.x)>10 &&  abs(cvRound(p[1])-Line.EndPoint.y)>10)
		return true;
	else 
		return false;
}
int Pattern::VectorMul(LINE LineOne,LINE LineTwo){
    return ((LineOne.EndPoint.x-LineOne.StartPoint.x)*(LineTwo.EndPoint.y-LineTwo.StartPoint.y) - (LineOne.EndPoint.y-LineOne.StartPoint.y)*(LineTwo.EndPoint.x-LineTwo.StartPoint.x));
}
int Pattern::mul(POINT A,POINT B,POINT C){   
	return((A.x-C.x)*(B.y-C.y)-(B.x-C.x)*(A.y-C.y));   
}  
int Pattern::Max(int a,int b){
    return a > b ? a : b ;
}
int Pattern::Min(int a,int b){
    return a < b ? a : b ;
}
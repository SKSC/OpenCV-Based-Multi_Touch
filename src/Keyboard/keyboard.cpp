#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include "keyboard.h"

//This function convert given area into a certain virtual key code according to point lcation
int  Keyboard::ConvertPosit(POINT KeyPoint)
{
	//calculate which row in keyboard layout this point belongs to
    //The distance to the top is 35 pix, each row is 60 pix high
    //Can refer to document for clear details
	int Yaxis=(KeyPoint.y-35)/60;
	int vk;
	switch (Yaxis){
		//Virtual-key row one
		case 0 :
			if(KeyPoint.x<65 || KeyPoint.x>590){//column
				vk=0;
				break;
			}
			else{
				vk=PositionKeyboard[Yaxis][(KeyPoint.x-65)/34];
			    break;
			}
		//Virtual-key row two
		case 1:
			if(KeyPoint.x<65 || KeyPoint.x>590){
				vk=0;
				break;
			}
			else{
				vk=PositionKeyboard[Yaxis][(KeyPoint.x-65)/34];
			    break;
			}
		//Virtual-key row three
		case 2:
			if(KeyPoint.x<65 || KeyPoint.x>590){
				vk=0;
				break;
			}
			else{
				vk=PositionKeyboard[Yaxis][(KeyPoint.x-65)/34];
			    break;
			}
		//Virtual-key row four
		case 3:
			if(KeyPoint.x<65 || KeyPoint.x>585){
				vk=0;
				break;
			}
			else{
				vk=PositionKeyboard[Yaxis][(KeyPoint.x-65)/34];
			    break;
			}
		//Virtual-key row five
		case 4:
			if(KeyPoint.x<65 || KeyPoint.x>580){
				vk=0;
				break;
			}
			else{
				vk=PositionKeyboard[Yaxis][(KeyPoint.x-65)/34];
			    break;
			}
		//Virtual-key row six
		case 5:
			if(KeyPoint.x<65 || KeyPoint.x>580){
				vk=0;
				break;
			}
			else{
				vk=PositionKeyboard[Yaxis][(KeyPoint.x-65)/30];
			    break;
			}
		//if point is not in the area,set virtual key zero
		default:
			vk=0;
			break;
	}
	return vk;
}
//output different keys combination according to different fingers actions
void Keyboard::OutputKey(CvSeq* TouchBlobList){
	int Temp;
	//release keys when there is no signal input,divided into three situations
	if(TouchBlobList->total==0){
		if(VirtualKeys[2]!=0){
			keybd_event(VirtualKeys[2],0,KEYEVENTF_KEYUP,0);
			keybd_event(VirtualKeys[1],0,KEYEVENTF_KEYUP,0);
			keybd_event(VirtualKeys[0],0,KEYEVENTF_KEYUP,0);
		}else if(VirtualKeys[1]!=0){
			keybd_event(VirtualKeys[1],0,KEYEVENTF_KEYUP,0);
			keybd_event(VirtualKeys[0],0,KEYEVENTF_KEYUP,0);
		}else if(VirtualKeys[0]!=0){
			//when only one finger, signal will not be transferred until released
			keybd_event(VirtualKeys[0],0,0,0);
			keybd_event(VirtualKeys[0],0,KEYEVENTF_KEYUP,0);
		}
	}
	ClearVirtualKey();//clear array for next use
	for(int i=0;i<TouchBlobList->total && i<3;i++){
		float* p = (float*)cvGetSeqElem(TouchBlobList,i);
		POINT Point={cvRound(p[0]),cvRound(p[1])};
		VirtualKeys[i]=ConvertPosit(Point);
	}//evaluate
	if(VirtualKeys[0]!=0 && VirtualKeys[1]==0){
		//if just one button pressed,do nothing and certain action will be done when TouchBlobList equals to zero
	}else if(VirtualKeys[1]!=0 && VirtualKeys[2]==0){
        //if two button pressed, move letter buttons backward and funtion buttons forward,press them down and not release until all fingers up
		//nearly all vitual-key code value is between 8 and 47 except VK_MENU 
		if(((VirtualKeys[0]>=8 && VirtualKeys[0]<=47)|| VirtualKeys[0]==VK_LWIN)&& VirtualKeys[1]!=VK_MENU){
			keybd_event(VirtualKeys[0],0,0,0);
			keybd_event(VirtualKeys[1],0,0,0);
		}else if(((VirtualKeys[1]>=8 && VirtualKeys[1]<=47)||VirtualKeys[1]==VK_LWIN)&& VirtualKeys[0]!=VK_MENU){
			Temp=VirtualKeys[0];
			VirtualKeys[0]=VirtualKeys[1];
			VirtualKeys[1]=Temp;
			keybd_event(VirtualKeys[0],0,0,0);
			keybd_event(VirtualKeys[1],0,0,0);
		}
	}else if(VirtualKeys[2]!=0){
	//if three buttons pressed,move letter buttons backward and function forward, press them down and not release until all fingers up
	//nearly all vitual-key code value is between 8 and 47 except VK_MENU 
		if(VirtualKeys[0]>=8 && VirtualKeys[0]<=47){
			if(VirtualKeys[2]>=8 && VirtualKeys[2]<=47){
				Temp=VirtualKeys[1];
				VirtualKeys[1]=VirtualKeys[2];
		        VirtualKeys[2]=Temp;
			}
		}else if(VirtualKeys[2]>=8 && VirtualKeys[2]<=47){
			if(VirtualKeys[0]>=8 && VirtualKeys[0]<=47){
				Temp=VirtualKeys[2];
				VirtualKeys[2]=VirtualKeys[0];
		        VirtualKeys[0]=Temp;
			}
		}else{
			Temp=VirtualKeys[1];
			VirtualKeys[1]=VirtualKeys[0];
		    VirtualKeys[0]=Temp;
		}
		//press three buttons in turns
		keybd_event(VirtualKeys[0],0,0,0);
		keybd_event(VirtualKeys[1],0,0,0);
		keybd_event(VirtualKeys[2],0,0,0);
	}
}
//clear array for next use
void Keyboard::ClearVirtualKey(){
	for(int i=0;i<3;i++)
		VirtualKeys[i]=0;
}
//in case press wrong keys in gesture action, such virtual key values should be avoided
void Keyboard::UpKeys(){
	keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_CONTROL,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_LWIN,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_MENU,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_TAB,0,KEYEVENTF_KEYUP,0);
	keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);
}
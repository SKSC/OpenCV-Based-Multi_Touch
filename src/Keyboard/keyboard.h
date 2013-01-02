#ifndef KEYBOARD_MT
#define KEYBOARD_MT

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"

//Keyboard class can support up to three buttons combination 
class Keyboard{
private:
        int PositionKeyboard[6][17];
		int VirtualKeys[3];//Used to store three buttons' virtual key value
public:
    //This is standard keyboard structure
	    Keyboard(){
			int PositionTemp[6][17]={
				{0,VK_CONTROL,VK_LWIN,VK_MENU,0xDC,VK_SPACE,VK_SPACE,VK_SPACE,VK_MENU,VK_LWIN,VK_APPS,VK_CONTROL,VK_LEFT,VK_DOWN,VK_RIGHT,0,0},
	            {VK_SHIFT,VK_SHIFT,0x5A,0x58,0x43,0x56,0x42,0x4E,0x4D,0xBC,0xBE,0xBF,VK_SHIFT,VK_SHIFT,VK_UP,0},
				{VK_CAPITAL,VK_CAPITAL,0x41,0x53,0x44,0x46,0x47,0x48,0x4A,0x4B,0x4C,0xBA,0xDE,VK_RETURN,VK_RETURN,0,0},
				{VK_TAB,VK_TAB,0x51,0x57,0x45,0x52,0x54,0x59,0x55,0x49,0x4F,0x50,0xDB,0xDD,0xDC,0,0},
				{0xC0,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,VK_SUBTRACT,VK_ADD,VK_BACK,VK_BACK,0,0},
				{VK_ESCAPE,VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8,VK_F9,VK_F10,VK_F11,VK_F12,VK_NUMLOCK,VK_INSERT,VK_PAUSE,VK_DELETE}
			};//virtual key code
			for(int i=0;i<6;i++)//assign values from a temporary array to a private array
				for(int j=0;j<17;j++)
					PositionKeyboard[i][j]=PositionTemp[i][j];
		};
		void OutputKey(CvSeq* TouchBlobList);//keys output according to different area information
		int  ConvertPosit(POINT KeyPoint);//convert point location to virtual key code
		void ClearVirtualKey();//clear array for next use
		void UpKeys();//
};
#endif
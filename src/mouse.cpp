#include "stdafx.h"
#include "windows.h"
#include "mouse.h"
//Use windows keys shortcuts to do certain actions

//Click mouse left button
void Mouse::LeftClick() 
{
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//left button click down and up
}
//Click mouse right button
void Mouse::RightClick() 
{
	mouse_event (MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0 );//right button click down and up
}

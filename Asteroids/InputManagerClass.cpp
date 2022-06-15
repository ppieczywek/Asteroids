#pragma once
#include "stdafx.h"
#include "InputManagerClass.h"

bool InputManagerClass::Initialize() {




	KbdLeftButton = VK_LEFT;
	KbdRightButton = VK_RIGHT;
	KbdUpButton = VK_UP;
	KbdDownButton = VK_DOWN;
	KbdFireButton = VK_SPACE;
	KbdEnterButton = VK_RETURN;

	KbdExitButton = VK_ESCAPE;

	return true;
};

bool InputManagerClass::GetInput() {

	
	KeyPressedList.clear();

	if (GetAsyncKeyState(KbdLeftButton))
	{
		KeyPressedList.push_back(LEFT);
	}

	if (GetAsyncKeyState(KbdRightButton))
	{
		KeyPressedList.push_back(RIGHT);
	}

	if (GetAsyncKeyState(KbdUpButton))
	{
		KeyPressedList.push_back(UP);
	}

	if (GetAsyncKeyState(KbdDownButton))
	{
		KeyPressedList.push_back(DOWN);
	}

	if (GetAsyncKeyState(KbdFireButton))
	{
		KeyPressedList.push_back(FIRE);
	}

	if (GetAsyncKeyState(KbdExitButton))
	{
		KeyPressedList.push_back(EXIT);
	}

	if (GetAsyncKeyState(KbdEnterButton))
	{
		KeyPressedList.push_back(ENTER);
	}

    return true;

};
#pragma once
#include "stdafx.h"
#include <vector>
#include "GamepadClass.h"
using namespace std;

enum KeyDefinitions {UP, DOWN, LEFT, RIGHT, FIRE, EXIT, ENTER};

class InputManagerClass {

	GamepadClass					Gamepad;

	int								KbdLeftButton;
	int								KbdRightButton;
	int								KbdUpButton;
	int								KbdDownButton;
	int								KbdFireButton;
	int								KbdExitButton;
	int								KbdEnterButton;
	vector<KeyDefinitions>			KeyPressedList;

public:
	bool Initialize();
	bool GetInput();
	inline vector<KeyDefinitions>&			GetKeyPressedList();

};

vector<KeyDefinitions>&	InputManagerClass::GetKeyPressedList()
{
	return KeyPressedList;
};
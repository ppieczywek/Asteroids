#pragma once

#define XUSER_MAX_COUNT   4

#include "stdafx.h"
#include "Windows.h"
#include <XInput.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>
#pragma comment(lib, "XInput.lib")
using namespace std;

class GamepadClass
{
private:
int cId;
XINPUT_STATE state;
 
float deadzoneX;
float deadzoneY;
 
public:
GamepadClass() : deadzoneX(0.05f), deadzoneY(0.02f) {}
GamepadClass(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY) {}
 
float leftStickX;
float leftStickY;
float rightStickX;
float rightStickY;
float leftTrigger;
float rightTrigger;
 
int  GetPort();
XINPUT_GAMEPAD *GetState();
bool CheckConnection();
bool Refresh();
bool IsPressed(WORD);
};
 
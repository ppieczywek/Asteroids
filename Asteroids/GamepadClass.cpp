#pragma once
#include "stdafx.h"
#include "GamepadClass.h"


int GamepadClass::GetPort()
{
return cId + 1;
}
 
XINPUT_GAMEPAD *GamepadClass::GetState()
{
return &state.Gamepad;
}
 
bool GamepadClass::CheckConnection()
{
	int controllerId = -1;
 
	for (DWORD i = 0; i < 20 && controllerId == -1; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
 

		if (XInputGetState(i, &state) == ERROR_SUCCESS)
			controllerId = i;
	}
 
	cId = controllerId;
 
	return controllerId != -1;
}
 
// Returns false if the controller has been disconnected
bool GamepadClass::Refresh()
{
if (cId == -1)
CheckConnection();
 
if (cId != -1)
{
ZeroMemory(&state, sizeof(XINPUT_STATE));
if (XInputGetState(cId, &state) != ERROR_SUCCESS)
{
cId = -1;
return false;
}
 
float normLX = max(-1.0f, static_cast<float>(state.Gamepad.sThumbLX) / 32767.0f);
float normLY = max(-1.0f, static_cast<float>(state.Gamepad.sThumbLY) / 32767.0f);
 
leftStickX = ( abs(normLX) < deadzoneX ? 0.0f : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
leftStickY = (abs(normLY) < deadzoneY ? 0.0f : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));
 
if (deadzoneX > 0.0f) leftStickX *= 1.0f / (1.0f - deadzoneX);
if (deadzoneY > 0.0f) leftStickY *= 1.0f / (1.0f - deadzoneY);
 
float normRX = max(-1.0f, static_cast<float>(state.Gamepad.sThumbRX) / 32767.0f);
float normRY = max(-1.0f, static_cast<float>(state.Gamepad.sThumbRY) / 32767.0f);
 
rightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
rightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));
 
if (deadzoneX > 0.0f) rightStickX *= 1.0f / (1.0f - deadzoneX);
if (deadzoneY > 0.0f) rightStickY *= 1.0f / (1.0f - deadzoneY);
 
leftTrigger = static_cast<float>(state.Gamepad.bLeftTrigger) / 255.0f;
rightTrigger = static_cast<float>(state.Gamepad.bRightTrigger) / 255.0f;
 
return true;
}
return false;
}
 
bool GamepadClass::IsPressed(WORD button)
{
return (state.Gamepad.wButtons & button) != 0;
}
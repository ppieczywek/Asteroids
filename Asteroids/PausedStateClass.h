#pragma once
#include "stdafx.h"
#include "CommonGameState.h"
#include "GameEngineClass.h"
#include <vector>
#include <string>

class PausedStateClass : public CommonGameState {
public:
			
	bool	WasExitPressed;
	void	DrawStateSpecific(Rectangle2D& Bounds);
	void	Initialize(GameEngineClass &Engine);
	void	Cleanup(GameEngineClass &Engine) {};

	void	Pause() {};
	void	Resume() {};

	void	Update(GameEngineClass &Engine);

			PausedStateClass() { };	
};

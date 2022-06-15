#pragma once
#include "stdafx.h"
#include "GameStateClass.h"
#include "GameEngineClass.h"
#include <vector>
#include <string>

class CommonGameState : public GameStateClass 
{
public:

	static int		TotalScore;
	
	virtual	void	Initialize(GameEngineClass &Engine) {};
	virtual	void	Cleanup(GameEngineClass &Engine) {};

	virtual	void	Pause() {};
	virtual	void	Resume() {};

	virtual	void	Update(GameEngineClass &Engine) {};
	virtual	void	Render(GameEngineClass &Engine);

	virtual	void	DrawStateSpecific(Rectangle2D& Bounds) {};
	virtual	void	TeleportObjects(GameObjectManagerClass& ObjectManager, Rectangle2D& Bounds);

					CommonGameState() { };	
};

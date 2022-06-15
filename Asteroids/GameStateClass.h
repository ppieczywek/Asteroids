#pragma once
#include "stdafx.h"
#include "GameEngineClass.h"
#include <vector>
using namespace std;


class GameStateClass {
public:
	virtual void Initialize(GameEngineClass &Engine) = 0;
	virtual void Cleanup(GameEngineClass &Engine) = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void Render(GameEngineClass &Engine) = 0;
	virtual void Update(GameEngineClass &Engine) = 0;

	void ChangeState(GameEngineClass &Engine, shared_ptr<GameStateClass> State) {
		Engine.ChangeState(State);
	};

	GameStateClass() {};

};
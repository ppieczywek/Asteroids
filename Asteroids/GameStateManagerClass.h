#pragma once
#include "stdafx.h"
#include "MainStateClass.h"
#include <vector>
using namespace std;

//class CGameState;

class GameStateManagerClass
{
public:

	//void Cleanup();

	void ChangeState(shared_ptr<GameStateClass> State, GameEngineClass &Engine);
	void PushState(shared_ptr<GameStateClass> State, GameEngineClass &Engine);
	void PopState(GameEngineClass &Engine);

	void Initialize(GameEngineClass &Engine);
	void Cleanup(GameEngineClass &Engine);

	void HandleEvents(GameEngineClass &Engine);
	void Update(GameEngineClass &Engine);
	void Render(GameEngineClass &Engine);

	bool Running() { return m_running; }
	void Quit() { m_running = false; }
	

private:
	// the stack of states
	vector<shared_ptr<GameStateClass>> StatesStack;

	bool m_running;
	bool m_fullscreen;
};
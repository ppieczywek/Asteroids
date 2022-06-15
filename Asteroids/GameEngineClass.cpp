#pragma once
#include "stdafx.h"
#include "dynamic_array.h"
#include "Polygon2.h"
#include "GameEngineClass.h"
#include "GameStartClass.h"
#include "MainStateClass.h"
#include "PausedStateClass.h"

bool GameEngineClass::Initialize(int nCmdShow) {
		
    if (!GameMainWindow.Create(L"Asteroids alpha", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,0,200,20,1024,768)) //1024 768
	{
		return false;
    };

	if (! OGLContext.Create(GameMainWindow.Window()) ) 
	{
		return false;
	};

	ShowWindow(GameMainWindow.Window(), nCmdShow); 
	InputManager.Initialize();
	GameTimer.Init();
		
	return true;
};

void GameEngineClass::Run()
{
	
	auto NewState = shared_ptr<GameStateClass> (new GameStartClass());
	ChangeState( NewState );
	NewState.reset();

	double TimeStamp1 = 0.0f;
	double TimeStamp2 = 0.0f;
	double Interval = 0.0f;
	double Fps = static_cast<double>(PhysicsManager.WorldProperties.GetFrameRate()) + 6;
	
	while(TRUE)
	{
		if(PeekMessage(&WindowMessage, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&WindowMessage);
			DispatchMessage(&WindowMessage);

            if(WindowMessage.message == WM_QUIT)
            break;
		}
					
			TimeStamp1 = GameTimer.GetPerformanceCounterTime();
			
			Update();
			
			Render();
			
			SwapBuffers(OGLContext.GetHDC());

			do
			{
				TimeStamp2 = GameTimer.GetPerformanceCounterTime();
			} 
			while( 1.0 / (TimeStamp2 - TimeStamp1) > Fps);

	}
};


void GameEngineClass::CleanupState() {
	while ( !StatesStack.empty() ) {
		StatesStack.back()->Cleanup(*this);
		StatesStack.pop_back();
	};
};

void GameEngineClass::ChangeState(shared_ptr<GameStateClass> &State) {
	if ( !StatesStack.empty() ) {
		StatesStack.back()->Cleanup(*this);
		StatesStack.pop_back();
	};
	StatesStack.push_back(State);
	StatesStack.back()->Initialize(*this);
};

void GameEngineClass::PushState(shared_ptr<GameStateClass> &State) {
	// pause current state
	if ( !StatesStack.empty() ) {
		StatesStack.back()->Pause();
	};

 	StatesStack.push_back(State);
	StatesStack.back()->Initialize(*this);
};	

void GameEngineClass::InitializeState() {
	if ( !StatesStack.empty() ) {
		StatesStack.back()->Initialize(*this);
	};
};

void GameEngineClass::PopState() {
	// cleanup the current state
	if ( !StatesStack.empty() ) {
		StatesStack.back()->Cleanup(*this);
		StatesStack.pop_back();
	};

	// resume previous state
	if ( !StatesStack.empty() ) {
		StatesStack.back()->Resume();
	};
};

void GameEngineClass::Update() {
	// let the state update the game
	if ( !StatesStack.empty() ) {
		StatesStack.back()->Update(*this);
	};
};

void GameEngineClass::Render() {
	// let the state draw the screen
	if ( !StatesStack.empty() ) {
		StatesStack.back()->Render(*this);
	};
};


#pragma once
#include "stdafx.h"
#include "MainWindow.h"
#include "OpenGLContext.h"
#include "LogicsManagerClass.h"
#include "CollisionManagerClass.h"
#include "WorldPropertiesClass.h"
#include "ConstraintManagerClass.h"
#include "GameObjectManagerClass.h"
#include "InputManagerClass.h"
#include "PhysicsManagerClass.h"
#include "CameraClass.h"
#include "CTime.h"
#include <set>
#include <vector>
using namespace std;

class GameStateClass;

class GameEngineClass {

	MainWindow					GameMainWindow;
	OpenGLContext				OGLContext;
	MSG							WindowMessage;

public:
	
	friend class				MainStateClass;
		
	GameObjectManagerClass		ObjectManager;
	PhysicsManagerClass			PhysicsManager;
	LogicsManagerClass			LogicsManager;
	CameraClass					Camera;
	InputManagerClass			InputManager;

	CTime						GameTimer;
	
	bool						Initialize(int nCmdShow);
	void						Run();
	void						Close() {CleanupState();};
	
	void						ChangeState(shared_ptr<GameStateClass> &State);
	void						PushState(shared_ptr<GameStateClass> &State);
	void						PopState();

	void						InitializeState();
	void						CleanupState();

	void						Update();
	void						Render();

	
	bool Running() { return m_running; }
	void Quit() { m_running = false; }
	

private:
	vector<shared_ptr<GameStateClass>> StatesStack;

	bool m_running;
	bool m_fullscreen;
};

 
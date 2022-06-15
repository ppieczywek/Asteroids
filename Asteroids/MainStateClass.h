#pragma once
#include "stdafx.h"
#include "CommonGameState.h"
#include "GameEngineClass.h"
#include <vector>
#include <string>

class MainStateClass : public CommonGameState {
public:
	
	Vector3				AsteroidColors[4];
	Vector2				PlayerPreviousPosition;
	CTime				EventTimer;
	double				FpsCnt;
	float				PlayersLifes;
	double				TimeStamp;
	double				TimeOfDeath;
	long long			PlayersId;
	vector<long long>	IconSlots;
	bool				PlayerIsDestroyed;
	bool				WasExitPressed;

	void				CheckPlayerStatus(GameObjectManagerClass&, Rectangle2D&);
	
	void				InsertAsteroid(GameObjectManagerClass&, Rectangle2D&);
	void				CountScore(GameObjectManagerClass&);
		
	int					AsteroidCounter;

	void				Initialize(GameEngineClass &Engine);
	void				Cleanup(GameEngineClass &Engine);

	void				Pause() {};
	void				Resume();

	void				Update(GameEngineClass &Engine);
	virtual void		DrawStateSpecific(Rectangle2D& Bounds);
	
						MainStateClass() { };	
};

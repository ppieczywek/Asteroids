#pragma once
#include "stdafx.h"
#include "CommonGameState.h"
#include "GameEngineClass.h"
#include <vector>
#include <string>

class GameStartClass : public CommonGameState 
{
public:
	
	CTime				EventTimer;
	double				StartTimeStamp;
	double				TerminateTimeStamp;
	double				CurrentTimeStamp;
	double				FadeTime;
	double				FadeInValue;
	double				FadeOutValue;
	double				Interval; 
	bool				InitializeState;
	bool				TerminateState;
	
	virtual void		Initialize(GameEngineClass &Engine);
	virtual void		Cleanup(GameEngineClass &Engine) {};

	virtual void		Pause() {};
	virtual void		Resume() {};

	virtual void		Update(GameEngineClass &Engine);
	virtual void		DrawStateSpecific(Rectangle2D& Bounds);
	
						GameStartClass() { };	
};

#pragma once
#include "stdafx.h"
#include "GameOverState.h"
#include "MainStateClass.h"

void GameOverState::Initialize(GameEngineClass &Engine) 
{
	EventTimer.Init();
	
	StartTimeStamp = EventTimer.GetPerformanceCounterTime();

	InitializeState = true;
	TerminateState = false;
	FadeTime = 3.0f;
	FadeOutValue = 0.0f;
};

void GameOverState::Update(GameEngineClass &Engine) 
{

	Rectangle2D	Bounds = Engine.PhysicsManager.WorldProperties.GetWorldBox();
	GameObjectManagerClass &ObjectManager = Engine.ObjectManager;

	Engine.InputManager.GetInput();
		
	Engine.PhysicsManager.Update(ObjectManager);

	Engine.LogicsManager.Update(Engine.PhysicsManager.CollisionManager.GetPhysicalContactList(),
						Engine.PhysicsManager.CollisionManager.GetPhysicalCollisionNumber(),
						Engine.PhysicsManager.CollisionManager.GetLogicalContactList(),
						Engine.PhysicsManager.CollisionManager.GetLogicalCollisionNumber(),
						Engine.InputManager.GetKeyPressedList(),
						Engine.ObjectManager );	
				
	TeleportObjects(ObjectManager, Bounds);
		
	Engine.Camera.UpdateCameraPosition();

	ObjectManager.Update();

	vector<KeyDefinitions>& KeyList = Engine.InputManager.GetKeyPressedList();
	
	auto Iterator = find(KeyList.begin(), KeyList.end(), ENTER);
	if(Iterator != KeyList.end() && InitializeState == false && TerminateState != true)
	{
		TerminateState = true;
		TerminateTimeStamp = EventTimer.GetPerformanceCounterTime();
	}

	if(InitializeState == true)
	{
		CurrentTimeStamp = EventTimer.GetPerformanceCounterTime();
		Interval = CurrentTimeStamp - StartTimeStamp;

		if( Interval < FadeTime )
		{
			FadeInValue = static_cast<float>(Interval / FadeTime);	
		}
		else
		{
			InitializeState = false;
			FadeInValue = 1.0f;	
		}
	}

	auto cLogicsList = Engine.ObjectManager.GetcLogicsList();
	if (!cLogicsList.empty())
	{
		for (size_t kk = 0; kk < cLogicsList.size(); kk++)
		{
			if (cLogicsList[kk]->GetEntityType() == AsteroidE)
			{
				auto cLogics = dynamic_pointer_cast<AsteroidLogics>(cLogicsList[kk]);
				cLogics->SetEnergy(0);
			}
		}
	}

	
	if(TerminateState == true)
	{
		CurrentTimeStamp = EventTimer.GetPerformanceCounterTime();
		Interval = CurrentTimeStamp - TerminateTimeStamp;

		if( Interval > FadeTime )
		{
			auto NewState = shared_ptr<GameStateClass> (new MainStateClass());
			Engine.ChangeState(NewState);
		}
		else
		{
			FadeOutValue = static_cast<float>(Interval / FadeTime);	
		}
	}
};


void GameOverState::DrawStateSpecific(Rectangle2D& Bounds)
{	
	Vector2 WorldCenter( (Bounds.min.x + Bounds.max.x)/2.0f, (Bounds.min.y + Bounds.max.y)/2.0f );
	
	GLfloat BcgColor = 0.1f;
	GLfloat BcgTranspercy = 0.05f +  0.3f * static_cast<GLfloat>( FadeInValue - FadeOutValue );
	
	glColor4f( BcgColor, BcgColor, BcgColor, BcgTranspercy);

	glBegin(GL_QUADS);
		glVertex3f( Bounds.min.x ,  Bounds.min.y  , 10.0f );
		glVertex3f( Bounds.min.x ,  Bounds.max.y  , 10.0f );
		glVertex3f( Bounds.max.x ,  Bounds.max.y  , 10.0f );
		glVertex3f( Bounds.max.x ,  Bounds.min.y  , 10.0f );
	glEnd();
			

	auto TextOffset = GetStringLength("Game Over");
	TextOffset/=2.0f;
	float Scale = 10.0f;

	GLfloat TextColor = 0.97f;	
	GLfloat TextTranspercy = 0.05f + 0.94f * static_cast<GLfloat>(FadeInValue - FadeOutValue);

	glColor4f(TextColor, TextColor, TextColor, TextTranspercy);
	glPushMatrix();
		glTranslatef( (WorldCenter.x - TextOffset*Scale) , WorldCenter.y + 10.0f,13.0f);	
		glScalef(Scale, Scale, 0.0f);
		PrintString( "Game Over" );
	glPopMatrix();
	

	TextOffset = GetStringLength("Press Enter to restart");
	TextOffset/=2.0f;
	Scale = 3.5f;
	
	glColor4f(TextColor, TextColor, TextColor, TextTranspercy);
	glPushMatrix();
		glTranslatef( (WorldCenter.x - TextOffset*Scale) , WorldCenter.y,13.0f);	
		glScalef(Scale, Scale, 0.0f);
		PrintString( "Press Enter to restart" );
	glPopMatrix();
	
	
	TextOffset = GetStringLength("Total score");
	TextOffset/=2.0f;
	Scale = 2.5f;
	glColor4f(TextColor, TextColor, TextColor, TextTranspercy);
	glPushMatrix();
		glTranslatef( (WorldCenter.x - TextOffset*Scale) , WorldCenter.y - 4.0f,13.0f);	
		glScalef(Scale, Scale, 0.0f);
		PrintString( "Total score" );
	glPopMatrix();


	string ScoreValue = to_string(static_cast<long long>(TotalScore));
	TextOffset = GetStringLength(ScoreValue.c_str());
	TextOffset/=2.0f;
	Scale = 3.5f;
	glColor4f(TextColor, TextColor, TextColor, TextTranspercy);
	glPushMatrix();
		glTranslatef( (WorldCenter.x - TextOffset*Scale) , WorldCenter.y - 9.0f,13.0f);	
		glScalef(Scale, Scale, 0.0f);
		PrintString( ScoreValue.c_str() );
	glPopMatrix();

};
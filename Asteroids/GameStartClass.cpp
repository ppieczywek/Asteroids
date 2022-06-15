#pragma once
#include "stdafx.h"
#include "GameStartClass.h"
#include "MainStateClass.h"

void GameStartClass::Initialize(GameEngineClass &Engine) 
{
	EventTimer.Init();
	shared_ptr<Entity>	BodyPtr = nullptr;
	Rectangle2D			scr;
	//Rectangle2D			cBox;

	Engine.Camera.SetAlpha(0.03f);
	Engine.Camera.SetFocusAtFixedPoint(Vector2(60.0f,45.0f));
	Engine.Camera.SetWidth(80.0f);
		
	scr.min.x = 0.0f;
	scr.max.x =  120.0f;
	scr.min.y = 0.0f;
	scr.max.y = 120.0f / Engine.Camera.GetAspecRatio();
	//cBox = scr;

	Engine.PhysicsManager.CollisionManager.Grid.SetWorldBox(scr);
	Engine.PhysicsManager.WorldProperties.SetWorldBox(scr);
	Engine.PhysicsManager.WorldProperties.SetGravityState(false);
	Engine.PhysicsManager.WorldProperties.SetDragState(true);
	Engine.PhysicsManager.WorldProperties.SetFrictionState(false);
		
	float x; 
	float y;
	float StarSize;	
	float StarLayer;	

	for(auto ii = 0; ii<2000; ii++) 
	{
		x = ((float)rand()/(float)RAND_MAX)*scr.max.x;
		y = ((float)rand()/(float)RAND_MAX)*scr.max.y;
		
		StarLayer = ((float)rand() / (float)RAND_MAX) * 60.0f + 1.0f;
		StarSize = pow(0.96f, StarLayer) * 1.65f;

		BodyPtr = shared_ptr<Entity> (new Star(StarSize));
		BodyPtr->p_cBody->SetTransform(Vector2(x,y));
		BodyPtr->p_cRender->SetLayer(-1.0f*StarLayer);
		Engine.ObjectManager.AddEntity( BodyPtr );
	}

	StartTimeStamp = EventTimer.GetPerformanceCounterTime();

	InitializeState = true;
	TerminateState = false;
	FadeTime = 3.0f;
	
	FadeOutValue = 0.0f;
};

void GameStartClass::Update(GameEngineClass &Engine) 
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
		};
	}
};


void GameStartClass::DrawStateSpecific(Rectangle2D& Bounds)
{	
	Vector2 WorldCenter( (Bounds.min.x + Bounds.max.x)/2.0f, (Bounds.min.y + Bounds.max.y)/2.0f );
	
	GLfloat BcgColor = 0.1f * static_cast<GLfloat>(FadeInValue);
	GLfloat BcgTranspercy = 1.0f - 0.75f * static_cast<GLfloat>( FadeInValue) - 0.24f * static_cast<GLfloat>(FadeOutValue);

					

	glColor4f( BcgColor, BcgColor, BcgColor, BcgTranspercy);

	glBegin(GL_QUADS);
		glVertex3f( Bounds.min.x ,  Bounds.min.y  , 10.0f );
		glVertex3f( Bounds.min.x ,  Bounds.max.y  , 10.0f );
		glVertex3f( Bounds.max.x ,  Bounds.max.y  , 10.0f );
		glVertex3f( Bounds.max.x ,  Bounds.min.y  , 10.0f );
	glEnd();
			
	auto TextOffset = GetStringLength("Asteroids");
	TextOffset/=2.0f;
	float Scale = 15.0f;


	GLfloat TextColor = 0.05f + 0.95f * static_cast<GLfloat>(FadeInValue);

	GLfloat TextTranspercy = 0.05f  + 0.94f - 0.94f * static_cast<GLfloat>(FadeOutValue);

	glColor4f(TextColor, TextColor, TextColor, TextTranspercy);
	glPushMatrix();
		glTranslatef( (WorldCenter.x - TextOffset*Scale) , WorldCenter.y,13.0f);	
		glScalef(Scale, Scale, 0.0f);
		
		glPushMatrix();
		
		PrintString( "Asteroids" );
		glPopMatrix();
	glPopMatrix();


	TextOffset = GetStringLength("Press Enter to start");
	TextOffset/=2.0f;
	Scale = 3.5f;

	glColor4f(TextColor, TextColor, TextColor, TextTranspercy);
	glPushMatrix();
		glTranslatef( (WorldCenter.x - TextOffset*Scale) , WorldCenter.y - (10.0f),13.0f);	
		glScalef(Scale, Scale, 0.0f);
		
		glPushMatrix();
		
		PrintString( "Press Enter to start" );
		glPopMatrix();
	glPopMatrix();

	TextOffset = GetStringLength("use keyboard arrows and space bar to play");
	TextOffset /= 2.0f;
	Scale = 2.0f;

	glColor4f(TextColor, TextColor, TextColor, TextTranspercy);
	glPushMatrix();
	glTranslatef((WorldCenter.x - TextOffset * Scale), WorldCenter.y - (12.0f), 13.0f);
	glScalef(Scale, Scale, 0.0f);

	glPushMatrix();

	PrintString("use keyboard arrows and space bar to play");
	glPopMatrix();
	glPopMatrix();


};
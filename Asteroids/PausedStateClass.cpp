#pragma once
#include "stdafx.h"
#include "PausedStateClass.h"

void PausedStateClass::Initialize(GameEngineClass &Engine) 
{
	WasExitPressed = true;
};

void PausedStateClass::Update(GameEngineClass &Engine) 
{
	Engine.InputManager.GetInput();
	Engine.Camera.UpdateCameraPosition();

	vector<KeyDefinitions>& KeyList = Engine.InputManager.GetKeyPressedList();
	
	auto Iterator = find(KeyList.begin(), KeyList.end(), EXIT);
	if(Iterator != KeyList.end())
	{
		if(!WasExitPressed)
		{
			Engine.PopState();
		}
	}
	else
	{
		WasExitPressed = false;
	}
};


void PausedStateClass::DrawStateSpecific(Rectangle2D& Bounds)
{	
	Vector2 WorldCenter( (Bounds.min.x + Bounds.max.x)/2.0f, (Bounds.min.y + Bounds.max.y)/2.0f );
	
	glColor4f(255.0f/256.0f, 246.0f/256.0f, 113.0f/256.0f, 0.9f);
	glPushMatrix();
		glTranslatef( Bounds.min.x + 5.0f, Bounds.max.y - 6.0f,9.0f);
		glScalef(2.7f, 2.7f, 0.0f);
		string ScoreInString = to_string(static_cast<long long>(TotalScore));
		PrintString( ScoreInString.c_str() );
	glPopMatrix();


	glBegin(GL_QUADS);
		glColor4f(0.1f, 0.1f, 0.1f, 0.23f);
		glVertex3f( Bounds.min.x ,  Bounds.min.y  , 10.0f );

		glColor4f(0.1f, 0.1f, 0.1f, 0.23f);
		glVertex3f( Bounds.min.x ,  Bounds.max.y  , 10.0f );

		glColor4f(0.1f, 0.1f, 0.1f, 0.23f);
		glVertex3f( Bounds.max.x ,  Bounds.max.y  , 10.0f );

		glColor4f(0.1f, 0.1f, 0.1f, 0.23f);
		glVertex3f( Bounds.max.x ,  Bounds.min.y  , 10.0f );
	glEnd();

		
	auto TextOffset = GetStringLength("Game Paused");
	TextOffset/=2.0f;

	glColor4f(0.97f, 0.97f, 0.97f, 1.0f);
	glPushMatrix();
		glTranslatef( (WorldCenter.x - TextOffset*5.0f) , WorldCenter.y,13.0f);	
		glScalef(5.0f, 5.0f, 0.0f);
		
		glPushMatrix();
		
		PrintString( "Game Paused" );
		glPopMatrix();
	glPopMatrix();

};
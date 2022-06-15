#pragma once
#include "stdafx.h"
#include "Star.h"
#include "GameEngineClass.h"

Star::Star(float Scale)
{
	p_cBody = shared_ptr<cBody>(new cBody());		
	p_cBody->SetBodyType(kinematic2D);

	vector<Vector2> Vertices;
	Vertices.push_back( Vector2(  -0.5f,  0.5f ) * Scale);
	Vertices.push_back( Vector2(   0.5f,  0.5f ) * Scale);
	Vertices.push_back( Vector2(   0.5f, -0.5f ) * Scale);
	Vertices.push_back( Vector2(  -0.5f, -0.5f ) * Scale);
	Polygon2 Shape = Polygon2(Vertices);
	p_cBody->AddPolygon(Shape);
	p_cBody->SetDensity(0.4f);
	p_cBody->SetAngularMotion(false);
	p_cBody->SetAngularDamping(0.0f);
	p_cBody->SetLinearDamping(0.0f);
		

	auto IX = ((float)rand()/(float)RAND_MAX)*0.2f - 0.05f;
	auto IY = ((float)rand()/(float)RAND_MAX)*0.2f - 0.05f;
		
	p_cBody->ApplyLinearVelocity( Vector2(IX,IY));
	p_cBody->SetCollisionType(None);
	
	p_cRender = shared_ptr<cRender>(new cRender());
	p_cRender->SetVisible(true);

	auto R = (((float)rand()/(float)RAND_MAX)*(254.0f - 199.0f) + 199.0f)/256.0f;
	auto G = (((float)rand()/(float)RAND_MAX)*(253.0f - 249.0f) + 249.0f)/256.0f;
	auto B = (((float)rand()/(float)RAND_MAX)*(254.0f - 199.0f) + 199.0f)/256.0f;

	auto Color = Vector3(R, G, B);
	auto Transpercy = ((float)rand() / (float)RAND_MAX) * 0.15f + 0.1f;
	p_cRender->SetBodyColor( Color);
	p_cRender->SetTranspercyValue(Transpercy);
	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());
	p_cLogics =  shared_ptr<cLogics>(new StarLogics(Color));
};


Star::~Star()
{

};


StarLogics::StarLogics(Vector3 Color)
{
	EntityT = StarE;
	IsHandlingContacts = false;
	IsHandlingInputs = false;
	CurrentAction = "BASE";

	BodyColor = Color;
	PhaseShift = static_cast<float>( (rand() % 60 + 1) );
	DeviationAmplitude = ((float)rand()/(float)RAND_MAX)*0.12f + 0.1f; 
	DeviationPeriod = ((float)rand()/(float)RAND_MAX)*40.0f + 30.0f;
	Time = 0.0f;
}; 


StarLogics::~StarLogics()
{
	
}; 

	
void StarLogics::HandleContacts(vector<ContactEvent>& ContactEventList, GameObjectManagerClass& ObjectManager)
{

};


void StarLogics::HandleInputs(vector<KeyDefinitions>& KeyList, GameObjectManagerClass& ObjectManager)
{

};


void StarLogics::Update(GameObjectManagerClass& ObjectManager)
{
	shared_ptr<Entity>& StarEntity = ObjectManager.GetEntity(ID);

	if(StarEntity != nullptr)
	{
		float x = (Time+PhaseShift)/DeviationPeriod * tau;	 	
		auto CurrentDeviation = DeviationAmplitude * sin(x);

		Time++;
		if(Time > DeviationPeriod) Time = 0.0f;

		auto CurrentColorR = BodyColor.x + CurrentDeviation;
		if(CurrentColorR < 0.0f)
		{
			CurrentColorR = 0.0f;
		}
		else if(CurrentColorR > 1.0f)
		{
			CurrentColorR = 1.0f;
		}

		auto CurrentColorG = BodyColor.y + CurrentDeviation;
		if(CurrentColorG < 0.0f)
		{
			CurrentColorG = 0.0f;
		}
		else if(CurrentColorG > 1.0f)
		{
			CurrentColorG = 1.0f;
		}

		auto CurrentColorB = BodyColor.z + CurrentDeviation;
		if(CurrentColorB < 0.0f)
		{
			CurrentColorB = 0.0f;
		}
		else if(CurrentColorB > 1.0f)
		{
			CurrentColorB = 1.0f;
		}	
		StarEntity->p_cRender->SetBodyColor( Vector3(CurrentColorR, CurrentColorG, CurrentColorB));
	}
};

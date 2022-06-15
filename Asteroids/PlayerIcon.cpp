#pragma once
#include "stdafx.h"
#include "PlayerIcon.h"
#include "GameEngineClass.h"

PlayerIcon::PlayerIcon() {
		
	p_cBody = shared_ptr<cBody>(new cBody());		
	p_cBody->SetBodyType(kinematic2D);
	p_cBody->SetCollisionType(None);
	vector<Vector2> RocketVertices;

	float ScaleFactor = 1.75f;
	RocketVertices.push_back( Vector2( -0.2f,  0.9633f ) * ScaleFactor);
	RocketVertices.push_back( Vector2(  0.2f,  0.9633f ) * ScaleFactor);
	RocketVertices.push_back( Vector2(  0.2f,  -0.336f )  * ScaleFactor);
	RocketVertices.push_back( Vector2( -0.2f,  -0.336f )  * ScaleFactor);


	Polygon2 RocketShape1 = Polygon2(RocketVertices);
	p_cBody->AddPolygon(RocketShape1);
	RocketVertices.clear();
	
	RocketVertices.push_back( Vector2( -0.6f,  0.1633f ) * ScaleFactor);
	RocketVertices.push_back( Vector2( -0.2f,  0.1633f )  * ScaleFactor);
	RocketVertices.push_back( Vector2( -0.2f, -0.536f ) * ScaleFactor);
	RocketVertices.push_back( Vector2( -0.6f, -0.536f ) * ScaleFactor);

	Polygon2 RocketShape2 = Polygon2(RocketVertices);
	p_cBody->AddPolygon(RocketShape2);
	RocketVertices.clear();
	
	
	RocketVertices.push_back( Vector2( 0.2f,  0.1633f )  * ScaleFactor);
	RocketVertices.push_back( Vector2( 0.6f,  0.1633f ) * ScaleFactor);
	RocketVertices.push_back( Vector2( 0.6f, -0.536f ) * ScaleFactor);
	RocketVertices.push_back( Vector2( 0.2f, -0.536f ) * ScaleFactor);

	Polygon2 RocketShape3 = Polygon2(RocketVertices);
	p_cBody->AddPolygon(RocketShape3);



	p_cBody->SetDensity(0.01f);

	p_cBody->SetRestitutionCoefficient(2.0f);

	p_cBody->SetAngularMotion(true);
	p_cBody->SetAngularDamping(0.0f);
	p_cBody->SetLinearDamping(0.7f);

	p_cBody->SetCollisionType(None);

	Vector3 BodyColor = Vector3(255.0f/256.0f, 246.0f/256.0f, 113.0f/256.0f);
	Vector3 LineColor = BodyColor * 0.25f;
	p_cRender = shared_ptr<cRender>(new cRender());
	p_cRender->SetVisible(true);
	p_cRender->SetBodyColor( BodyColor );
	p_cRender->SetLineColor( LineColor );
	p_cRender->SetTranspercyValue(0.0f);
	p_cRender->SetLayer(10.0f);

	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());
	p_cLogics =  shared_ptr<cLogics>(new PlayerIconLogics());
	auto thisEntity = IconE;
	p_cLogics->SetEntityType(thisEntity);
};

PlayerIcon::~PlayerIcon() 
{
	
};


PlayerIconLogics::PlayerIconLogics()
{
	Initialize = true;
	InitializeTime = 120.f;
};


PlayerIconLogics::~PlayerIconLogics()
{
	
};


void PlayerIconLogics::Update(GameObjectManagerClass& ObjectManager)
{
	if(Initialize) 
	{		
		shared_ptr<Entity>& PlayerIconEntity = ObjectManager.GetEntity(ID);
		if(PlayerIconEntity != nullptr)
		{				
			InitializeTime--;
			if(InitializeTime > 0.0)
			{	
				float Value = 0.8f -  0.79f*(InitializeTime / 120.0f);
				PlayerIconEntity->p_cRender->SetTranspercyValue( Value );
			}
			else
			{
				Initialize = false;
				PlayerIconEntity->p_cRender->SetTranspercyValue( 0.8f );
			}
		}
	}
};
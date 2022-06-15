#pragma once
#include "stdafx.h"
#include "Debris.h"
#include "GameEngineClass.h"

Debris::Debris(float Width, float Height, float Mass, float Life)
{
	p_cBody = shared_ptr<cBody>(new cBody());		
	p_cBody->SetBodyType(dynamic2D);

	vector<Vector2> DebrisVertices;
	
	DebrisVertices.push_back( Vector2( -Width/2.0f,  Height/2.0f ));
	DebrisVertices.push_back( Vector2(  Width/2.0f,  Height/2.0f ));
	DebrisVertices.push_back( Vector2(  Width/2.0f, -Height/2.0f ));
	DebrisVertices.push_back( Vector2( -Width/2.0f, -Height/2.0f ));
	
	Polygon2 DebrisShape = Polygon2(DebrisVertices);
	p_cBody->AddPolygon(DebrisShape);
	
	p_cBody->SetDensity(Mass);
	p_cBody->SetAngularMotion(true);
	p_cBody->SetAngularDamping(0.0f);
	p_cBody->SetLinearDamping(0.0f);
	
		
	p_cBody->SetCollisionType(None);
	
	p_cRender = shared_ptr<cRender>(new cRender());
	p_cRender->SetVisible(true);
	p_cRender->SetDrawLines(true);
		
	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());

	p_cLogics =  shared_ptr<cLogics>(new DebrisLogics(Life));
};


Debris::~Debris() 
{

};



DebrisLogics::DebrisLogics(float L)
{
	EntityT = Junk;
	IsHandlingContacts = false;
	IsHandlingInputs = false;
	TotalLifeSpan = LifeSpan = L;
	CurrentAction = "BASE";
	
}; 


DebrisLogics::~DebrisLogics()
{
	
}; 

	
void DebrisLogics::HandleContacts(vector<ContactEvent>& ContactEventList, GameObjectManagerClass& ObjectManager)
{


};


void DebrisLogics::HandleInputs(vector<KeyDefinitions>& KeyList, GameObjectManagerClass& ObjectManager)
{


};



void DebrisLogics::Update(GameObjectManagerClass& ObjectManager)
{

	if(LifeSpan > 0.0f)
	{	
		LifeSpan--;
	
		shared_ptr<Entity>& DebrisEntity =	ObjectManager.GetEntity( ID );
		if(DebrisEntity!=nullptr) 
		{
			float HalfLife = TotalLifeSpan * 0.5f;
			auto CurrentValue = LifeSpan;
			if(CurrentValue < HalfLife)
			{
				auto Value = CurrentValue / HalfLife;
				DebrisEntity->p_cRender->SetTranspercyValue( Value );
			}
		}
	}
	else
	{
		if( CurrentAction.compare("DESTROY") != 0 ) 
		{
			CurrentAction = "DESTROY";
			ObjectManager.AddEntityToRelease( ID );
		}
	}
};

void CreateExplosion(const int& NumberOfParticles, const Vector2& Position, const Vector2& InitialVelocity, GameObjectManagerClass& ObjectManager)
{
	float		Angle;
	float		ImpulseMagnitude;
	Vector2		Direction;
	Vector2		Offset;
	Vector2		Impulse;
	shared_ptr<Entity> NewEntity;

	for(auto ii = 0; ii < NumberOfParticles; ++ii)
	{
		Angle = ((float)rand()/(float)RAND_MAX)*tau;
		Direction = RotationMatrix(Angle) * Vector2(0.0f, 1.0f);
			
		Offset = Direction * 0.8f;
		ImpulseMagnitude = ((float)rand()/(float)RAND_MAX)*1.6f + 0.4f;
		Impulse = Direction * ImpulseMagnitude;

		NewEntity = shared_ptr<Entity>(new Debris(0.22f, 1.15f, 0.3f, 50.0f + (((float)rand()/(float)RAND_MAX) * 50.0f)));
		NewEntity->p_cBody->SetTransform( Position + Offset, Angle);
		NewEntity->p_cBody->ApplyLinearVelocity(InitialVelocity);
		NewEntity->p_cBody->ApplyImpulseToCenter(Impulse);
		NewEntity->p_cBody->SetLinearDamping(0.2f);
		NewEntity->p_cBody->SetAngularDamping(0.2f);
						
		NewEntity->p_cRender->SetBodyColor( Vector3(0.95f,0.95f,0.95f) );
		NewEntity->p_cRender->SetDrawLines(false);
		//NewEntity->p_cRender->SetLineColor( Vector3(0.95f,0.95f,0.95f) );
		NewEntity->p_cRender->SetLayer(3.0f);
		ObjectManager.InsertIntoQueue(NewEntity);
		NewEntity.reset();
	}

};
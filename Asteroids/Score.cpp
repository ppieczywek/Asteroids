#pragma once
#include "stdafx.h"
#include "Score.h"
#include "GameEngineClass.h"

Score::Score(int Value)
{
	p_cBody = shared_ptr<cBody>(new cBody());		
	p_cBody->SetBodyType(kinematic2D);

	vector<Vector2> Vertices;
	
	Vertices.push_back( Vector2(  -0.5f,  0.5f ));
	Vertices.push_back( Vector2(   0.5f,  0.5f ));
	Vertices.push_back( Vector2(   0.5f, -0.5f ));
	Vertices.push_back( Vector2(  -0.5f, -0.5f ));
	
	
	Polygon2 Shape = Polygon2(Vertices);
	p_cBody->AddPolygon(Shape);
	
	p_cBody->SetDensity(0.4f);
	p_cBody->SetAngularMotion(false);
	
	p_cBody->SetCollisionType(None);
	
	p_cRender = shared_ptr<cRender>(new ScoreRender(Value));
	p_cRender->SetVisible(true);
			
	
	p_cRender->SetTranspercyValue(0.6f);
		
	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());

	p_cLogics =  shared_ptr<cLogics>(new ScoreLogics());

};


Score::~Score()
{

};



ScoreLogics::ScoreLogics()
{
	EntityT = Junk;
	IsHandlingContacts = false;
	IsHandlingInputs = false;
	CurrentAction = "BASE";

	LifeSpan = 35.0f;
}; 


ScoreLogics::~ScoreLogics()
{
	
}; 

void ScoreLogics::Update(GameObjectManagerClass& ObjectManager)
{
	if(LifeSpan > 0.0f) 
	{
		LifeSpan--;
		
		shared_ptr<Entity>& ScoreEntity = ObjectManager.GetEntity(ID);
		if(ScoreEntity != nullptr)
		{				
			auto Value = 0.15f + 0.85f * abs(LifeSpan / 35.0f);
			ScoreEntity->p_cRender->SetTranspercyValue( Value );
		
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

ScoreRender::ScoreRender(int ScoreValue)
{
	Value = to_string(static_cast<long long>(ScoreValue));
	TextOffset = GetStringLength(Value.c_str());
	TextOffset/=2.0f;
	Scale = 1.6f;
};



void ScoreRender::Draw(const vector<Polygon2>& PolygonList, const unsigned& PolygonsNumber)
{
	if(Scale < 3.0f)
	{
		Scale += 0.005f;
	}

	if(IsVisible) 
	{				
		Vector2 Position = PolygonList[0].GlobalCentroid;
		glColor4f( 255.0f/256.0f, 10.0f/256.0f, 10.0f/256.0f, Alpha);
		glPushMatrix();
			glTranslatef(Position.x - TextOffset*Scale, Position.y - (0.5f*Scale) ,9.0f);
			glScalef(Scale, Scale, 0.0f);
			PrintString( Value.c_str() );
		glPopMatrix();
	}
};

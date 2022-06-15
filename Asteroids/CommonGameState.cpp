#pragma once
#include "stdafx.h"
#include "CommonGameState.h"


int	 CommonGameState::TotalScore = 0;

void CommonGameState::Render(GameEngineClass &Engine) 
{

	Rectangle2D	Bounds = Engine.PhysicsManager.WorldProperties.GetWorldBox();

	vector<shared_ptr<Entity>>& EntityList = Engine.ObjectManager.GetEntityList();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	glClearColor( 246.0f/256.0f, 146.0f/256.0f, 174.0f/256.0f, 1.0f);  // pink  
	
	glPushMatrix();	
	for (unsigned ii=0; ii < EntityList.size(); ++ii) 
	{ 				
		EntityList[ii]->p_cRender->Draw( EntityList[ii]->p_cBody->GetPolygonsList(), EntityList[ii]->p_cBody->GetPolygonsNumber() );
	};
	DrawStateSpecific(Bounds);
	glPopMatrix();	
};

void CommonGameState::TeleportObjects(GameObjectManagerClass& ObjectManager, Rectangle2D& Bounds)
{
	EntityType Type;
	Vector2 Position;
	Vector2 Velocity;

	vector<shared_ptr<Entity>>& EntityList = ObjectManager.GetEntityList();
	Vector2 WorldCenter( (Bounds.min.x + Bounds.max.x)/2.0f, (Bounds.min.y + Bounds.max.y)/2.0f );
	
	for (unsigned ii=0; ii < EntityList.size(); ++ii) 
	{
		Type = EntityList[ii]->p_cLogics->GetEntityType();
			
		if( !BoxesIntersect(Bounds, EntityList[ii]->p_cBody->BBox) ) 
		{					
			Position = EntityList[ii]->p_cBody->GetPosition();
			Velocity = EntityList[ii]->p_cBody->GetLinearVelocity();

			if(Position.x > Bounds.max.x)
			{
				//if(Velocity.x > 0.0f)
				//{
					if(Type == PlayerE || Type == StarE || Type == AsteroidE)
					{
						Position.x = Bounds.min.x + 0.1f;//WorldCenter.x - (Position.x - WorldCenter.x);
						EntityList[ii]->p_cBody->SetTransform( Position , EntityList[ii]->p_cBody->GetAngle());
					}
					/*else if(Type == AsteroidE)
					{
						if (Velocity.x > 0.0f) 	ObjectManager.AddEntityToRelease( EntityList[ii]->ID );
					}
					else
					{

					}*/
				//}
			}
			else if(Position.x < Bounds.min.x)
			{
				//if(Velocity.x < 0.0f)
				//{
					if(Type == PlayerE || Type == StarE || Type == AsteroidE)
					{
						Position.x = Bounds.max.x - 0.001f;//WorldCenter.x - (Position.x - WorldCenter.x);
						EntityList[ii]->p_cBody->SetTransform( Position , EntityList[ii]->p_cBody->GetAngle());
					}
					/*else if(Type == AsteroidE)
					{
						if (Velocity.x < 0.0f) ObjectManager.AddEntityToRelease( EntityList[ii]->ID );
					}*/
				//}
			}
	
			if(Position.y > Bounds.max.y)
			{
				//if(Velocity.y > 0.0f)
				//{
					if(Type == PlayerE || Type == StarE || Type == AsteroidE)
					{
						Position.y = Bounds.min.y + 0.001f;//WorldCenter.y - (Position.y - WorldCenter.y);
						EntityList[ii]->p_cBody->SetTransform( Position , EntityList[ii]->p_cBody->GetAngle());
					}
	/*				else if(Type == AsteroidE)
					{
						if (Velocity.y > 0.0f) ObjectManager.AddEntityToRelease( EntityList[ii]->ID );
					}
					else
					{

					}*/
				//}
			}
			else if(Position.y < Bounds.min.y)
			{
					if(Type == PlayerE || Type == StarE || Type == AsteroidE)
					{
						Position.y = Bounds.max.y - 0.001f; //WorldCenter.y - (Position.y - WorldCenter.y);
						EntityList[ii]->p_cBody->SetTransform( Position , EntityList[ii]->p_cBody->GetAngle());
					}
			}
		}

	}
};
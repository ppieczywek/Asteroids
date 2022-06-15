#pragma once
#include "stdafx.h"
#include "PlayerProjectile.h"
#include "GameEngineClass.h"

PlayerProjectile::PlayerProjectile(Vector2 Position, float Angle)
{
	p_cBody = shared_ptr<cBody>(new cBody());		
	p_cBody->SetBodyType(dynamic2D);

	vector<Vector2> ProjectileVertices;

	ProjectileVertices.push_back( Vector2(  -0.25f,  0.71f ));
	ProjectileVertices.push_back( Vector2(   0.25f,  0.71f ));
	ProjectileVertices.push_back( Vector2(   0.25f, -0.71f ) );
	ProjectileVertices.push_back( Vector2(  -0.25f, -0.71f ) );

	Polygon2 ProjectileShape = Polygon2(ProjectileVertices);
	p_cBody->AddPolygon(ProjectileShape);
	
	p_cBody->SetDensity(0.001f);
	p_cBody->SetAngularMotion(true);
	p_cBody->SetAngularDamping(0.0f);
	p_cBody->SetLinearDamping(0.0f);
	
	p_cBody->SetTransform( Position+ (RotationMatrix(Angle) * Vector2(0.0f,3.5f)) , Angle);

	
	p_cBody->SetLinearVelocity( RotationMatrix(Angle) * Vector2(0.0f, 40.5f));

	p_cBody->SetCollisionType(Logical);
	
	p_cRender = shared_ptr<cRender>(new PlayerProjectileRender());
	p_cRender->SetVisible(true);

	p_cRender->SetBodyColor( Vector3(255.0f/256.0f, 246.0f/256.0f, 113.0f/256.0f));
	p_cRender->SetLineColor( Vector3(255.0f/256.0f, 246.0f/256.0f, 113.0f/256.0f));
		
	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());

	p_cLogics =  shared_ptr<cLogics>(new ProjectileLogics());

};


PlayerProjectile::~PlayerProjectile() 
{


};


ProjectileLogics::ProjectileLogics()
{
	EntityT = PlayerProj;
	IsHandlingContacts = true;
	IsHandlingInputs = false;
	LifeSpan = 60.0f;
	CurrentAction = "BASE";


}; 


ProjectileLogics::~ProjectileLogics()
{
	
}; 
	

void ProjectileLogics::HandleContacts(vector<ContactEvent>& ContactEventList, GameObjectManagerClass& ObjectManager)
{
	if(!ContactEventList.empty() && CurrentAction.compare("DESTROY") != 0 ) 
	{
		vector<shared_ptr<cLogics>>& cLogicsList = ObjectManager.GetcLogicsList();
		for each(auto Event in ContactEventList)
		{		
			shared_ptr<cLogics>& TargetLogics = cLogicsList[Event.Target];

			if(TargetLogics != nullptr)
			{
				if(TargetLogics->GetEntityType() == AsteroidE)
				{
					CurrentAction = "DESTROY";
					ObjectManager.AddEntityToRelease( ID );
		
					shared_ptr<Entity>& ProjectileEntity = ObjectManager.GetEntity(ID);

					if(ProjectileEntity != nullptr)
					{
	
						auto ProjectileBody = ProjectileEntity->p_cBody;
						if(ProjectileBody!=nullptr)
						{
							
								float NormalAngle = 0.0f;

								NormalAngle = atan2(Event.Solution->CollisionNormal.y , Event.Solution->CollisionNormal.x);
		
								shared_ptr<Entity> Stuff; 
								Vector2 Position = ProjectileBody->GetPosition();


								for(auto ii=0; ii<12; ++ii)
								{	
									auto AngleDeviation = ((float)rand()/(float)RAND_MAX)*2.2f - 1.1f;
									auto VelocityDeviation = ((float)rand()/(float)RAND_MAX)*6.0f;
									Vector2 Velocity = RotationMatrix( AngleDeviation ) * Event.Solution->CollisionNormal;

									
									if( Event.Solution->Body2 != Event.Source ) Velocity*=-1.0f;
		
									Stuff = shared_ptr<Entity>( new Debris( 0.9f, 0.25f, 0.02f,100.0f - (((float)rand()/(float)RAND_MAX) * 50.0f)));
									Stuff->p_cBody->SetTransform( Position, NormalAngle + AngleDeviation);
									Stuff->p_cBody->SetLinearVelocity(Velocity* (11.5f - VelocityDeviation) );
									Stuff->p_cBody->SetCollisionType(None);
									Stuff->p_cBody->SetLinearDamping(0.0f);
									Stuff->p_cBody->SetAngularDamping(0.0f);
									Stuff->p_cRender->SetBodyColor( Vector3(255.0f/256.0f, 246.0f/256.0f, 113.0f/256.0f));
									Stuff->p_cRender->SetDrawLines(false);
									Stuff->p_cRender->SetLayer(-1.0f);
					
						       		ObjectManager.InsertIntoQueue(Stuff);
									Stuff.reset();
								}	
						}
					}

				}
				break;
			}
		}
	}
};


void ProjectileLogics::HandleInputs(vector<KeyDefinitions>& KeyList, GameObjectManagerClass& ObjectManager)
{

};



void ProjectileLogics::Update(GameObjectManagerClass& ObjectManager)
{
	if(LifeSpan > 0.0f)
	{	
		LifeSpan--;
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



void PlayerProjectileRender::Draw(const vector<Polygon2>& PolygonList, const unsigned& PolygonsNumber)
{
	if(IsVisible) 
	{				
		if( PolygonsNumber != 0 )
		{		
			for(unsigned ii=0; ii < PolygonsNumber; ++ii)
			{
				glBegin(GL_TRIANGLE_FAN);
				glColor4f(BodyColor.x, BodyColor.y, BodyColor.z, (Alpha+0.01f)/2.0f);	
				glVertex3f(  PolygonList[ii].GlobalCentroid.x ,  PolygonList[ii].GlobalCentroid.y  , Layer );
				
				for( unsigned jj = 0; jj < PolygonList[ii].VertexNumber; ++jj ) 
				{ 					
					if(jj > 1)
					{
						glColor4f(BodyColor.x, BodyColor.y, BodyColor.z, 0.01f);
					}
					else
					{
							glColor4f(BodyColor.x, BodyColor.y, BodyColor.z, Alpha);
					}
					glVertex3f( PolygonList[ii].GlobalVertex[jj].x ,  PolygonList[ii].GlobalVertex[jj].y  , Layer );
				}

				glColor4f(BodyColor.x, BodyColor.y, BodyColor.z, Alpha);	
				glVertex3f( PolygonList[ii].GlobalVertex[0].x ,  PolygonList[ii].GlobalVertex[0].y  , Layer );
				glEnd();
			}
		}
	}
};

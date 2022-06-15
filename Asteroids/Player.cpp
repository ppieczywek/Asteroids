#pragma once
#include "stdafx.h"
#include "Player.h"
#include "GameEngineClass.h"

PlayerRocket::PlayerRocket() {
		
	p_cBody = shared_ptr<cBody>(new cBody());		
	p_cBody->SetBodyType(dynamic2D);

	vector<Vector2> RocketVertices;

	float ScaleFactor = 3.0f;
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
	p_cBody->SetDensity(0.0055f);

	p_cBody->SetRestitutionCoefficient(2.0f);

	p_cBody->SetAngularMotion(true);
	p_cBody->SetAngularDamping(0.0f);
	p_cBody->SetLinearDamping(0.6f);

	p_cBody->SetCollisionType(Both);

	Vector3 BodyColor = Vector3(255.0f/256.0f, 246.0f/256.0f, 113.0f/256.0f);
	Vector3 LineColor = BodyColor * 0.25f;
	p_cRender = shared_ptr<cRender>(new cRender());
	p_cRender->SetVisible(true);
	p_cRender->SetBodyColor( BodyColor );
	p_cRender->SetLineColor( LineColor );
	p_cRender->SetTranspercyValue(0.2f);

	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());
	p_cLogics =  shared_ptr<cLogics>(new PlayerLogics());
};

PlayerRocket::~PlayerRocket() 
{


};


PlayerLogics::PlayerLogics()
{
	EntityT = PlayerE;
	IsHandlingContacts = true;
	IsHandlingInputs = true;
	Charge = 0;
	WasRotateButtonPressed = false;
	ReleaseRate = 0;
	ImmuneTime = 240;
	IsImmune = true;
}; 


PlayerLogics::~PlayerLogics()
{
	
}; 
	
void PlayerLogics::HandleContacts(vector<ContactEvent>& ContactEventList, GameObjectManagerClass& ObjectManager)
{
	if(!IsImmune)
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

						auto PlayerEntity = ObjectManager.GetEntity( ID );
						
						if(PlayerEntity!=nullptr)
						{
							auto Velocity		= PlayerEntity->p_cBody->GetLinearVelocity();
							auto PlayerPosition = PlayerEntity->p_cBody->GetPosition();
							auto Angle			= PlayerEntity->p_cBody->GetAngle();
							auto BodyColor		= PlayerEntity->p_cRender->GetBodyColor();

							auto PolygonsNumber				= PlayerEntity->p_cBody->GetPolygonsNumber();
							vector<Polygon2>& PolygonsList	= PlayerEntity->p_cBody->GetPolygonsList();
								
							Vector2 ParticlePosition;
							Vector2 Impulse;
							
							float ImpulseDirection;
							float ImpulseMagnitude;
							float AngularVelocity;


							for(unsigned ii = 0; ii < PolygonsNumber; ++ii)
							{						
								for(auto kk=0; kk<2; ++kk)
								{
									for(unsigned jj=0; jj< PolygonsList[ii].VertexNumber; ++jj)
									{
										ParticlePosition = (PolygonsList[ii].GlobalVertex[jj] - PolygonsList[ii].GlobalCentroid) / 2.0f;
										ImpulseDirection = ((float)rand()/(float)RAND_MAX)*(pi/2.0f) + pi + static_cast<float>(jj)*(pi/2.0f);
										ImpulseMagnitude = ((float)rand()/(float)RAND_MAX)*3.0f;
										Impulse			 =  RotationMatrix(ImpulseDirection) * Vector2(0.0f, ImpulseMagnitude);
										AngularVelocity	 = ((float)rand()/(float)RAND_MAX)*2.4f - 1.2f;
																		
										shared_ptr<Entity> NewEntity = shared_ptr<Entity>(new Debris(0.4f, 0.4f, 2.6f,80.0f));
										NewEntity->p_cBody->SetTransform( PolygonsList[ii].GlobalCentroid + ParticlePosition, Angle);
										NewEntity->p_cBody->SetLinearVelocity(Velocity);
										NewEntity->p_cBody->SetAngularVelocity(AngularVelocity);
										NewEntity->p_cBody->ApplyImpulseToCenter(Impulse);
										NewEntity->p_cBody->SetLinearDamping(1.8f);
										NewEntity->p_cRender->SetBodyColor(BodyColor);
										NewEntity->p_cRender->SetLineColor(BodyColor);
										NewEntity->p_cRender->SetLayer(0.0f);
										ObjectManager.InsertIntoQueue(NewEntity);
										NewEntity.reset();
									}
								}
							}
							CreateExplosion(45, PlayerPosition, Velocity, ObjectManager);
						}
						break;
					}
				}
			}
		}
	}
};


void PlayerLogics::HandleInputs(vector<KeyDefinitions>& KeyList, GameObjectManagerClass& ObjectManager)
{
	
	shared_ptr<Entity>& PlayerEntity = ObjectManager.GetEntity(ID);

	if(PlayerEntity != nullptr)
	{

		WasRotateButtonPressed = false;

		for each(auto Key in KeyList)
		{
			if(Key == LEFT)
			{
				PlayerEntity->p_cBody->ApplyTorque( 400.5f * PlayerEntity->p_cBody->GetInertia() );
				WasRotateButtonPressed = true;
			}

			if(Key == RIGHT)
			{
				PlayerEntity->p_cBody->ApplyTorque( -400.5f * PlayerEntity->p_cBody->GetInertia() );
				WasRotateButtonPressed = true;
			}

			if(Key == UP)
			{

				auto PlayerAngle = PlayerEntity->p_cBody->GetAngle();
				auto PlayerPosition = PlayerEntity->p_cBody->GetPosition();
				auto Thrust = RotationMatrix( PlayerAngle ) * Vector2(0.0f, 0.4f);
				PlayerEntity->p_cBody->ApplyImpulseToCenter(Thrust * PlayerEntity->p_cBody->GetMass());

				if(ReleaseRate>=4)
				{					
					auto Emission = (rand() % 5 + 1);
					Vector3 BodyColor = Vector3(246.0f/256.0f, 238.0f/256.0f, 240.0f/256.0f);
					Vector3 LineColor = BodyColor * 0.9f;

					for(auto ii=0; ii<Emission; ++ii)
					{
						//auto x = ((float)rand()/(float)RAND_MAX)*0.6f - 0.3f;
						auto AngleDeviation = ((float)rand()/(float)RAND_MAX)*1.2f - 0.6f;
						auto VelocityDeviation = ((float)rand()/(float)RAND_MAX)*6.0f - 3.0f;
			
						Vector2 StartPosition = TransformationMatrix( PlayerAngle, PlayerPosition ) * Vector2(0.0f, -1.8f);
						Vector2 StartVelocity = RotationMatrix( PlayerAngle + AngleDeviation) * Vector2(0.0f, -1.0f);
		
						shared_ptr<Entity> Stuff = shared_ptr<Entity>( new Debris(0.65f, 0.65f, 0.01f, 110.0f - (((float)rand()/(float)RAND_MAX) * 40.0f)));
						Stuff->p_cBody->SetBodyType(dynamic2D);
						Stuff->p_cBody->SetTransform( StartPosition, PlayerAngle + AngleDeviation);
						Stuff->p_cBody->SetLinearVelocity( StartVelocity* (6.5f - VelocityDeviation) );
						Stuff->p_cBody->SetCollisionType(None);
						Stuff->p_cBody->SetLinearDamping(1.6f);
						Stuff->p_cRender->SetBodyColor( BodyColor );
						Stuff->p_cRender->SetLineColor( LineColor );
						ObjectManager.InsertIntoQueue(Stuff);
						Stuff.reset();
					}

					ReleaseRate = 0;
				}
				++ReleaseRate;
			}

			if(Key == FIRE)
			{
				if( Charge <= 0 )
				{
				
					Charge = 6;
					auto Recoil = RotationMatrix( PlayerEntity->p_cBody->GetAngle() ) * Vector2(0.0f, -0.07f);
					PlayerEntity->p_cBody->ApplyImpulseToCenter(Recoil * PlayerEntity->p_cBody->GetMass());
					
					shared_ptr<Entity> BodyPtr;

					auto AngleDeviation = ((float)rand()/(float)RAND_MAX)*0.1f - 0.05f;
					BodyPtr = shared_ptr<Entity>( new PlayerProjectile( PlayerEntity->p_cBody->GetPosition(), PlayerEntity->p_cBody->GetAngle() + AngleDeviation) );
					auto Direction = RotationMatrix(PlayerEntity->p_cBody->GetAngle()) * Vector2(0.0f,1.0f);
					auto RelativeVelocity = Direction * PlayerEntity->p_cBody->GetLinearVelocity() * Direction;
					BodyPtr->p_cBody->ApplyLinearVelocity(RelativeVelocity);
					BodyPtr->p_cBody->AddNonCollidable(ID);
					ObjectManager.InsertIntoQueue(BodyPtr);
					BodyPtr.reset();
				}
			}
		}
		
	}
};


void PlayerLogics::Update( GameObjectManagerClass& ObjectManager)
{

	shared_ptr<Entity>& PlayerEntity = ObjectManager.GetEntity(ID);

	if(PlayerEntity != nullptr)
	{
		if(Charge > 0) Charge--;

		if(IsImmune) 
		{
			auto x = pi/2.0f +   7.0f * tau * ((ImmuneTime / 240.0f));
			auto CurrentAlpha = 0.6f - 0.4f * sin(x);
			PlayerEntity->p_cRender->SetTranspercyValue(CurrentAlpha);

			ImmuneTime--;
			if(ImmuneTime <= 0.0f)
			{
				IsImmune = false;
				PlayerEntity->p_cRender->SetTranspercyValue(1.0f);
			}
		}


	
		auto PlayerVelocity = PlayerEntity->p_cBody->GetLinearVelocity();

		if( PlayerVelocity.length() > 20.0f)
		{		
			PlayerEntity->p_cBody->SetLinearVelocity( PlayerVelocity.unit() * 20.0f );
		}


		auto PlayerAngularVelocity = PlayerEntity->p_cBody->GetAngularVelocity();

		if( abs(PlayerAngularVelocity) > 4.05f)
		{		
			PlayerEntity->p_cBody->SetAngularVelocity(  (PlayerAngularVelocity / abs(PlayerAngularVelocity)) * 4.05f );
		}

		if(WasRotateButtonPressed == false)
		{
			auto AngVel = PlayerEntity->p_cBody->GetAngularVelocity();
			PlayerEntity->p_cBody->ApplyAngularVelocity( AngVel * -0.3f );
		}
	}
};
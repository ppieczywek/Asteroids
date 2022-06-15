#pragma once
#include "stdafx.h"
#include "Asteroid.h"
#include "GameEngineClass.h"


Asteroid::Asteroid()
{
	p_cBody = shared_ptr<cBody>(new cBody());		
	p_cBody->SetBodyType(dynamic2D);

	unsigned Grid[5][5] = {0};
	
	unsigned TotalBlocksNumber = AsteroidMaxBlockNumber - ((rand() % 7)+1);
	unsigned CurrentBlocksNumber = 1;

	vector<pair<int,int>> CurrentCheck;
	vector<pair<int,int>> NextCheck;


	int Row = 0;
	int Col = 0;

	int NextRow = 0;
	int NextCol = 0;

	int XNeighbours[4] = {-1, 0, 1,  0};
	int YNeighbours[4] = { 0, 1, 0, -1};
	
	
	CurrentCheck.push_back(pair<int,int>(3,3));

	while( CurrentBlocksNumber < TotalBlocksNumber )
	{

		NextCheck.clear();

		if(!CurrentCheck.empty())
		{
			for each(auto Coordinates in CurrentCheck)
			{
				Row =  Coordinates.first;
				Col = Coordinates.second;
				if( Row >= 0 && Row < 5 && Col >= 0 && Col < 5)
				{
					if( Grid[Row][Col] != 1)
					{
						auto Result = rand() % 4;
						if( Result <= 2 || (Row == 3 && Col == 3))
						{
							
							Grid[Row][Col] = 1;
							CurrentBlocksNumber++;

							for(auto kk=0; kk<4; ++kk)
							{
								NextRow = Row + YNeighbours[kk];
								NextCol = Col + XNeighbours[kk];

								if( NextRow >= 0 && NextRow < 5 && NextCol >= 0 && NextCol < 5)
								{
									if( Grid[NextRow][NextCol] != 1)
									{
										NextCheck.push_back(pair<int, int>( NextRow, NextCol));
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			break;
		}

		if(!CurrentCheck.empty())
		{
			CurrentCheck.clear();
			if(!NextCheck.empty())
			{
				CurrentCheck.resize( NextCheck.size() );
				CurrentCheck.assign( NextCheck.begin(), NextCheck.end() );
			}
		}
	}
	
	vector<Vector2> Vertices;
	Vector2 Offset;

	float Size = AsteroidMaxBlockSize - ((float)rand()/(float)RAND_MAX)*0.75f;
	

	Vertices.push_back( Vector2( 0.0f,  0.0f ));
	Vertices.push_back( Vector2( 0.0f,  Size ));
	Vertices.push_back( Vector2( Size,  Size ));
	Vertices.push_back( Vector2( Size,  0.0f ));
	
	Polygon2 AsteroidBlock(Vertices);
	Polygon2 NewBlock;

	for(auto ii=0; ii<5; ++ii)
	{
		for(auto jj=0; jj<5; ++jj)
		{

			if(Grid[ii][jj] == 1)
			{
				Offset = Vector2( Size * static_cast<float>(jj), Size * static_cast<float>(ii)  );
				
				NewBlock = AsteroidBlock;
				for(unsigned qq=0; qq< NewBlock.VertexNumber;++qq)
				{
					NewBlock.LocalVertex[qq] += Offset;
					NewBlock.GlobalVertex[qq]  += Offset;
				}
				NewBlock.LocalCentroid += Offset;
				NewBlock.GlobalCentroid  += Offset;

				p_cBody->AddPolygon(NewBlock);
				Vertices.clear();
			}
		}
	}

	p_cBody->SetDensity(3.4f);
	p_cBody->SetAngularMotion(true);
	p_cBody->SetAngularDamping(0.0f);
	p_cBody->SetLinearDamping(0.0f);
	
	

	p_cBody->SetCollisionType(Logical);
	
	p_cRender = shared_ptr<cRender>(new cRender());
	p_cRender->SetVisible(true);
	p_cRender->SetDrawLines(true);
	p_cRender->SetLineColor(Vector3(0.95f,0.95f,0.95f));

	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());

	p_cLogics =  shared_ptr<cLogics>(new AsteroidLogics(  ));
};




Asteroid::Asteroid(vector<Polygon2>& PolygonsList)
{
	p_cBody = shared_ptr<cBody>(new cBody());		
	p_cBody->SetBodyType(dynamic2D);
		
			
	for(unsigned ii=0; ii < PolygonsList.size(); ++ii)
	{
		p_cBody->AddPolygon(PolygonsList[ii]);
	}
	
	p_cBody->SetDensity(2.02f);
	p_cBody->SetAngularMotion(true);
	p_cBody->SetAngularDamping(0.0f);
	p_cBody->SetLinearDamping(0.0f);
	
	

	p_cBody->SetCollisionType(Logical);
	
	p_cRender = shared_ptr<cRender>(new cRender());
	p_cRender->SetVisible(true);
	
	p_cRender->SetDrawLines(true);

	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());

	p_cLogics =  shared_ptr<cLogics>(new AsteroidLogics());

};


Asteroid::~Asteroid()
{
/*	p_cRender.reset();
	p_cBody.reset();
	p_cConstrained.reset();
	p_cLogics.reset();*/
};



AsteroidLogics::AsteroidLogics()
{
	EntityT = AsteroidE;
	IsHandlingContacts = true;
	IsHandlingInputs = false;
	Energy = 13;
	CurrentAction = "BASE";
}; 


AsteroidLogics::~AsteroidLogics()
{
	
}; 

void AsteroidLogics::SetEnergy(const int& value)
{
	Energy = value;
};
	
void AsteroidLogics::HandleContacts(vector<ContactEvent>& ContactEventList, GameObjectManagerClass& ObjectManager)
{
	vector<shared_ptr<cLogics>>& LogicsList = ObjectManager.GetcLogicsList();

    for each(auto Event in ContactEventList)
	{
		if( LogicsList[Event.Target] != nullptr )
		{
			if( LogicsList[Event.Target]->GetEntityType() == PlayerProj )
			{
				Energy -=11;
			}
		}
	}
				
};


void AsteroidLogics::HandleInputs(vector<KeyDefinitions>& KeyList, GameObjectManagerClass& ObjectManager)
{


};


void AsteroidLogics::Update(GameObjectManagerClass& ObjectManager)
{
	
	shared_ptr<Entity>& AsteroidEntity = ObjectManager.GetEntity(ID);

	if(AsteroidEntity != nullptr)
	{

		shared_ptr<cBody>& AsteroidBody = AsteroidEntity->p_cBody;
		auto AsteroidColor = AsteroidEntity->p_cRender->GetBodyColor();
		auto LineColor = AsteroidEntity->p_cRender->GetLineColor();
		auto AsteroidLayer = AsteroidEntity->p_cRender->GetLayer();

		if(Energy <= 0 && CurrentAction.compare("DESTROY") != 0 ) 
		{
			CurrentAction = "DESTROY";
			ObjectManager.AddEntityToRelease( ID );

			if(AsteroidBody!=nullptr)
			{

				auto PolygonsNumber = AsteroidBody->GetPolygonsNumber();
				vector<Polygon2>& PolygonsList = AsteroidBody->GetPolygonsList();
				auto AsteroidAngle = AsteroidBody->GetAngle();
				auto AsteroidCentroid = AsteroidBody->GetPosition();
				auto AsteroidDensity = AsteroidBody->GetDensity();

				if(PolygonsNumber > 2)
				{					
					vector<unsigned> SubGroups[4];	 
					Vector2			 GlobalCentroids[4] = {ZeroVector};
					

					for(unsigned ii = 0; ii < PolygonsNumber; ++ii)
					{

						if(PolygonsList[ii].LocalCentroid.x <= 0.0f && PolygonsList[ii].LocalCentroid.y >= 0.0f)
						{
							SubGroups[0].push_back(ii);
							GlobalCentroids[0] += PolygonsList[ii].GlobalCentroid;
						} 
						else if(PolygonsList[ii].LocalCentroid.x <= 0.0f && PolygonsList[ii].LocalCentroid.y < 0.0f)
						{
							SubGroups[1].push_back(ii);
							GlobalCentroids[1] += PolygonsList[ii].GlobalCentroid;
						}
						else if(PolygonsList[ii].LocalCentroid.x > 0.0f && PolygonsList[ii].LocalCentroid.y < 0.0f)
						{
							SubGroups[2].push_back(ii);
							GlobalCentroids[2] += PolygonsList[ii].GlobalCentroid;
						}
						else if(PolygonsList[ii].LocalCentroid.x > 0.0f && PolygonsList[ii].LocalCentroid.y >= 0.0f)
						{
							SubGroups[3].push_back(ii);
							GlobalCentroids[3] += PolygonsList[ii].GlobalCentroid;
						}

					}


					vector<Polygon2> NewAsteroid;

					for(auto ii = 0; ii < 4; ++ii)
					{

						NewAsteroid.clear();
						auto GroupSize = static_cast<float>(SubGroups[ii].size());
						
						if(GroupSize > 0.0f)
						{							
							GlobalCentroids[ii] /= GroupSize;
						

						for each(auto PolygonsId in SubGroups[ii])
						{
							NewAsteroid.push_back( PolygonsList[ PolygonsId ] );
						}

						if(!NewAsteroid.empty())
						{
							auto PreviousVelocity = AsteroidBody->GetLinearVelocityAtPoint(GlobalCentroids[ii]);
							//auto Impulse = (GlobalCentroids[ii] - AsteroidCentroid).unit() * 20.7f;
							auto AngularVelocity = ((float)rand()/(float)RAND_MAX)*0.8f - 0.1f;
	
							shared_ptr<Entity> NewEntity = shared_ptr<Entity>(new Asteroid(NewAsteroid));
							NewEntity->p_cBody->SetTransform(GlobalCentroids[ii], AsteroidAngle);

							NewEntity->p_cBody->SetLinearVelocity(PreviousVelocity);
							NewEntity->p_cBody->SetAngularVelocity(AngularVelocity);
							//NewEntity->p_cBody->ApplyImpulseToCenter(Impulse);
							NewEntity->p_cRender->SetBodyColor(AsteroidColor);
							NewEntity->p_cRender->SetLineColor(LineColor);
							NewEntity->p_cRender->SetLayer(AsteroidLayer);
							ObjectManager.InsertIntoQueue(NewEntity);
							
							NewEntity.reset();

						}
						}
					}

				}
				else
				{
					float ParticleSize = abs(PolygonsList[0].LocalVertex[1].y - PolygonsList[0].LocalVertex[0].y) / 3.0f;

					for(unsigned ii = 0; ii < PolygonsNumber; ++ii)
					{
						Vector2 ParticlePosition;
						Vector2 Impulse;
						Vector2 Velocity;

						float ImpulseDirection;
						float ImpulseMagnitude;
						float AngularVelocity;
						
						for(auto kk=0; kk<2; ++kk)
						{
							for(unsigned jj=0; jj< PolygonsList[ii].VertexNumber; ++jj)
							{

								ParticlePosition = (PolygonsList[ii].GlobalVertex[jj] - PolygonsList[ii].GlobalCentroid) / 2.0f;

								Velocity = AsteroidBody->GetLinearVelocityAtPoint(ParticlePosition + PolygonsList[ii].GlobalCentroid);

								ImpulseDirection = ((float)rand()/(float)RAND_MAX)*(pi/2.0f) + pi + static_cast<float>(jj)*(pi/2.0f);
								ImpulseMagnitude = ((float)rand()/(float)RAND_MAX)*3.0f;
								Impulse =  RotationMatrix(ImpulseDirection) * Vector2(0.0f, ImpulseMagnitude);
								AngularVelocity = ((float)rand()/(float)RAND_MAX)*2.4f - 1.2f;

								//Impulse = ParticlePosition.unit() * 2.0f;
								//AngularVelocity = 0.0f;

								shared_ptr<Entity> NewEntity = shared_ptr<Entity>(new Debris(ParticleSize, ParticleSize, 0.8f,80.0f));
								NewEntity->p_cBody->SetTransform( PolygonsList[ii].GlobalCentroid + ParticlePosition, AsteroidAngle);
									
								NewEntity->p_cBody->SetLinearVelocity(Velocity);
								NewEntity->p_cBody->SetAngularVelocity(AngularVelocity);
								NewEntity->p_cBody->ApplyImpulseToCenter(Impulse);
								NewEntity->p_cBody->SetLinearDamping(0.0f);
								NewEntity->p_cBody->SetAngularDamping(0.0f);
								NewEntity->p_cRender->SetBodyColor(AsteroidColor);
								NewEntity->p_cRender->SetLineColor(LineColor);
								NewEntity->p_cRender->SetLayer(0.0f);
								ObjectManager.InsertIntoQueue(NewEntity);
								NewEntity.reset();
							}
						}
					}

				}

				auto Velocity = AsteroidBody->GetLinearVelocity();
				CreateExplosion(25, AsteroidCentroid, Velocity, ObjectManager);


			}
		}

		//AsteroidBody.reset();
	}

	//AsteroidEntity.reset();
};
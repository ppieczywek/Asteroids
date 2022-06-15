#pragma once
#include "stdafx.h"
#include "MainStateClass.h"
#include "PausedStateClass.h"
#include "GameOverState.h"

void MainStateClass::Initialize(GameEngineClass &Engine) 
{	
	FpsCnt = EventTimer.GetPerformanceCounterTime();
	EventTimer.Init();
	PlayersLifes = 3;
	PlayerIsDestroyed = false;
	TotalScore = 0;

	shared_ptr<Entity> BodyPtr = nullptr;
	BodyPtr = nullptr;
		
	AsteroidColors[0] = Vector3(121.0f/256.0f, 210.0f/256.0f, 224.0f/256.0f);
	AsteroidColors[1] = Vector3(156.0f/256.0f, 215.0f/256.0f, 164.0f/256.0f);
	AsteroidColors[2] = Vector3(237.0f/256.0f, 119.0f/256.0f, 213.0f/256.0f);
	AsteroidColors[3] = Vector3(228.0f/256.0f, 225.0f/256.0f, 215.0f/256.0f);

	BodyPtr = shared_ptr<Entity> (new PlayerRocket());
	BodyPtr->p_cBody->SetTransform(Vector2(60.0f,45.0f),0.0f );
	Engine.ObjectManager.AddEntity( BodyPtr );
	
	PlayerPreviousPosition.x = 60.0f;
	PlayerPreviousPosition.y = 45.0f;

	PlayersId = BodyPtr->GetID();
		
	Engine.Camera.SetAlpha(0.01f);
	Engine.Camera.SetFocusAtFixedPoint(Vector2(60.0f,45.0f));
	Engine.Camera.SetWidth(80.0f);
	
	for(int ii=0; ii < PlayersLifes; ++ii)
	{
		Vector2 Origin(25.0f, 20.0f); 
		Vector2 Offset(3.0f, 00.0f); 
		auto BodyPtr = shared_ptr<Entity>( new PlayerIcon());
		BodyPtr->p_cBody->SetTransform( Origin + Offset * static_cast<float>(ii), 0.1f); 
		Engine.ObjectManager.AddEntity( BodyPtr );
		IconSlots.push_back( BodyPtr->GetID() );
	}

	Rectangle2D scr = Engine.PhysicsManager.WorldProperties.GetWorldBox();	
	TimeStamp = EventTimer.GetPerformanceCounterTime();
};

void MainStateClass::Cleanup(GameEngineClass &Engine) 
{

};


void MainStateClass::Resume()
{
	WasExitPressed = true;
};


void MainStateClass::Update(GameEngineClass &Engine) 
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
		
	auto player = Engine.ObjectManager.GetEntity(PlayersId);
	if (player != nullptr)
	{
		auto PlayerCurrentPosition = player->p_cBody->GetPosition();

		Vector2 DstToCenter = PlayerCurrentPosition - Vector2(60.0f, 45.0f);
		auto translation = PlayerCurrentPosition - PlayerPreviousPosition;

		translation = (DstToCenter.length() / 250.0f) * translation;

		auto cBodyList = Engine.ObjectManager.GetcBodyList();
		auto cLogicsList = Engine.ObjectManager.GetcLogicsList();

		for (size_t kk = 0; kk < cBodyList.size(); kk++)
		{
			if (cLogicsList[kk]->GetEntityType() == StarE)
			{
				auto bodyPos = cBodyList[kk]->GetPosition();
				auto starID = cBodyList[kk]->GetID();
				auto star = Engine.ObjectManager.GetEntity(starID);
				auto Layer = -1.0f * star->p_cRender->GetLayer();
				auto factor = 1.0f - Layer / 60.0f; 
				auto starTranslation = factor * translation;

				cBodyList[kk]->SetTransform(bodyPos - starTranslation);
			}
			else if (cLogicsList[kk]->GetEntityType() == IconE)
			{

			}
			else
			{
				auto bodyPos = cBodyList[kk]->GetPosition();
				cBodyList[kk]->SetTransform(bodyPos - translation);
			}
		}
	}

	CheckPlayerStatus(ObjectManager, Bounds); 	
	CountScore(ObjectManager);
	ObjectManager.Update();
	TeleportObjects(ObjectManager, Bounds);

		
	double CurrentTime = EventTimer.GetPerformanceCounterTime();
	if( (CurrentTime - TimeStamp) > 8.0  && PlayersLifes > 0.0f )
	{
		for(int ii=0; ii<3; ++ii)
		{
			InsertAsteroid(ObjectManager, Bounds);
		}
		TimeStamp = CurrentTime;
	}
	Engine.Camera.UpdateCameraPosition();

	
	vector<KeyDefinitions>& KeyList = Engine.InputManager.GetKeyPressedList();

	auto Iterator = find(KeyList.begin(), KeyList.end(), EXIT);
	if(Iterator != KeyList.end())
	{
		if(!WasExitPressed)
		{
			auto NewState = shared_ptr<GameStateClass> (new PausedStateClass());
			Engine.PushState(NewState);
		}
	}
	else
	{
		WasExitPressed = false;
	}

	if(PlayersLifes <= 0.0f && PlayerIsDestroyed == true )
	{
		auto NewState = shared_ptr<GameStateClass> (new GameOverState());
		Engine.ChangeState(NewState);
	}
	
};


void MainStateClass::InsertAsteroid(GameObjectManagerClass& ObjectManager, Rectangle2D& Bounds)
{
	float PosX,PosY, DevX, DevY, AngVel, VelocityMagnitude;
	Vector2 WorldCenter( (Bounds.min.x + Bounds.max.x)/2.0f, (Bounds.min.y + Bounds.max.y)/2.0f );

	auto BodyPtr = shared_ptr<Entity> (new Asteroid());
	if( (rand() % 2) == 1 ) 
	{
		if( (rand() % 2) == 1 ) 
		{
			PosX = Bounds.min.x - (BodyPtr->p_cBody->BBox.max.x - BodyPtr->p_cBody->BBox.min.x) / 2.0f;
		}
		else
		{
			PosX = Bounds.max.x + (BodyPtr->p_cBody->BBox.max.x - BodyPtr->p_cBody->BBox.min.x) / 2.0f;
		}
		PosY = ((float)rand()/(float)RAND_MAX)*65.0f;
	}
	else
	{
		if( (rand() % 2) == 1 ) 
		{
			PosY = Bounds.min.y - (BodyPtr->p_cBody->BBox.max.y - BodyPtr->p_cBody->BBox.min.y) / 2.0f;
		}
		else
		{
			PosY = Bounds.max.y + (BodyPtr->p_cBody->BBox.max.y - BodyPtr->p_cBody->BBox.min.y) / 2.0f;
		}
		PosX = ((float)rand()/(float)RAND_MAX)*81.0f;
	}

	DevX = ((float)rand()/(float)RAND_MAX)*60.0f - 30.0f;
	DevY = ((float)rand()/(float)RAND_MAX)*60.0f - 30.0f;
	VelocityMagnitude = ((float)rand()/(float)RAND_MAX)*6.0f + 5.0f;

	AngVel = ((float)rand()/(float)RAND_MAX)*2.6f - 1.3f;

	Vector2 VelocityDirection = (WorldCenter + Vector2(DevX,DevY)) - Vector2(PosX, PosY);
	Vector2 Velocity = VelocityDirection.unit() * VelocityMagnitude; 
	

	BodyPtr->p_cBody->SetTransform(Vector2(PosX, PosY), 0.0f );
	BodyPtr->p_cBody->ApplyLinearVelocity(Velocity);
	BodyPtr->p_cBody->ApplyAngularVelocity(AngVel );

	int ColorId = rand() % 4;
	Vector3 BodyColor = AsteroidColors[ColorId];
	Vector3 LineColor = BodyColor * 0.25f;
	float Layer  = ((float)rand()/(float)RAND_MAX)*3.0f;
	BodyPtr->p_cRender->SetBodyColor( BodyColor );
	BodyPtr->p_cRender->SetLineColor( LineColor );
	BodyPtr->p_cRender->SetLayer(Layer);
	ObjectManager.AddEntity( BodyPtr );

}

void MainStateClass::DrawStateSpecific(Rectangle2D& Bounds)
{		
	glColor4f(255.0f/256.0f, 246.0f/256.0f, 113.0f/256.0f, 0.9f);
	glPushMatrix();
		glTranslatef( Bounds.min.x + 25.0f, Bounds.max.y - 25.0f,9.0f);
		glScalef(2.7f, 2.7f, 0.0f);
		string ScoreInString = to_string(static_cast<long long>(TotalScore));
		PrintString( ScoreInString.c_str() );
	glPopMatrix();
};


void MainStateClass::CheckPlayerStatus(GameObjectManagerClass& ObjectManager, Rectangle2D& Bounds) 
{

	auto Player = ObjectManager.GetEntity(PlayersId);
	if(Player == nullptr)
	{
		if(!PlayerIsDestroyed)
		{
			PlayerIsDestroyed = true;
			TimeOfDeath = EventTimer.GetPerformanceCounterTime();
			if(PlayersLifes > 0.0f)
			{
				PlayersLifes--;
				auto Icon = IconSlots[ IconSlots.size() - 1 ];
				ObjectManager.AddEntityToRelease( Icon );
				IconSlots.pop_back();
			}
		} 
		else
		{
			if(PlayersLifes > 0.0f)
			{
				auto CurrentTime = EventTimer.GetPerformanceCounterTime();
				auto Interval = CurrentTime - TimeOfDeath;
				if(Interval > 3.0f) 
				{
					PlayerIsDestroyed = false;
					auto BodyPtr = shared_ptr<Entity> (new PlayerRocket());
					BodyPtr->p_cBody->SetTransform(Vector2( Bounds.max.x / 2.0f , Bounds.max.y / 2.0f  ),0.0f );
					ObjectManager.AddEntity( BodyPtr );
					PlayersId = BodyPtr->GetID();
				}
			}

		}
		
	}
};

void MainStateClass::CountScore(GameObjectManagerClass& ObjectManager)
{
	
	vector<long long>& EntityToRelease = ObjectManager.GetEntityToReleaseList();
	
	shared_ptr<Entity> GameEntity = nullptr;
	shared_ptr<Entity> ScoreEntity = nullptr;
	Vector3 Color;
	Vector2 Position;
	Vector2 Velocity;

	unsigned NumberOfBlocks;
	int		 CurrentScore;
	for each(auto Id in EntityToRelease)
	{
		
		GameEntity = ObjectManager.GetEntity( Id );
		if(GameEntity != nullptr)
		{
			if(GameEntity->p_cLogics->GetEntityType() == AsteroidE)
			{
				auto Action = GameEntity->p_cLogics->GetCurrentAction();
				if(Action.compare("DESTROY") == 0)
				{
					NumberOfBlocks = GameEntity->p_cBody->GetPolygonsNumber();
					CurrentScore = NumberOfBlocks * 100;
								
					TotalScore += CurrentScore;

					Color = GameEntity->p_cRender->GetBodyColor(); 
					Position = GameEntity->p_cBody->GetPosition();
					Velocity = GameEntity->p_cBody->GetLinearVelocity();

					ScoreEntity = shared_ptr<Entity>(new Score(CurrentScore));
					ScoreEntity->p_cBody->SetTransform(Position, 0.0f);
					ScoreEntity->p_cRender->SetBodyColor(Color);
					ObjectManager.InsertIntoQueue(ScoreEntity);
					ScoreEntity.reset();
				}
			}
		}

	}

};
#pragma once
#include "Vector2.h"
#include "Player.h"
#include "PlayerIcon.h"
#include "PlayerProjectile.h"
#include "Debris.h"
#include "Asteroid.h"
#include "Star.h"
#include "Score.h"
#include "SoftConstraint.h"
#include "HingeConstraint.h"
#include "WeldConstraint.h"
#include "HybridConstraint.h"
#include "dynamic_array.h"
#include <set>
#include <unordered_map>
#include <map>
#include <vector>
using namespace std;

class GameObjectManagerClass 
{
	long long								IDCounter;
	int										NumberOfConstraints;
	int										NumberOfEntities;
	bool									AddConstraintW(shared_ptr<Constraint> &C);//wrapped function
	
	vector<shared_ptr<Entity>>				EntityList;
	vector<shared_ptr<cBody>>				cBodyList;
	vector<shared_ptr<cRender>>				cRenderList;
	vector<shared_ptr<cConstrained>>		cConstrainedList;
	vector<shared_ptr<cLogics>>				cLogicsList;

	vector<shared_ptr<Entity>>				EntityQueue;

	vector<long long>						EntityToRelease;
	vector<shared_ptr<Constraint>>			ConstraintList;
	map<long long,int>						IDtoArrayIndex;
	map<long long,int>						ConstraintIDtoArrayIndex;

public:
	
	friend class							ConstraintManagerClass;
	friend class							CollisionManagerClass;
	friend class							PhysicsManagerClass;
	friend class							MainStateClass;

											GameObjectManagerClass();
											~GameObjectManagerClass();

	bool									AddEntity(shared_ptr<Entity>&);
	bool									RemoveEntity(const long long&);
	bool									RemoveEntity(shared_ptr<Entity>&);
	shared_ptr<Entity>						GetEntity(const long long&);
			
	bool									AddConstraint(shared_ptr<HingeConstraint>&);
	bool									AddConstraint(shared_ptr<SoftConstraint>&);
	bool									AddConstraint(shared_ptr<WeldConstraint>&);
	bool									AddConstraint(shared_ptr<HybridConstraint>&);
	bool									RemoveConstraint(const long long&);
	bool									RemoveConstraint(shared_ptr<Constraint>&);
	shared_ptr<Constraint>					GetConstraint(const long long&);

	inline vector<shared_ptr<cBody>>&		GetcBodyList(); 
	inline vector<shared_ptr<cLogics>>&		GetcLogicsList();
	inline vector<shared_ptr<cRender>>&		GetcRenderList();
	inline vector<shared_ptr<Entity>>&		GetEntityList();

	inline vector<long long>&				GetEntityToReleaseList();

	
	void									AddEntityToRelease(long long&);
	void									CleanUp();

	void									InsertIntoQueue(shared_ptr<Entity>&);
	void									ClearQueue();
	void									Update();


	
	void									ClearAll();
};

vector<shared_ptr<Entity>>&	GameObjectManagerClass::GetEntityList()
{
	return EntityList;
}


vector<shared_ptr<cBody>>&	GameObjectManagerClass::GetcBodyList()
{
	return 	cBodyList;
}; 


vector<shared_ptr<cLogics>>&	GameObjectManagerClass::GetcLogicsList()
{
	return 	cLogicsList;
}; 


vector<long long>& GameObjectManagerClass::GetEntityToReleaseList()
{
	return EntityToRelease;	
};


vector<shared_ptr<cRender>>&	GameObjectManagerClass::GetcRenderList()
{
	return 	cRenderList;
}; 
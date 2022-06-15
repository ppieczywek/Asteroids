#pragma once
#include "stdafx.h"
#include "GameObjectManagerClass.h"


GameObjectManagerClass::GameObjectManagerClass() : NumberOfEntities(0),
												   NumberOfConstraints(0),
												   IDCounter(0)
{
	EntityList.reserve(1000);
	cBodyList.reserve(1000);
	cRenderList.reserve(1000);
	cConstrainedList.reserve(1000);
	cLogicsList.reserve(1000);
	EntityQueue.reserve(1000);
	EntityToRelease.reserve(1000);
	ConstraintList.reserve(1000);
};


GameObjectManagerClass::~GameObjectManagerClass() 
{
	ConstraintList.clear();
	EntityQueue.clear();
	EntityToRelease.clear();
	IDtoArrayIndex.clear();
	ConstraintIDtoArrayIndex.clear();
	EntityList.clear();
	cBodyList.clear();
	cRenderList.clear();
	cLogicsList.clear();
	cConstrainedList.clear();
		
};


bool GameObjectManagerClass::AddEntity(shared_ptr<Entity> &Entity)
{
	if(Entity!=nullptr) 
	{
			Entity->SetID(IDCounter);
			IDtoArrayIndex.insert(pair<long long,int>(IDCounter,NumberOfEntities));
			EntityList.push_back(Entity);
			cBodyList.push_back(Entity->p_cBody);
			cRenderList.push_back(Entity->p_cRender);
			cConstrainedList.push_back(Entity->p_cConstrained);
			cLogicsList.push_back(Entity->p_cLogics);
			IDCounter++;
			NumberOfEntities++;
		return true;
	} 
	else
	{
		return false;
	};
};


bool GameObjectManagerClass::RemoveEntity(const long long &EntityId) 
{
	auto MapIndex = (IDtoArrayIndex.find(EntityId));

	if (MapIndex == IDtoArrayIndex.end())
	{
		return false;
	}

	auto Index = MapIndex->second;

	if(Index >= 0 && Index < (int)EntityList.size() ) 
	{
		if(	EntityList[ Index] != nullptr ) 
		{

			long long ConstraintIndex = 0;
			while(!EntityList[ Index ]->p_cConstrained->ConstraintsList.empty()) 
			{
				ConstraintIndex = *(EntityList[ Index ]->p_cConstrained->ConstraintsList.begin());
				RemoveConstraint( ConstraintIndex );
			}

			
			auto LastElementIndex = (int)EntityList.size() - 1;//NumberOfEntities - 1;
			if(Index != LastElementIndex)
			{
				auto LastElementID = EntityList[LastElementIndex]->ID;
				auto it2 = IDtoArrayIndex.find(LastElementID);
				if(it2!=IDtoArrayIndex.end())
				{
					it2->second = Index;
				}
				IDtoArrayIndex.erase(MapIndex);
			
				cLogicsList[Index].swap( cLogicsList[LastElementIndex] );
				cBodyList[Index].swap( cBodyList[LastElementIndex] );	
				cRenderList[Index].swap( cRenderList[LastElementIndex] );
				cConstrainedList[Index].swap( cConstrainedList[LastElementIndex] );
				EntityList[Index].swap( EntityList[LastElementIndex] );	
			}
			else
			{
				IDtoArrayIndex.erase(MapIndex);
			}

			NumberOfEntities--;
			
			cLogicsList.pop_back();
			cConstrainedList.pop_back();
			cRenderList.pop_back();
			cBodyList.pop_back();
			EntityList.pop_back();
			
			return true;
		}
		else
		{
			return false;
		}
	} 
	else
	{
		return false;
	}
};


bool GameObjectManagerClass::RemoveEntity(shared_ptr<Entity> &Entity)
{
	return RemoveEntity(Entity->ID);
};

shared_ptr<Entity> GameObjectManagerClass::GetEntity(const long long &EntityId)
{
	auto MapIndex = (IDtoArrayIndex.find(EntityId));
	if(MapIndex != IDtoArrayIndex.end() )
	{ 
		return EntityList[ MapIndex->second ];
	}
	else
	{
		return nullptr;
	}
};


bool GameObjectManagerClass::AddConstraint(shared_ptr<HingeConstraint> &H)
{
	return AddConstraintW( dynamic_pointer_cast<Constraint>(H) );
};


bool GameObjectManagerClass::AddConstraint(shared_ptr<SoftConstraint> &S)
{
	return AddConstraintW( dynamic_pointer_cast<Constraint>(S) );
};


bool GameObjectManagerClass::AddConstraint(shared_ptr<WeldConstraint> &W)
{
	return AddConstraintW( dynamic_pointer_cast<Constraint>(W) );
};


bool GameObjectManagerClass::AddConstraint(shared_ptr<HybridConstraint> &W) 
{
	return AddConstraintW( dynamic_pointer_cast<Constraint>(W) );
};


bool GameObjectManagerClass::AddConstraintW(shared_ptr<Constraint> &Constraint)
{
	if(Constraint!=nullptr) 
	{
		Constraint->ID = IDCounter;
		ConstraintList.push_back(Constraint);

		ConstraintIDtoArrayIndex.insert(pair<long long,int>(IDCounter,NumberOfConstraints));
				
		Constraint->Body1->p_cConstrained->IsConstrained = true;
		Constraint->Body1->p_cConstrained->ConstraintsList.insert(IDCounter);
		
		Constraint->Body2->p_cConstrained->IsConstrained = true;
		Constraint->Body2->p_cConstrained->ConstraintsList.insert(IDCounter);
		
		IDCounter++;
		NumberOfConstraints++;
		
		return true;
	} 
	else
	{
		return false;
	};
};


bool GameObjectManagerClass::RemoveConstraint(const long long &ConstraintId) 
{
	auto MapIndex = (ConstraintIDtoArrayIndex.find(ConstraintId));

	auto Index = MapIndex->second;

	if(Index >= 0 && Index < (int)ConstraintList.size()) 
	{
		if(	ConstraintList[ Index ] != nullptr ) 
		{
			auto LastElementIndex = NumberOfConstraints - 1;
			if(Index != LastElementIndex)
			{
				auto LastElementID = ConstraintList[LastElementIndex]->ID;
				auto it2 = ConstraintIDtoArrayIndex.find(LastElementID);
				if(it2!=ConstraintIDtoArrayIndex.end())
				{
					it2->second = Index;
				}
				
				ConstraintList[Index].swap( ConstraintList[LastElementIndex] );				
			}
			
			ConstraintIDtoArrayIndex.erase(MapIndex);
			
			ConstraintList[ LastElementIndex ]->Body1->p_cConstrained->ConstraintsList.erase(ConstraintId);
			if(ConstraintList[ LastElementIndex ]->Body1->p_cConstrained->ConstraintsList.empty()) 
			{
				ConstraintList[ LastElementIndex ]->Body1->p_cConstrained->IsConstrained = false;
			};

			ConstraintList[ LastElementIndex ]->Body2->p_cConstrained->ConstraintsList.erase(ConstraintId);
			if(ConstraintList[ LastElementIndex ]->Body2->p_cConstrained->ConstraintsList.empty()) 
			{ 
				ConstraintList[ LastElementIndex ]->Body2->p_cConstrained->IsConstrained = false;
			};

			NumberOfConstraints--;
			ConstraintList[LastElementIndex].reset();
			ConstraintList.pop_back();
			return true;
		} 
		else
		{
			return false;
		};
	} 
	else
	{
		return false;
	};
};


bool GameObjectManagerClass::RemoveConstraint(shared_ptr<Constraint> &C)
{
	return RemoveConstraint(C->ID);
};


shared_ptr<Constraint> GameObjectManagerClass::GetConstraint(const long long &ConstraintId) 
{
	if( ConstraintId >= 0 && ConstraintId < (int)ConstraintList.size() ) 
	{
		bool IsInside = ConstraintIDtoArrayIndex.end()!=ConstraintIDtoArrayIndex.find( ConstraintId );
		if(IsInside) 
		{
			return ConstraintList[ ConstraintId ];
		}
		else 
		{
			return nullptr;
		};
	}
	else
	{
		return nullptr;
	};
};

void GameObjectManagerClass::CleanUp()
{
	for each(auto Id in EntityToRelease)
	{
		RemoveEntity(Id);
	}
	EntityToRelease.clear();
};

void GameObjectManagerClass::AddEntityToRelease(long long& Value)
{
	EntityToRelease.push_back(Value);
};


void GameObjectManagerClass::ClearAll() 
{		
	ConstraintList.clear();
	EntityQueue.clear();
	EntityToRelease.clear();
	IDtoArrayIndex.clear();
	ConstraintIDtoArrayIndex.clear();

	EntityList.clear();
	cBodyList.clear();
	cRenderList.clear();
	cLogicsList.clear();
	cConstrainedList.clear();
};


void GameObjectManagerClass::InsertIntoQueue(shared_ptr<Entity>& Entity)
{
	EntityQueue.push_back(Entity);	
};


void GameObjectManagerClass::ClearQueue()
{
	for(unsigned ii=0; ii < EntityQueue.size(); ++ii)
	{
		AddEntity(EntityQueue[ii]);
	}
	EntityQueue.clear();
};


void GameObjectManagerClass::Update()
{
	CleanUp();
	ClearQueue();
};
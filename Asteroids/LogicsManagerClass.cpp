#pragma once
#include "stdafx.h"
#include "LogicsManagerClass.h"
#include "GameEngineClass.h"


LogicsManagerClass::LogicsManagerClass() 
{

};


LogicsManagerClass::~LogicsManagerClass() 
{ 
	ContactPairsMap.clear();
	ContactEventsMap.clear();
};


void LogicsManagerClass::Update(vector<ContactSolution>& PhysicalContactList,unsigned PhysicalContactListSize, vector<ContactSolution>& LogicalContactList,unsigned LogicalContactListSize, vector<KeyDefinitions>&	KeyPressedList, GameObjectManagerClass& ObjectManager) 
{ 
	
						
	vector<shared_ptr<cBody>>& BodyList = ObjectManager.GetcBodyList();
	vector<shared_ptr<cLogics>>& LogicsList = ObjectManager.GetcLogicsList();
	ContactEventsMap.clear();

	unsigned CurrentListSize = LogicsList.size();

	for(unsigned ii=0; ii<LogicalContactListSize; ++ii)
	{
		if( BodyList[ LogicalContactList[ii].Body1 ]->GetCollisionType() != Physical && LogicsList[ LogicalContactList[ii].Body1 ]->GetIsHandlingContacts() )
		{
			ContactEventsMap[LogicalContactList[ii].Body1].push_back(ContactEvent(LogicalContactList[ii].Body1, LogicalContactList[ii].Face1, LogicalContactList[ii].Body2, LogicalContactList[ii].Face2, LogicalContactList[ii]));
		}

		if( BodyList[ LogicalContactList[ii].Body2 ]->GetCollisionType() != Physical && LogicsList[ LogicalContactList[ii].Body2 ]->GetIsHandlingContacts() )
		{
			ContactEventsMap[LogicalContactList[ii].Body2].push_back(ContactEvent(LogicalContactList[ii].Body2, LogicalContactList[ii].Face2, LogicalContactList[ii].Body1, LogicalContactList[ii].Face1, LogicalContactList[ii]));
		}
	}

	for(unsigned ii=0; ii<PhysicalContactListSize; ++ii)
	{
		if( BodyList[ PhysicalContactList[ii].Body1 ]->GetCollisionType() == Both  && LogicsList[ LogicalContactList[ii].Body1 ]->GetIsHandlingContacts() )
		{
			ContactEventsMap[PhysicalContactList[ii].Body1].push_back( ContactEvent(PhysicalContactList[ii].Body1, PhysicalContactList[ii].Face1, PhysicalContactList[ii].Body2, PhysicalContactList[ii].Face2, PhysicalContactList[ii]) ); 
		}

		if( BodyList[ PhysicalContactList[ii].Body2 ]->GetCollisionType() == Both  && LogicsList[ LogicalContactList[ii].Body2 ]->GetIsHandlingContacts() )
		{
			ContactEventsMap[PhysicalContactList[ii].Body2].push_back( ContactEvent(PhysicalContactList[ii].Body2, PhysicalContactList[ii].Face2, PhysicalContactList[ii].Body1, PhysicalContactList[ii].Face1, PhysicalContactList[ii]) ); 
		}
	}


	if(!ContactEventsMap.empty())
	{
		for(auto it = ContactEventsMap.begin(); it!=ContactEventsMap.end(); ++it)
		{
			if(LogicsList[ it->first ]->GetIsHandlingContacts())
			{
				LogicsList[ it->first ]->HandleContacts( it->second, ObjectManager);
			}
		}
	}

	
	for(unsigned ii=0; ii<CurrentListSize; ++ii)
	{
		if(LogicsList[ii]->GetIsHandlingInputs())
		{
			LogicsList[ii]->HandleInputs(KeyPressedList, ObjectManager);
		}
	}


	for(unsigned ii=0; ii<CurrentListSize; ++ii)
	{
		LogicsList[ii]->Update(ObjectManager);
	}

};
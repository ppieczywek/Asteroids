#pragma once
#include "stdafx.h"
#include "WorldPropertiesClass.h"
#include "CollisionManagerClass.h"
#include "WorldPropertiesClass.h"
#include "ConstraintManagerClass.h"
#include "GameObjectManagerClass.h"
#include "InputManagerClass.h"
#include "ContactEventClass.h"
#include <map>
#include <vector>
using namespace std;


class LogicsManagerClass
{
	map<int, vector<ContactEvent>> ContactEventsMap;
	map<int, vector<int>>		   ContactPairsMap;
	
public:
			
								LogicsManagerClass();
								~LogicsManagerClass();
	
	void						Update(vector<ContactSolution>& PhysicalContactList, unsigned, vector<ContactSolution>& LogicalContactList,unsigned, vector<KeyDefinitions>& KeyPressedList, GameObjectManagerClass& ObjectManager);
};
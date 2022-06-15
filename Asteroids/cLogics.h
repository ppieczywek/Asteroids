#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "InputManagerClass.h"
#include "ContactEventClass.h"
#include <vector>

using namespace std;

enum EntityType {DefaultE, PlayerE, PlayerProj, PlatformE, Junk, AsteroidE, StarE, IconE};

class cLogics 
{
protected:
	long long	ID;
	bool		IsHandlingContacts;
	bool		IsHandlingInputs;
	EntityType  EntityT;

	string		CurrentAction;
	
public:	

	friend class Physics2D;
	friend class CollisionManagerClass;
	friend class ConstraintManagerClass;
	friend class GameObjectManagerClass;
	friend class PhysicsManagerClass;
	friend class SoftConstraint;
	friend class HingeConstraint;
	friend class WeldConstraint;
	
							cLogics(); 
	virtual					~cLogics(); 
	inline void				SetID(const long long&);
	inline void				SetIsHandlingContacts(bool&);
	inline void				SetIsHandlingInputs(bool&);
	inline void				SetCurrentAction(string&);
	inline void				SetEntityType(EntityType&);

	inline long long		GetID(); 
	inline bool				GetIsHandlingContacts();
	inline bool				GetIsHandlingInputs();
	inline EntityType		GetEntityType();
	inline string			GetCurrentAction();

	virtual void			HandleInputs(vector<KeyDefinitions>&, GameObjectManagerClass&) {};
	virtual void			HandleContacts(vector<ContactEvent>&, GameObjectManagerClass&) {};
	virtual void			Update(GameObjectManagerClass& ) {};
	
};

void cLogics::SetEntityType(EntityType& value)
{
	EntityT = value;
}

void cLogics::SetCurrentAction(string& Value)
{
	CurrentAction = Value;
}

string cLogics::GetCurrentAction()
{
	return CurrentAction;
}

void cLogics::SetID(const long long& NewID) 
{
	ID = NewID;
};


EntityType	cLogics::GetEntityType()
{
	return EntityT;
};


long long	cLogics::GetID() 
{
	return ID;
};


void cLogics::SetIsHandlingContacts(bool& Value)
{
	IsHandlingContacts = Value;
};


void cLogics::SetIsHandlingInputs(bool& Value)
{
	IsHandlingInputs = Value;
};


bool cLogics::GetIsHandlingContacts()
{
	return IsHandlingContacts;
};


bool cLogics::GetIsHandlingInputs()
{
	return IsHandlingInputs;
};




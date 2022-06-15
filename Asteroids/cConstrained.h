#pragma once
#include "Vector2.h"
#include "Entity.h"
#include <vector>
#include <set>
using namespace std;

class cConstrained {
protected:
	long long		ID;
	set<long long>	ConstraintsList;
	bool			IsConstrained;
public:	

	friend class Physics2D;
	friend class CollisionManagerClass;
	friend class ConstraintManagerClass;
	friend class GameObjectManagerClass;
	friend class PhysicsManagerClass;
	friend class SoftConstraint;
	friend class HingeConstraint;
	friend class WeldConstraint;
	
							cConstrained(); 
	virtual					~cConstrained(); 
	inline void				SetID(const long long&); 
	inline long long		GetID(); 

};

void cConstrained::SetID(const long long& NewID) 
{
	ID = NewID;
};


long long	cConstrained::GetID() 
{
	return ID;
};
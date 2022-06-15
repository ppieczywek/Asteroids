#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "cConstrained.h"
#include "cLogics.h"
#include <vector>
using namespace std;

class Star : public Entity 
{
public:
	Star(float);
	~Star();
};


class StarLogics : public cLogics
{		
	Vector3					BodyColor;
	float					DeviationAmplitude;
	float					DeviationPeriod;
	float					PhaseShift;
	float					Time;

	public:	

	friend class			Physics2D;
	friend class			CollisionManagerClass;
	friend class			ConstraintManagerClass;
	friend class			GameObjectManagerClass;
	friend class			PhysicsManagerClass;
	friend class			SoftConstraint;
	friend class			HingeConstraint;
	friend class			WeldConstraint;
	
							StarLogics(Vector3); 
							~StarLogics(); 
	
	virtual void			HandleInputs(vector<KeyDefinitions>&, GameObjectManagerClass&);
	virtual void			HandleContacts(vector<ContactEvent>&, GameObjectManagerClass&);
	virtual void			Update(GameObjectManagerClass&);
};


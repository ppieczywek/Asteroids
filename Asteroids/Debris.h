#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "cConstrained.h"
#include "cLogics.h"
#include <vector>
using namespace std;

class Debris : public Entity {
public:
	Debris(float, float, float, float);
	~Debris();
};


class DebrisLogics : public cLogics
{	
	float LifeSpan;
	float TotalLifeSpan;

	public:	

	friend class Physics2D;
	friend class CollisionManagerClass;
	friend class ConstraintManagerClass;
	friend class GameObjectManagerClass;
	friend class PhysicsManagerClass;
	friend class SoftConstraint;
	friend class HingeConstraint;
	friend class WeldConstraint;

							DebrisLogics(float); 
							~DebrisLogics(); 
	
	virtual void			HandleInputs(vector<KeyDefinitions>&, GameObjectManagerClass&);
	virtual void			HandleContacts(vector<ContactEvent>&, GameObjectManagerClass&);
	virtual void			Update(GameObjectManagerClass&);


};

void CreateExplosion(const int&, const Vector2&, const Vector2&, GameObjectManagerClass&);


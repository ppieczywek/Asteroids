#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "cConstrained.h"
#include "cLogics.h"
#include <vector>
using namespace std;

class PlayerProjectile : public Entity 
{
public:
	PlayerProjectile(Vector2, float);
	~PlayerProjectile();
};


class ProjectileLogics : public cLogics
{	
	float LifeSpan;

	public:	

	friend class Physics2D;
	friend class CollisionManagerClass;
	friend class ConstraintManagerClass;
	friend class GameObjectManagerClass;
	friend class PhysicsManagerClass;
	friend class SoftConstraint;
	friend class HingeConstraint;
	friend class WeldConstraint;
	
							ProjectileLogics(); 
							~ProjectileLogics(); 
	
	virtual void			HandleInputs(vector<KeyDefinitions>&, GameObjectManagerClass&);
	virtual void			HandleContacts(vector<ContactEvent>&, GameObjectManagerClass&);
	virtual void			Update(GameObjectManagerClass&);


};

class PlayerProjectileRender : public cRender
{
public:
		
						 PlayerProjectileRender() {};
						~PlayerProjectileRender() {};

	virtual void		Draw(const vector<Polygon2>&, const unsigned&);
};
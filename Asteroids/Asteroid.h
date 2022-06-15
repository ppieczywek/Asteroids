#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "cConstrained.h"
#include "cLogics.h"
#include "dynamic_array.h"
#include <vector>
using namespace std;

const int	AsteroidMaxBlockNumber = 12;
const float AsteroidMaxBlockSize = 1.9f;


class Asteroid : public Entity 
{
public:
	Asteroid();
	Asteroid(vector<Polygon2>& PolygonList);
	~Asteroid();
};


class AsteroidLogics : public cLogics
{	
	int Energy;
	
	public:	

	friend class Physics2D;
	friend class CollisionManagerClass;
	friend class ConstraintManagerClass;
	friend class GameObjectManagerClass;
	friend class PhysicsManagerClass;
	friend class SoftConstraint;
	friend class HingeConstraint;
	friend class WeldConstraint;
	
							AsteroidLogics(); 
							~AsteroidLogics(); 
	void					SetEnergy(const int&);
	virtual void			HandleInputs(vector<KeyDefinitions>&, GameObjectManagerClass&);
	virtual void			HandleContacts(vector<ContactEvent>&, GameObjectManagerClass&);
	virtual void			Update(GameObjectManagerClass&);


};
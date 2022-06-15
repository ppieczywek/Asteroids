#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "cConstrained.h"
#include "cLogics.h"
#include <vector>
using namespace std;

class PlayerRocket : public Entity {
public:
	PlayerRocket();
	~PlayerRocket();
};


class PlayerLogics : public cLogics
{

	
	int		Charge;
	int		ReleaseRate;
	float	ImmuneTime;
	bool	IsImmune;
	bool	WasRotateButtonPressed;



	public:	

	friend class Physics2D;
	friend class CollisionManagerClass;
	friend class ConstraintManagerClass;
	friend class GameObjectManagerClass;
	friend class PhysicsManagerClass;
	friend class SoftConstraint;
	friend class HingeConstraint;
	friend class WeldConstraint;
	
							PlayerLogics(); 
							~PlayerLogics(); 
	
	virtual void			HandleInputs(vector<KeyDefinitions>&, GameObjectManagerClass&);
	virtual void			HandleContacts(vector<ContactEvent>&, GameObjectManagerClass&);
	virtual void			Update(GameObjectManagerClass&);
};
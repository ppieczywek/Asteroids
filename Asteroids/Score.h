#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "cConstrained.h"
#include "cLogics.h"
#include <vector>
#include <string>
using namespace std;

class Score : public Entity 
{
public:
	Score(int);
	~Score();
};

class ScoreLogics : public cLogics
{		
	float						LifeSpan;

	public:	

	friend class			Physics2D;
	friend class			CollisionManagerClass;
	friend class			ConstraintManagerClass;
	friend class			GameObjectManagerClass;
	friend class			PhysicsManagerClass;
	friend class			SoftConstraint;
	friend class			HingeConstraint;
	friend class			WeldConstraint;
	
							ScoreLogics(); 
							~ScoreLogics(); 
	
	virtual void			HandleInputs(vector<KeyDefinitions>&, GameObjectManagerClass&) {};
	virtual void			HandleContacts(vector<ContactEvent>&, GameObjectManagerClass&) {};
	virtual void			Update(GameObjectManagerClass&);
};

class ScoreRender : public cRender
{
	string Value;
	float  Scale;
	float  TextOffset;
public:		
				ScoreRender(int);
				~ScoreRender() {};

	virtual void Draw(const vector<Polygon2>&, const unsigned&);
};
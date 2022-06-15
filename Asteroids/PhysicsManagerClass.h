#pragma once
#include "Vector2.h"
#include "WorldPropertiesClass.h"
#include "ConstraintManagerClass.h"
#include "CollisionManagerClass.h"
#include "GameObjectManagerClass.h"
#include <set>
#include <vector>
using namespace std;

class PhysicsManagerClass
{
	int							SolverIterations;
	Vector2						PrevPos;
	float						PrevAngle;
	void						ApplyForces(vector<shared_ptr<cBody>>&);
	void						IntegrateImpulses(vector<shared_ptr<cBody>>   &BodyList);
public:

	WorldPropertiesClass		WorldProperties;
	CollisionManagerClass		CollisionManager;
	ConstraintManagerClass		ConstraintManager;
		
								PhysicsManagerClass();
								~PhysicsManagerClass();

	void						Update(GameObjectManagerClass&);
	
	void						SetSolverIterations(const int &Iterations) { SolverIterations = Iterations > 0 ? Iterations : 1; };
	int							GetSolverIterations()						{ return SolverIterations; };
};


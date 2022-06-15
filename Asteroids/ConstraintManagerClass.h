#pragma once
#include "Vector2.h"
#include "Box2D.h"
#include "WorldPropertiesClass.h"
#include "GameObjectManagerClass.h"
#include "HingeConstraint.h"
#include "SoftConstraint.h"
#include "HybridConstraint.h"
#include <set>
#include <vector>

class ConstraintManagerClass {
		
public:
	friend class GameEngineClass;
	ConstraintManagerClass() {};
	~ConstraintManagerClass() {};
	
	void SolveConstraints(vector<shared_ptr<Constraint>>& ConstraintList, float TimeStep);
};
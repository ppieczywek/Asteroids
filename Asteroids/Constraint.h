#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "cConstrained.h"
#include <vector>
#include <set>

enum ConstraintType {SConstraint, HConstraint, WConstraint};

class Constraint {
protected:
	long long				ID;
	const ConstraintType	cType;
	shared_ptr<Entity>		Body1;
	shared_ptr<Entity>		Body2;
	
							Constraint(const ConstraintType&, shared_ptr<Entity>&, shared_ptr<Entity>&);
	virtual void			SolveVelocityConstraint(const float&) = 0;
	virtual void			SolvePositionConstraint(const float&) = 0;
	virtual void			Initialize(const float&) = 0;
	virtual void			ApplyImpulse() = 0;
	virtual ConstraintType  GetConstraintType();

public:

	friend class			ConstraintManagerClass;
	friend class			GameObjectManagerClass;
	friend class			Physics2D;
	virtual					~Constraint();
};

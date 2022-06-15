#pragma once
#include "stdafx.h"
#include "RotationMatrix.h"
#include "TransformationMatrix.h"
#include "Constraint.h"
#include "Matrix22.h"

//revolute joint
class HingeConstraint : public Constraint 
{		
	float			TotalInvMass;
	float			R1x2;
	float			R2x2;
	float			R1y2;
	float			R2y2;
	float			R1xy;
	float			R2xy;
	Vector2			LocalAnchor1;
	Vector2			LocalAnchor2;
	Vector2			Impulse;
	Vector2			tmpLocalAnchor1;
	Vector2			tmpLocalAnchor2;
	Vector2			RelativeVelocity;
	Vector2			R1;
	Vector2			R2;
	Vector2			Correction;
	Matrix22		M; 
			
	virtual void	SolveVelocityConstraint(const float&);
	virtual void	SolvePositionConstraint(const float&);
	virtual void	Initialize(const float&);
	virtual void	ApplyImpulse();

public:
	friend class	ConstraintManagerClass;
					HingeConstraint(shared_ptr<Entity>&, shared_ptr<Entity>&, const Vector2&);
};
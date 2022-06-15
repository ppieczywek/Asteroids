#pragma once
#include "stdafx.h"
#include "RotationMatrix.h"
#include "TransformationMatrix.h"
#include "Constraint.h"
#include "Matrix33.h"

class WeldConstraint : public Constraint
{	
	float			AngularImpulse;
	float			RelativeAngle;
	float			TotalInvMass;
	float			R1x2;
	float			R2x2;
	float			R1y2;
	float			R2y2;
	float			R1xy;
	float			R2xy;
	Vector2			LocalAnchor1;
	Vector2			LocalAnchor2;
	Vector2			LinearImpulse;
	Vector2			tmpLocalAnchor1;
	Vector2			tmpLocalAnchor2;
	Vector2			R1;
	Vector2			R2;
	Vector3			TotalImpulse;
	Vector3			Correction;
	Vector3			RelativeVelocity;
	Matrix33		M; 
		
	virtual void	SolveVelocityConstraint(const float&);
	virtual void	SolvePositionConstraint(const float&);
	virtual void	Initialize(const float&);
	virtual void	ApplyImpulse();

public:
	friend class	ConstraintManagerClass;
					WeldConstraint(shared_ptr<Entity>&, shared_ptr<Entity>&, const Vector2&);
};
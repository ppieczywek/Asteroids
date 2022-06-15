#pragma once
#include "stdafx.h"
#include "RotationMatrix.h"
#include "TransformationMatrix.h"
#include "Constraint.h"
#include "Matrix22.h"
#include "Matrix33.h"

class HybridConstraint : public Constraint 
{
	bool			IsLimited;
	bool			Exceedes;
	float			R1x2;
	float			R2x2;
	float			R1y2;
	float			R2y2;
	float			R1xy;
	float			R2xy;
	float			CurrentAngle;
	float			AngularImpulse;
	float			RelativeAngle;
	float			TotalInvMass;
	float			ToleranceAngle;
	float			UpperLimit;
	float			LowerLimit;
	Vector2			LocalAnchor1;
	Vector2			LocalAnchor2;
	Vector2			LinearImpulse;
	Vector2			Impulse;
	Vector2			RelativeVelocity22;
	Vector2			tmpLocalAnchor1;
	Vector2			tmpLocalAnchor2;
	Vector2			R1;
	Vector2			R2;
	Vector2			Correction22;
	Vector3			TotalImpulse;
	Vector3			Correction33;
	Vector3			RelativeVelocity33;
	Matrix22		M2; 
	Matrix33		M3; 
			
	virtual void	SolveVelocityConstraint(const float&);
	virtual void	SolvePositionConstraint(const float&);
	virtual void	Initialize(const float&);
	virtual void	ApplyImpulse();

public:
	
	friend class	ConstraintManagerClass;
					HybridConstraint(shared_ptr<Entity>&, shared_ptr<Entity>&, const Vector2&);
	virtual void	SetLimit(const float&);
};
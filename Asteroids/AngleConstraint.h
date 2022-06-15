#pragma once
#include "stdafx.h"
#include "TransformationMatrix.h"
#include "Constraint.h"
#include "Matrix22.h"

class AngleConstraint : public Constraint {
			
	Vector2 LocalAnchor1;
	Vector2 LocalAnchor2;
	float AngularImpulse;
	
	float RelativeAngle;
	float TotalInvMass;

	float Correction;
	float RelativeVelocity;

	Vector2 tmpLocalAnchor1, tmpLocalAnchor2, R1, R2;
	//float R1x2, R2x2, R1y2, R2y2, R1xy, R2xy;
	
	virtual void SolveVelocityConstraint(const float &TimeStep);
	virtual void SolvePositionConstraint(const float &TimeStep);
	virtual void Initialize(const float &TimeStep);
	virtual void ApplyImpulse();

public:
	friend class ConstraintManagerClass;
	AngleConstraint(shared_ptr<Entity> &Body1,shared_ptr<Entity> &Body2, const Vector2 &Anchor);*/
};
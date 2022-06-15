#pragma once
#include "stdafx.h"
#include "RotationMatrix.h"
#include "TransformationMatrix.h"
#include "Constraint.h"

class SoftConstraint : public Constraint {
		
	float			Omega;
	float			DampingRatio;
	float			Frequency;
	float			RestLength;
	float			TotalInvMass;
	float			ImpulseMagnitude;
	float			gamma;
	float			beta;
	float			c;
	float			k;
	float			h;
	float			Elongation;
	float			tmpImpulseMagnitude;
	float			EffectiveMass;
	float			RN1;
	float			RN2;
	Vector2			LocalAnchor1;
	Vector2			LocalAnchor2;
	Vector2			Impulse;
	Vector2			tmpAnchor1;
	Vector2			tmpAnchor2;
	Vector2			RelativeVelocity;
	Vector2			R1;
	Vector2			R2;
	Vector2			Direction;
	Vector2			Correction;
	
	virtual void	SolveVelocityConstraint(const float&);
	virtual void	SolvePositionConstraint(const float&);
	virtual void	Initialize(const float&);
	virtual void	ApplyImpulse();
	
public:
	
	friend class	ConstraintManagerClass;
					SoftConstraint(shared_ptr<Entity>&, const Vector2&, shared_ptr<Entity>&, const Vector2&);

	void			SetFrequency(const float&);
	void			SetDampingRatio(const float&);
	inline float	GetFrequency();
	inline float	GetDampingRatio();
};

float SoftConstraint::GetFrequency()
{
	return Frequency;
};


float SoftConstraint::GetDampingRatio() 
{
	return DampingRatio;
}; 
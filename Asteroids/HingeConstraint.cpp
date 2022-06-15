#pragma once
#include "stdafx.h"
#include "HingeConstraint.h"

HingeConstraint::HingeConstraint(shared_ptr<Entity> &B1, shared_ptr<Entity> &B2, const Vector2 &Anchor) : Constraint(WConstraint, B1, B2) 
{

	RotationMatrix Rot1( -Body1->p_cBody->Angle);
	LocalAnchor1 = Rot1 * (Anchor - Body1->p_cBody->Position);


	RotationMatrix Rot2( -Body2->p_cBody->Angle);
	LocalAnchor2 = Rot2 * (Anchor - Body2->p_cBody->Position);

	TotalInvMass = B1->p_cBody->InverseMass + B2->p_cBody->InverseMass;

	tmpLocalAnchor1 = tmpLocalAnchor2 = RelativeVelocity = R1 = R2 = Correction = ZeroVector;
	R1x2 = R2x2 = R1y2 = R2y2 = R1xy = R2xy;
};


void HingeConstraint::Initialize(const float &TimeStep) 
{	
	tmpLocalAnchor1 = TransformationMatrix(Body1->p_cBody->Angle, Body1->p_cBody->Position) * LocalAnchor1; 
	tmpLocalAnchor2 = TransformationMatrix(Body2->p_cBody->Angle, Body2->p_cBody->Position) * LocalAnchor2; 
		
	R1 = tmpLocalAnchor1 - Body1->p_cBody->Position;
	R2 = tmpLocalAnchor2 - Body2->p_cBody->Position;
		
	R1x2 = R1.x*R1.x;
	R2x2 = R2.x*R2.x;
	R1y2 = R1.y*R1.y;
	R2y2 = R2.y*R2.y;
	R1xy = R1.x*R1.y;
	R2xy = R2.x*R2.y;
	
	M.a11 = TotalInvMass + Body1->p_cBody->InverseInertia * R1y2 + Body2->p_cBody->InverseInertia * R2y2;
	M.a12 = M.a21 = -Body1->p_cBody->InverseInertia * R1xy - Body2->p_cBody->InverseInertia * R2xy; 
	M.a22 = TotalInvMass + Body1->p_cBody->InverseInertia * R1x2 + Body2->p_cBody->InverseInertia * R2x2;
};


void HingeConstraint::SolveVelocityConstraint(const float &TimeStep) 
{	
	RelativeVelocity = Body2->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor2) - Body1->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor1);
	if( M.Inverse() ) 
	{
		Impulse = -(M*RelativeVelocity);
	} 
	else
	{
		Impulse = ZeroVector;
	};
};


void HingeConstraint::ApplyImpulse() 
{
	Body1->p_cBody->ApplyImpulse( -Impulse, tmpLocalAnchor1);
	Body2->p_cBody->ApplyImpulse(  Impulse, tmpLocalAnchor2);
};


void HingeConstraint::SolvePositionConstraint(const float &TimeStep) 
{
	Correction = 0.5f*(tmpLocalAnchor2 -  tmpLocalAnchor1);
	
	Correction/=TimeStep;
	RelativeVelocity = Body2->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor2) - Body1->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor1);
		
		
	Correction-=RelativeVelocity;
	//if(Correction.length()>0.01f) {
	Impulse = -(M*Correction);
	Body1->p_cBody->ApplyImpulse( -Impulse, tmpLocalAnchor1);
	Body2->p_cBody->ApplyImpulse(  Impulse, tmpLocalAnchor2);
};


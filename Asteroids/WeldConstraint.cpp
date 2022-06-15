#pragma once
#include "stdafx.h"
#include "WeldConstraint.h"

WeldConstraint::WeldConstraint(shared_ptr<Entity> &B1, shared_ptr<Entity> &B2, const Vector2 &Anchor) : Constraint(SConstraint, B1, B2)
{
	RotationMatrix R1( -Body1->p_cBody->Angle);
	LocalAnchor1 = R1 * (Anchor - Body1->p_cBody->Position);


	RotationMatrix R2( -Body2->p_cBody->Angle);
	LocalAnchor2 = R2 * (Anchor - Body2->p_cBody->Position);

	RelativeAngle = B2->p_cBody->Angle - B1->p_cBody->Angle;	
	TotalInvMass = B1->p_cBody->InverseMass + B2->p_cBody->InverseMass;		
};

void WeldConstraint::Initialize(const float &TimeStep) 
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
	M.a13 = -Body1->p_cBody->InverseInertia * R1.y - Body2->p_cBody->InverseInertia * R2.y; 
	M.a22 = TotalInvMass + Body1->p_cBody->InverseInertia * R1x2 + Body2->p_cBody->InverseInertia * R2x2;
	M.a32 = M.a23 = Body1->p_cBody->InverseInertia * R1.x + Body2->p_cBody->InverseInertia * R2.x; 
	M.a31 = M.a13; 
	M.a33 = Body1->p_cBody->InverseInertia + Body2->p_cBody->InverseInertia; 
};

void WeldConstraint::SolveVelocityConstraint(const float &TimeStep) 
{
	auto RelativeLinearVelocity = Body2->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor2) - Body1->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor1);
	RelativeVelocity.x = RelativeLinearVelocity.x;
	RelativeVelocity.y = RelativeLinearVelocity.y;
	RelativeVelocity.z = Body2->p_cBody->AngularVelocity - Body1->p_cBody->AngularVelocity; 
		
	if( M.Inverse() ) 
	{
		TotalImpulse = -(M*RelativeVelocity);
		LinearImpulse.x = TotalImpulse.x;
		LinearImpulse.y = TotalImpulse.y;
		AngularImpulse = TotalImpulse.z;
	}
	else 
	{
		LinearImpulse = ZeroVector;
		AngularImpulse = 0.0f;
	};
};

void WeldConstraint::ApplyImpulse() 
{
	Body1->p_cBody->ApplyImpulse( -LinearImpulse, tmpLocalAnchor1);
	Body2->p_cBody->ApplyImpulse(  LinearImpulse, tmpLocalAnchor2);

	Body1->p_cBody->ApplyAngularVelocity( -AngularImpulse*Body1->p_cBody->InverseInertia);
	Body2->p_cBody->ApplyAngularVelocity(  AngularImpulse*Body2->p_cBody->InverseInertia);
};

void WeldConstraint::SolvePositionConstraint(const float &TimeStep) 
{
	auto RelativeLinearVelocity = Body2->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor2) - Body1->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor1);
	RelativeVelocity.x = RelativeLinearVelocity.x;
	RelativeVelocity.y = RelativeLinearVelocity.y;
	RelativeVelocity.z = Body2->p_cBody->AngularVelocity - Body1->p_cBody->AngularVelocity; 
		
	Correction.x = 1.0f*(tmpLocalAnchor2.x -  tmpLocalAnchor1.x);
	Correction.y = 1.0f*(tmpLocalAnchor2.y -  tmpLocalAnchor1.y);
	Correction.z = 1.0f*(Body2->p_cBody->Angle - Body1->p_cBody->Angle) - RelativeAngle ;
	Correction/=TimeStep;
	Correction-=RelativeVelocity;

	TotalImpulse = -(M*Correction);
	LinearImpulse.x = TotalImpulse.x;
	LinearImpulse.y = TotalImpulse.y;
	AngularImpulse = TotalImpulse.z;

	//if(RelativeVelocity.length() > 0.01f) {
	Body1->p_cBody->ApplyImpulse( -LinearImpulse, tmpLocalAnchor1);
	Body2->p_cBody->ApplyImpulse(  LinearImpulse, tmpLocalAnchor2);
	Body1->p_cBody->ApplyAngularVelocity( -AngularImpulse*Body1->p_cBody->InverseInertia);
	Body2->p_cBody->ApplyAngularVelocity(  AngularImpulse*Body2->p_cBody->InverseInertia);
	//};
};

#pragma once
#include "stdafx.h"
#include "HybridConstraint.h"

HybridConstraint::HybridConstraint(shared_ptr<Entity> &B1, shared_ptr<Entity> &B2, const Vector2 &Anchor) : Constraint(SConstraint, B1, B2) 
{	
	RotationMatrix Rot1( -Body1->p_cBody->Angle);
	LocalAnchor1 = Rot1 * (Anchor - Body1->p_cBody->Position);


	RotationMatrix Rot2( -Body2->p_cBody->Angle);
	LocalAnchor2 = Rot2 * (Anchor - Body2->p_cBody->Position);
		

	RelativeAngle = B2->p_cBody->Angle - B1->p_cBody->Angle;	
	TotalInvMass = B1->p_cBody->InverseMass + B2->p_cBody->InverseMass;
	
	Exceedes = IsLimited = false; //ma dzia³aæ jak revolute joint
	

	tmpLocalAnchor1 = tmpLocalAnchor2 =  R1 = R2 = ZeroVector;
	ToleranceAngle = UpperLimit = LowerLimit = R1x2 = R2x2 = R1y2 = R2y2 = R1xy = R2xy;	
};

void HybridConstraint::SetLimit(const float &Limit) 
{
	IsLimited = true;
	ToleranceAngle = Limit;
	UpperLimit = RelativeAngle + Limit;
	LowerLimit = RelativeAngle - Limit;
};

void HybridConstraint::Initialize(const float &TimeStep)
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
	
	CurrentAngle = (Body2->p_cBody->Angle - Body1->p_cBody->Angle);
	
	if(IsLimited==true) {
	if(CurrentAngle < LowerLimit ||  CurrentAngle > UpperLimit) 
		Exceedes = true;
	else
		Exceedes = false;
	};
	if(IsLimited == false ||  (IsLimited == true && Exceedes == false) ){
		M2.a11 = TotalInvMass + Body1->p_cBody->InverseInertia * R1y2 + Body2->p_cBody->InverseInertia * R2y2;
		M2.a12 = M2.a21 = -Body1->p_cBody->InverseInertia * R1xy - Body2->p_cBody->InverseInertia * R2xy; 
		M2.a22 = TotalInvMass + Body1->p_cBody->InverseInertia * R1x2 + Body2->p_cBody->InverseInertia * R2x2;
	};

	if(IsLimited == true && Exceedes == true) {
		M3.a11 = TotalInvMass + Body1->p_cBody->InverseInertia * R1y2 + Body2->p_cBody->InverseInertia * R2y2;
		M3.a12 = M3.a21 = -Body1->p_cBody->InverseInertia * R1xy - Body2->p_cBody->InverseInertia * R2xy; 
		M3.a13 = -Body1->p_cBody->InverseInertia * R1.y - Body2->p_cBody->InverseInertia * R2.y; 
		M3.a22 = TotalInvMass + Body1->p_cBody->InverseInertia * R1x2 + Body2->p_cBody->InverseInertia * R2x2;
		M3.a32 = M3.a23 = Body1->p_cBody->InverseInertia * R1.x + Body2->p_cBody->InverseInertia * R2.x; 
		M3.a31 = M3.a13; 
		M3.a33 = Body1->p_cBody->InverseInertia + Body2->p_cBody->InverseInertia; 
	};
};


void HybridConstraint::SolveVelocityConstraint(const float &TimeStep) 
{
	if(IsLimited == false ||  (IsLimited == true && Exceedes == false) )
	{
		RelativeVelocity22 = Body2->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor2) - Body1->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor1);
		if( M2.Inverse() ) 
		{
			Impulse = -(M2*RelativeVelocity22);
			AngularImpulse = 0.0f;
		} 
		else
		{
			Impulse = ZeroVector;
			AngularImpulse = 0.0f;
		};
	};

	if(IsLimited == true && Exceedes == true) 
	{
		auto RelativeLinearVelocity = Body2->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor2) - Body1->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor1);
		RelativeVelocity33.x = RelativeLinearVelocity.x;
		RelativeVelocity33.y = RelativeLinearVelocity.y;
		RelativeVelocity33.z = Body2->p_cBody->AngularVelocity - Body1->p_cBody->AngularVelocity;// + ((ToleranceAngle-CurrentAngle)/TimeStep)*0.1; 
		
		if( M3.Inverse() ) 
		{
			TotalImpulse = -(M3*RelativeVelocity33);
			LinearImpulse.x = TotalImpulse.x;
			LinearImpulse.y = TotalImpulse.y;
			AngularImpulse =  TotalImpulse.z;
		} 
		else
		{
			LinearImpulse = ZeroVector;
			AngularImpulse = 0.0f;
		};
	};
};


void HybridConstraint::ApplyImpulse() {
	
	if(IsLimited == false ||  (IsLimited == true &&  Exceedes == false) )
	{
		Body1->p_cBody->ApplyImpulse( -Impulse, tmpLocalAnchor1);
		Body2->p_cBody->ApplyImpulse(  Impulse, tmpLocalAnchor2);
	};

	if(IsLimited == true &&  Exceedes == true)
	{
		Body1->p_cBody->ApplyImpulse( -LinearImpulse, tmpLocalAnchor1);
		Body2->p_cBody->ApplyImpulse(  LinearImpulse, tmpLocalAnchor2);

		Body1->p_cBody->ApplyAngularVelocity( -AngularImpulse*Body1->p_cBody->InverseInertia);
		Body2->p_cBody->ApplyAngularVelocity(  AngularImpulse*Body2->p_cBody->InverseInertia);
	};
};

void HybridConstraint::SolvePositionConstraint(const float &TimeStep) {
	
	if(IsLimited == false ||  (IsLimited == true &&  Exceedes == false) )
	{
		Correction22 = 0.5f*(tmpLocalAnchor2 -  tmpLocalAnchor1);
		Correction22/=TimeStep;
		auto RelativeVelocity = Body2->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor2) - Body1->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor1);
		Correction22-=RelativeVelocity;
		Impulse = -(M2*Correction22);
		
		Body1->p_cBody->ApplyImpulse( -Impulse, tmpLocalAnchor1);
		Body2->p_cBody->ApplyImpulse(  Impulse, tmpLocalAnchor2);
	};

	if(IsLimited == true &&  Exceedes == true) 
	{
		auto RelativeLinearVelocity = Body2->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor2) - Body1->p_cBody->GetLinearVelocityAtPoint(tmpLocalAnchor1);
		Vector3 RelativeVelocity;
		RelativeVelocity.x = RelativeLinearVelocity.x;
		RelativeVelocity.y = RelativeLinearVelocity.y;
		RelativeVelocity.z = Body2->p_cBody->AngularVelocity - Body1->p_cBody->AngularVelocity; 
		
	
		Correction33.x = 0.5f*(tmpLocalAnchor2.x -  tmpLocalAnchor1.x);//by³o 1
		Correction33.y = 0.5f*(tmpLocalAnchor2.y -  tmpLocalAnchor1.y);
		Correction33.z =  0.0f;
		
		if(CurrentAngle < LowerLimit) 
		{
			Correction33.z = CurrentAngle - LowerLimit;
		}
		else if (CurrentAngle > UpperLimit) 
		{
			Correction33.z = CurrentAngle - UpperLimit;
		};

		
		//if(abs(Correction33.z) > 0.01f) {
			Correction33/=TimeStep;
			Correction33-=RelativeVelocity;

			TotalImpulse = -(M3*Correction33);
			LinearImpulse.x = TotalImpulse.x;
			LinearImpulse.y = TotalImpulse.y;
		
		if(RelativeVelocity.length() > 0.01f) 
		{
			AngularImpulse = 0.5f*TotalImpulse.z; 
		} 
		else
		{
			AngularImpulse = 0.0f; 
		};

			Body1->p_cBody->ApplyImpulse( -LinearImpulse, tmpLocalAnchor1);
			Body2->p_cBody->ApplyImpulse(  LinearImpulse, tmpLocalAnchor2);
			Body1->p_cBody->ApplyAngularVelocity( -AngularImpulse*Body1->p_cBody->InverseInertia);
			Body2->p_cBody->ApplyAngularVelocity( AngularImpulse*Body2->p_cBody->InverseInertia);
	};
};
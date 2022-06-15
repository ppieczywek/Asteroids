#pragma once
#include "stdafx.h"
#include "SoftConstraint.h"

SoftConstraint::SoftConstraint(shared_ptr<Entity> &B1,const Vector2 &Anchor1, shared_ptr<Entity> &B2, const Vector2 &Anchor2) : Constraint(SConstraint, B1, B2)
{	
	RotationMatrix R1( -Body1->p_cBody->Angle);
	LocalAnchor1 = R1 * (Anchor1 - Body1->p_cBody->Position);
	
	RotationMatrix R2( -Body2->p_cBody->Angle);
	LocalAnchor2 = R2 * (Anchor2 - Body2->p_cBody->Position);
	
	Frequency = 0.0f;
	Omega = tau * Frequency;
	DampingRatio = 0.0f;
			
	RestLength = ( Anchor2 - Anchor1 ).length();
	TotalInvMass = B1->p_cBody->InverseMass + B2->p_cBody->InverseMass;
	
	Direction = Impulse = ZeroVector;
	ImpulseMagnitude = gamma = beta = c = k = h = Elongation = tmpImpulseMagnitude = EffectiveMass = RN1 = RN2 = 0.0f;
};

void SoftConstraint::SetFrequency(const float &NewFrequency)
{	
	if(NewFrequency < 0.0f)
		Frequency = 0.0f;
	else
		Frequency = NewFrequency;
	Omega = tau * Frequency;
};


void SoftConstraint::SetDampingRatio(const float &NewDampingRatio) 
{
	if(NewDampingRatio < 0.0f)
		DampingRatio = 0.0f;
	else
		DampingRatio = NewDampingRatio;	
};


void SoftConstraint::Initialize(const float &TimeStep) 
{	
	ImpulseMagnitude = 0.0f;

	tmpAnchor1 = TransformationMatrix(Body1->p_cBody->Angle, Body1->p_cBody->Position) * LocalAnchor1; 
	tmpAnchor2 = TransformationMatrix(Body2->p_cBody->Angle, Body2->p_cBody->Position) * LocalAnchor2;

	Direction = (tmpAnchor2 - tmpAnchor1);

	Elongation = Direction.length() - RestLength;
	Direction.normalize();

	R1 = tmpAnchor1 - Body1->p_cBody->Position;
	R2 = tmpAnchor2 - Body2->p_cBody->Position;
	RN1 = VectorProduct(R1,Direction);
	RN2 = VectorProduct(R2,Direction);
	
	EffectiveMass = TotalInvMass + (((RN1*RN1) * Body1->p_cBody->InverseInertia ) + ((RN2*RN2) * Body2->p_cBody->InverseInertia));
	EffectiveMass  != 0.0f ? 1.0f / EffectiveMass : 0.0f;

	if(Frequency > 0.0f) {
		k = EffectiveMass * Omega * Omega;
		c = 2.0f * EffectiveMass * DampingRatio * Omega;
		h = TimeStep;
		gamma = c + h * k;
		gamma = gamma != 0.0f ? 1.0f / gamma : 0.0f;
    
		beta = h*k * gamma;
		gamma*=EffectiveMass;
		gamma = gamma != 0.0f ? 1.0f / gamma : 0.0f;
	}
};


void SoftConstraint::SolveVelocityConstraint(const float &TimeStep) 
{

	RelativeVelocity = (Body2->p_cBody->GetLinearVelocityAtPoint(tmpAnchor2) - Body1->p_cBody->GetLinearVelocityAtPoint(tmpAnchor1));
	
	if(Frequency == 0.0f)
		ImpulseMagnitude = -RelativeVelocity* Direction; 
	else
		ImpulseMagnitude = ((RelativeVelocity* Direction - Elongation*beta*gamma) / (1 + h*gamma)) - RelativeVelocity* Direction; 
	
	Impulse = ImpulseMagnitude * EffectiveMass * Direction;

	/*glBegin(GL_LINES);
		
				glColor3f(1.0f, 0.0f, 0.0f);	
				glVertex3f(  tmpAnchor1.x ,  tmpAnchor1.y  , 0.5f );
				glVertex3f(  tmpAnchor2.x ,  tmpAnchor2.y  , 0.5f );
	glEnd();*/
};


void SoftConstraint::ApplyImpulse() 
{
	Body1->p_cBody->ApplyImpulse( -Impulse, tmpAnchor1);
	Body2->p_cBody->ApplyImpulse(  Impulse, tmpAnchor2);
};


void SoftConstraint::SolvePositionConstraint(const float &TimeStep) 
{
	if(Frequency == 0.0f) {
		//if(abs(Elongation) > 0.01f) {
			Impulse = -(1.0f*Elongation/TimeStep)*Direction*EffectiveMass;
		
			Body1->p_cBody->ApplyImpulse( -Impulse, tmpAnchor1);
			Body2->p_cBody->ApplyImpulse(  Impulse, tmpAnchor2);
		//};
		/*Correction = Elongation*Direction;
		if(Body1->p_cMove->IsDynamic() && Body2->p_cMove->IsDynamic()) {
			Body1->p_cConstrained->TotalTranslation+=0.5f*Correction;
			Body1->p_cConstrained->NumberOfTranslations++;
			Body2->p_cConstrained->TotalTranslation-=0.5f*Correction;
			Body2->p_cConstrained->NumberOfTranslations++;
		} else if(!Body1->p_cMove->IsDynamic()) {
			Body2->p_cConstrained->TotalTranslation-=Correction;
			Body2->p_cConstrained->NumberOfTranslations++;
		} else if(!Body2->p_cMove->IsDynamic()) {
			Body1->p_cConstrained->TotalTranslation+=Correction;
			Body1->p_cConstrained->NumberOfTranslations++;
		};	*/
	};
};

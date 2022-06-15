#pragma once
#include "stdafx.h"
#include "PhysicsManagerClass.h"
#include "GameEngineClass.h"

PhysicsManagerClass::PhysicsManagerClass() 
{
	SolverIterations = 10;
};

PhysicsManagerClass::~PhysicsManagerClass() 
{ 

};


void PhysicsManagerClass::IntegrateImpulses(vector<shared_ptr<cBody>>   &BodyList)
{		

	float TimeStep = WorldProperties.TimeStep / WorldProperties.SimulationSubSteps;
	float SquaredTimeStep = TimeStep * TimeStep;
	float InversedTimeStep = 1.0f / TimeStep;

	for each( auto Body in BodyList) 
	{ 		
		if(Body->AngularMotion) 
		{ 
			PrevAngle = Body->Angle;
			Body->Angle += Body->AngularVelocity * TimeStep +  0.5f*(Body->AngularAcceleration) * SquaredTimeStep;
			Body->AngularVelocity = (Body->Angle - PrevAngle) * InversedTimeStep;
			
			Body->AngularAcceleration = 0.0f;				
		}
			
		PrevPos = Body->Position;
		Body->Position += Body->LinearVelocity* TimeStep +  0.5f*(Body->LinearAcceleration) * SquaredTimeStep;
		
		Body->LinearVelocity = (Body->Position - PrevPos) * InversedTimeStep;
		Body->LinearAcceleration = ZeroVector;
							
		Body->SetTransform(Body->Position,Body->Angle);		
	}
};


void PhysicsManagerClass::ApplyForces(vector<shared_ptr<cBody>>   &BodyList) 
{	
	if(WorldProperties.Gravity) 
	{
		for each( auto Body in BodyList)
		{
			if(Body->IsDynamic()) Body->LinearAcceleration+=WorldProperties.GravityValue;;
		}
	}
	
	if(WorldProperties.Drag)
	{
		for each( auto Body in BodyList) 
		{
			if(Body->IsDynamic())
			{
				Body->LinearAcceleration -=Body->LinearVelocity * Body->LinearDamping; 
				Body->AngularAcceleration-=Body->AngularDamping * Body->AngularVelocity;
			}
		}
	}
};


void PhysicsManagerClass::Update(GameObjectManagerClass& ObjectManager) {

	
	for(auto ii = 0; ii<WorldProperties.SimulationSubSteps; ++ii)
	{
		ApplyForces(ObjectManager.GetcBodyList());		
		IntegrateImpulses(ObjectManager.GetcBodyList());				
	}
	
	CollisionManager.BroadPhaseDetection(ObjectManager.GetcBodyList());
	CollisionManager.NarrowPhaseDetection(ObjectManager.GetcBodyList());

	for(auto ii=0; ii<SolverIterations;++ii) 
	{
		ConstraintManager.SolveConstraints(ObjectManager.ConstraintList, WorldProperties.TimeStep);
		CollisionManager.SolveCollisions(ObjectManager, WorldProperties);
	}
};




#pragma once
#include "stdafx.h"
#include "ConstraintManagerClass.h"
#include "GameEngineClass.h"



void ConstraintManagerClass::SolveConstraints(vector<shared_ptr<Constraint>> &ConstraintList, float TimeStep) 
{
	if(!ConstraintList.empty()) 
	{	
		for (unsigned ii = 0; ii<ConstraintList.size(); ++ii) 
		{
			ConstraintList[ii]->Initialize(TimeStep);
		}
		
		for (unsigned ii = 0; ii<ConstraintList.size(); ++ii) 
		{
			ConstraintList[ii]->SolveVelocityConstraint(TimeStep);
		}
		
		for (unsigned ii = 0; ii<ConstraintList.size(); ++ii) 
		{
			ConstraintList[ii]->ApplyImpulse();
		}
		
		for (unsigned ii = 0; ii<ConstraintList.size(); ++ii) 
		{
			ConstraintList[ii]->SolvePositionConstraint(TimeStep);
		}	
	}
};
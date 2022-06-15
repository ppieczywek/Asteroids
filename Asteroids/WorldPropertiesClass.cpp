#pragma once
#include "stdafx.h"
#include "WorldPropertiesClass.h"


WorldPropertiesClass::WorldPropertiesClass() : Gravity(false), Drag(false), Friction(false), GravityValue(0.0f,0.0f) {
	FrameRate = DefaultFrameRate;
	SimulationSubSteps = DefaultSimulationSubSteps;
	SquaredTimeStep = TimeStep = (1/FrameRate);
	HalfTimeStep = 0.5f*TimeStep;
	SquaredTimeStep*=SquaredTimeStep;
	InversedTimeStep = TimeStep > 0.0f ? 1.0f/TimeStep : 0.0f;
};

void	WorldPropertiesClass::SetPhysicsTimeStep(const float &Step)	{
	SquaredTimeStep = TimeStep = Step > 0.0f ? Step : DefaultTimeStep;
	HalfTimeStep = 0.5f*TimeStep;
	SquaredTimeStep*=SquaredTimeStep;

};

void	WorldPropertiesClass::SetFrameRate(const float &Frames)	{
	FrameRate = Frames > 0.0f ? Frames : DefaultFrameRate;
};
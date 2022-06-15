#pragma once
#include "Vector2.h"
#include "Rectangle2D.h"
const float DefaultTimeStep = 1.1f;
const float DefaultSquareTimeStep = 1.01f;
const float DefaultFrameRate = 60.0f;
const float DefaultSimulationSubSteps = 3.0f;

const float MaxImpulse = 1000.0f;
const float MinImpulse = 0.0f;//0.005f;
const float VelocityThreshold = 0.05f; //1.1
const float PlasticCollisionThreshold = 0.2f;// -0.5f;

class WorldPropertiesClass
{

	bool				Gravity;
	bool				Drag;
	bool				Friction;
	float				TimeStep;
	float				InversedTimeStep;
	float				HalfTimeStep;
	float				SquaredTimeStep;
	float				FrameRate;
	float				SimulationSubSteps;
	Vector2				GravityValue;
	Rectangle2D			WorldBox;
		
public:
	friend class		GameEngineClass;
	friend class		CollisionManagerClass;
	friend class		PhysicsManagerClass;
	
						WorldPropertiesClass(); 
	inline void			SetGravityState(const bool&);
	inline void			SetFrictionState(const bool&);
	inline void			SetDragState(const bool&);
	inline void			SetGravityValue(const Vector2&);
	inline void			SetWorldBox(const Rectangle2D&);
	void				SetPhysicsTimeStep(const float&);
	void				SetFrameRate(const float&);

	inline bool			GetGravityState();
	inline bool			GetFriciotnState();
	inline bool			GetDragState();
	inline Vector2		GetGravityValue();
	inline float		GetPhysicsTimeStep();
	inline float		GetPhysicsSquaredTimeStep();
	inline float		GetFrameRate();
	inline Rectangle2D	GetWorldBox();
	inline float		GetSimulationSubSteps();
};


void WorldPropertiesClass::SetGravityState(const bool &GravityFlag)			
{
	Gravity = GravityFlag;
};


void WorldPropertiesClass::SetFrictionState(const bool &FrictionFlag)
{
	Friction = FrictionFlag;
};


void WorldPropertiesClass::SetDragState(const bool &DragFlag)
{
	Drag = DragFlag;
};


void WorldPropertiesClass::SetGravityValue(const Vector2 &Gravity)
{
	GravityValue = Gravity;
};


void WorldPropertiesClass::SetWorldBox(const Rectangle2D &NewWorldBox)
{
	WorldBox = NewWorldBox;
};


bool WorldPropertiesClass::GetGravityState()
{
	return Gravity;
};


bool WorldPropertiesClass::GetFriciotnState()							
{
	return Friction;
};


bool WorldPropertiesClass::GetDragState()								
{
	return Drag;
};


Vector2	WorldPropertiesClass::GetGravityValue()							
{
	return GravityValue;
};


float WorldPropertiesClass::GetPhysicsTimeStep()						
{
	return TimeStep;
};


float WorldPropertiesClass::GetPhysicsSquaredTimeStep()					
{
	return SquaredTimeStep;
};


float WorldPropertiesClass::GetFrameRate()								
{
	return FrameRate;
};


Rectangle2D	WorldPropertiesClass::GetWorldBox()								
{
	return WorldBox;
};

float WorldPropertiesClass::GetSimulationSubSteps()								
{
	return SimulationSubSteps;
};
#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "WorldPropertiesClass.h"
#include "QTree2.h"
#include "TCGrid.h"
#include <set>
#include <vector>


class ContactSolution 
{
public:
	int					Body1, Body2;
	int					Face1,Face2;
	int					OwnerIndex,NormalIndex;
	int					Impulse;
	int					Overlap;
	int 				CollisionPointNumber;
	CollisionType		cType;
	float				CumulativeMass;
	float				Restitution;
	float				Penetration;
	float				NormalImpulseMagnitude;
	float				TangentImpulseMagnitude;
	float				CorrectionCoeff1;
	float				CorrectionCoeff2;
	Vector2				MinimumTranlationVector;
	Vector2				CollisionNormal;
	Vector2				CollisionTangent;
	Vector2				CollisionPoint;
	Vector2				TangentImpulse;
	Vector2				NormalImpulse;

	friend class		CollisionManagerClass;
	friend class		LogicsManagerClass;

						ContactSolution();
	void				Clear();		
};


class CollisionManagerClass 
{		
	vector<pair<int,int>>					ContactList;
	vector<ContactSolution>					PhysicalContactList;
	vector<ContactSolution>					LogicalContactList;
	//dynamic_array<bool>						CollisionFlags;
	
	int										BodyOverlap;
	int										SensorOverlap;
	int										tempFace1;
	int										tempFace2;
	int										OIndex;
	int										NIndex;
	unsigned								MaxNumberOfSolutions; 
	unsigned								CurrentNumberOfSolutions;
	unsigned								PhysicalCollisionNumber;
	unsigned								LogicalCollisionNumber;
	unsigned								tempOIndex;
	unsigned								tempNIndex;
	Vector2									MinimumTranlationVector;
	Vector2									RelativeVelocity;
	Vector2									R1;
	Vector2									R2;
	Vector2									Bias;
	Vector2									TempMinimumTranslationVector;
	float									VelocityMagnitude;
	float									RN_1;
	float									RN_2;
	float									Denominator, NormalPart, MinimumTranslationVectorLength, LongestMinimumTranslationVector;
	float									BiasCoeff;
	float									PenetrationTolerance;



	void									AdjustContainersSize();
	void									PreCalculateIntervals(vector<shared_ptr<cBody>>& BodyList);
	void									UpdateCollisionPoints3(vector<shared_ptr<cBody>>& BodyList);
	void									UpdateCollisionPoints2(vector<shared_ptr<cBody>>& BodyList);
	void									UpdateCollisionPoints(vector<shared_ptr<cBody>>& BodyList);
	void									UpdatePhysicalContact(vector<shared_ptr<cBody>>&);
	void									UpdateCollisionImpulse(vector<shared_ptr<cBody>>&,const Vector2&, const float&);
	void									CalculateInterval(const Vector2&, const Polygon2&, float&, float&);
	bool									IsAxisSeparating(const Polygon2&, const Polygon2&, const int&, const int&, Vector2&);
	bool									AreTrianglesSeparated(const Polygon2&, const Polygon2&, Vector2&, int&, int&); 
	bool									IsPointInside(const Vector2&, const vector<Polygon2>&, const unsigned&);
	bool									IsPointInside2(const Vector2&, const Polygon2&);
		
public:
	TCGrid									Grid;
	QTree2									QuadTree;
											CollisionManagerClass();
											~CollisionManagerClass();

	void									SolveCollisions(GameObjectManagerClass&, WorldPropertiesClass&);
	//void									Initialize(GameEngineClass&);
	void									BroadPhaseDetection(vector< shared_ptr<cBody>>& BodyList);
	void									NarrowPhaseDetection(vector< shared_ptr<cBody> >  &BodyList);

	inline void								SetPenetrationTolerance(const float&);
	inline void								SetBiasCoefficient(const float&);
	inline float							GetPenetrationTolerance();
	inline float							GetBiasCoefficient();
	inline vector<ContactSolution>&			GetPhysicalContactList();
	inline vector<ContactSolution>&			GetLogicalContactList();

	inline unsigned							GetPhysicalCollisionNumber();
	inline unsigned							GetLogicalCollisionNumber();

	inline vector<pair<int,int>>&			GetContactList();
	
	
};
	

void CollisionManagerClass::SetPenetrationTolerance(const float& NewPenetrationTolerance)	
{
	PenetrationTolerance = NewPenetrationTolerance;
};


void CollisionManagerClass::SetBiasCoefficient(const float& NewBias)
{
	BiasCoeff = NewBias;
};
		

float CollisionManagerClass::GetPenetrationTolerance()		
{
	return PenetrationTolerance;
};


float CollisionManagerClass::GetBiasCoefficient()			
{
	return BiasCoeff;
};


vector<ContactSolution>& CollisionManagerClass::GetPhysicalContactList()	 
{
	return PhysicalContactList;
};


vector<ContactSolution>& CollisionManagerClass::GetLogicalContactList()	 
{
	return LogicalContactList;
};


unsigned CollisionManagerClass::GetPhysicalCollisionNumber()
{
	return 	PhysicalCollisionNumber;
};


unsigned CollisionManagerClass::GetLogicalCollisionNumber()
{
	return 	LogicalCollisionNumber;	
};


vector<pair<int,int>>&	 CollisionManagerClass::GetContactList()
{
	return ContactList;		
};
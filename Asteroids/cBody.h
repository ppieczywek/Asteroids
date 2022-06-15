#pragma once
#include "Vector2.h"
#include "Rectangle2D.h"
#include "RotationMatrix.h"
#include "TransformationMatrix.h"
#include "Entity.h"

#include <set>
#include <vector>
#include "Polygon2.h"
#include <algorithm>
using namespace std;


enum BodyType {static2D, kinematic2D, dynamic2D};
enum CollisionType {Logical, Physical, Both, None};

class cBody 
{
protected:
		
	long long				ID;
	BodyType				bType;
	CollisionType			cType;
	float					Density;
	float					Mass;
	float					InverseMass;
	float					Inertia;
	float					InverseInertia;
	float					Angle;
	float					LinearDamping;
	float					AngularDamping;
	float					AngularVelocity;
	float					AngularAcceleration;
	float					RestitutionCoefficient;
	float					FrictionCoefficient;
	float					NumberOfTranslations;
	Vector2					Position;
	Vector2					LinearVelocity;
	Vector2					LinearAcceleration;
	Vector2					LocalOrigin;
	Vector2					WorldOrigin;
	Vector2					OriginOffset;
	Vector2					TotalTranslationVector;
	Vector2					CumulativeTranslationVector;

	unsigned				SensorsNumber;
	unsigned				PolygonsNumber;
	vector<Polygon2>		PolygonsList;
	set<long long>			NonCollidable;

	bool					AngularMotion;

	TransformationMatrix	T;
	Vector2					VectorBuffer;
	void					UpdateNormals();
	void					UpdateBodyBBox();
	void					UpdateFaceBBox();

public:

	Rectangle2D				BBox;

	friend class			CollisionManagerClass;
	friend class			ConstraintManagerClass;
	friend class			GameObjectManagerClass;
	friend class			PhysicsManagerClass;
	friend class			SoftConstraint;
	friend class			HingeConstraint;
	friend class			WeldConstraint;
	friend class			HybridConstraint;
	friend class			CameraClass;
	friend class			cRender;
	friend class			TCGrid;
					
		
	void					ScaleModel(const float &Xscale, const float &Yscale);//zmienia siê po³o¿enie uk³adu wsp

	//void		SetLocalOrigin(const Vector2 &V);
	//void		SetWorldOrigin(const Vector2 &V);

							cBody();
	virtual					~cBody();

	inline void				ApplyForce(const Vector2&, const Vector2&);
	inline void				ApplyForceToCenter(const Vector2&);
	inline void				ApplyTorque(const float&);
	inline void				ApplyImpulse(const Vector2&, const Vector2&);
	inline void				ApplyImpulseToCenter(const Vector2&);
	inline void				ApplyLinearVelocity(const Vector2&);
	inline void				ApplyAngularVelocity(const float&);
	inline void				ApplyLinearAcceleration(const Vector2&);
	inline void				ApplyAngularAcceleration(const float&);

	inline void				SetID(const long long&);
	void					SetTransform(const float&);
	void					SetTransform(const Vector2&);
	void					SetTransform(const Vector2&, const float&);
	inline void				SetLocalOrigin(const Vector2&); 
	inline void				SetWordlOrigin(const Vector2&); 
	inline void				Rotate(const Vector2&, const float&);
	void					SetDensity(const float&);
	inline void				SetBodyType(const BodyType&);//?
	inline void				SetLinearVelocity(const Vector2&);
	inline void				SetAngularVelocity(const float&);
	inline void				SetLinearAcceleration(const Vector2&);
	inline void				SetAngularAcceleration(const float&);
	inline void				SetLinearDamping(const float&);
	inline void				SetAngularDamping(const float&);
	inline void				SetAngularMotion(const bool&);

	inline long long		GetID();
	inline Vector2			GetPosition();
	inline float			GetAngle();
	inline Vector2			GetLocalOrigin();
	inline Vector2			GetWorldOrigin();
	inline float			GetDensity();
	inline float			GetMass();
	inline float			GetInverseMass();
	inline float			GetInertia();
	inline float			GetInverseInertia();
	inline Vector2		    GetLinearVelocity();
	inline Vector2			GetLinearVelocityAtPoint(const Vector2&);
	inline float			GetAngularVelocity();
	inline Vector2			GetLinearAcceleration();
	inline float			GetAngularAcceleration();
	inline float			GetLinearDamping();
	inline float			GetAngularDamping();
	inline bool				GetAngularMotion();

	void						AddPolygon(const Polygon2& NewPolygon);
	inline unsigned				GetPolygonsNumber();
	inline vector<Polygon2>&	GetPolygonsList();

	inline bool				IsCollidable();
	void					SetRestitutionCoefficient(const float&);//			{RestitutionCoefficient = val;};
	inline float			GetRestitutionCoefficient();//							{return RestitutionCoefficient;};
	
	inline void				SetFrictionCoefficient(const float&);//				{FrictionCoefficient = val;};
	inline float			GetFrictionCoefficient();//							{return FrictionCoefficient;};

	inline void				SetCollisionType(const CollisionType&);
	inline CollisionType	GetCollisionType();
		
	inline void				AccumulateTranslation(const Vector2&);
	inline Vector2			GetTotalTranslationVector();
	
	void					AddNonCollidable(const long long&);
	void					RemoveNonCollidable(const long long&);
	bool					IsExcluded(const long long&);


	inline bool				IsStatic();
	inline bool				IsKinematic();
	inline bool				IsDynamic();
	

	
};

void cBody::SetBodyType(const BodyType &NewBodyType) 
{
	bType = NewBodyType;
	/*if(Type == static2D) { 
		AngularMotion = false;
	} else if(Type == kinematic2D) {
		AngularMotion = true;
	} else if(Type == dynamic2D) {
		AngularMotion = true;
	}*/
};


void cBody::SetID(const long long& NewID) 
{
	ID = NewID;
};


long long	cBody::GetID() 
{
	return ID;
}


Vector2	cBody::GetPosition()			
{
	return Position;
};


float cBody::GetAngle()				
{
	return Angle;
};


float cBody::GetMass()
{
	return Mass;
};


float cBody::GetDensity()
{
	return Density;
};


float cBody::GetInverseMass()
{
	return InverseMass;
};


float cBody::GetInertia()			
{
	return Inertia;
};


float cBody::GetInverseInertia()		
{
	return InverseInertia;
};


void cBody::ApplyImpulse(const Vector2 &Impulse, const Vector2 &WorldPoint) 
{
	if(bType==dynamic2D)
	{
		LinearVelocity += Impulse * InverseMass;
		if(AngularMotion) AngularVelocity += VectorProduct((WorldPoint - Position), Impulse) *  InverseInertia;
	}
};


void cBody::ApplyForce(const Vector2 &Force, const Vector2 &WorldPoint)
{
	if(bType==dynamic2D)
	{
		LinearAcceleration += Force * InverseMass;
		if(AngularMotion) AngularAcceleration += VectorProduct( ( WorldPoint - Position ), Force) *  InverseInertia;
	};
};

void cBody::ApplyLinearVelocity(const Vector2 &LinearVelocityIncrement)			
{ 
	if(bType!=static2D) LinearVelocity += LinearVelocityIncrement; 
};


void cBody::ApplyAngularVelocity(const float &AngularVelocityIncrement)			
{ 
	if(AngularMotion && bType!=static2D) AngularVelocity += AngularVelocityIncrement; 
};


void cBody::ApplyLinearAcceleration(const Vector2 &LinearAccelerationIncrement)		
{ 
	if(bType==dynamic2D) LinearAcceleration += LinearAccelerationIncrement; 
};


void cBody::ApplyAngularAcceleration(const float &AngularAccelerationIncrement)		
{ 
	if(AngularMotion && bType==dynamic2D) AngularAcceleration += AngularAccelerationIncrement; 
};


void cBody::ApplyForceToCenter(const Vector2 &Force)
{
	if(bType==dynamic2D) LinearAcceleration += Force * InverseMass;
};


void cBody::ApplyImpulseToCenter(const Vector2 &Impulse) 
{
	if(bType==dynamic2D) LinearVelocity += Impulse * InverseMass;
};


void cBody::ApplyTorque(const float &Torque) 
{
	if(bType==dynamic2D) AngularAcceleration += Torque *  InverseInertia;
};


Vector2 cBody::GetLinearVelocityAtPoint(const Vector2 &WorldPoint)
{
	return  LinearVelocity + Vector2(-(AngularVelocity*(WorldPoint.y - Position.y)), AngularVelocity*(WorldPoint.x - Position.x));
};


void cBody::SetLinearVelocity(const Vector2 &NewLinearVelocity)			
{ 
	if(bType!=static2D) LinearVelocity = NewLinearVelocity; 
};


void cBody::SetAngularVelocity(const float &NewAngularVelocity)
{
	if(AngularMotion && bType!=static2D) AngularVelocity = NewAngularVelocity; 
};


void cBody::SetLinearAcceleration(const Vector2 &NewLinearAcceleration)
{ 
	if(bType==dynamic2D) LinearAcceleration = NewLinearAcceleration; 
};


void cBody::SetAngularAcceleration(const float &NewAngularAcceleration)
{ 
	if(AngularMotion && bType==dynamic2D) AngularAcceleration = NewAngularAcceleration; 
};


void cBody::SetLinearDamping(const float &NewLinearDamping)					
{
	LinearDamping = (NewLinearDamping >= 0.0f) ? NewLinearDamping : 0.0f;;
};


void cBody::SetAngularDamping(const float &NewAngularDamping)				
{
	AngularDamping = (NewAngularDamping >= 0.0f) ? NewAngularDamping : 0.0f;
};


void cBody::SetAngularMotion(const bool &NewAngularMotionFlag)
{
	if(!(AngularMotion = NewAngularMotionFlag)) AngularVelocity = AngularAcceleration = 0.0f;
};


Vector2 cBody::GetLinearVelocity()
{
	return LinearVelocity;
};


float cBody::GetAngularVelocity()							
{
	return AngularVelocity;
};


Vector2 cBody::GetLinearAcceleration()
{
	return LinearAcceleration;
};


float cBody::GetAngularAcceleration()
{
	return AngularAcceleration;
};


float cBody::GetLinearDamping()
{
	return LinearDamping;
};


float cBody::GetAngularDamping()
{
	return AngularDamping;
};


bool cBody::GetAngularMotion()								
{
	return AngularMotion;
};

bool cBody::IsStatic()		
{
	return (bType==static2D);
};


Vector2	cBody::GetLocalOrigin() 
{
	return LocalOrigin;		
};


Vector2	cBody::GetWorldOrigin()
{
	return WorldOrigin;
};


void cBody::SetLocalOrigin(const Vector2& NewLocalOrigin) 
{
	LocalOrigin = NewLocalOrigin;
};


void cBody::SetWordlOrigin(const Vector2& NewWorldOrigin) 
{
	SetTransform(NewWorldOrigin + LocalOrigin);
	//LocalOrigin = NewLocalOrigin;//return LocalOrigin;		
};


void cBody::Rotate(const Vector2& LocalRotationCenter, const float& Angle) //do poprawki
{
	//auto WorldRotationCenter = LocalRotationCenter + Position;
	//auto Offset = RotationMatrix(Angle)*(Position - WorldRotationCenter);
	auto Offset = RotationMatrix(Angle)*(-LocalRotationCenter);
	SetTransform(Offset + LocalRotationCenter + Position, Angle);
};


bool cBody::IsKinematic()
{
	return (bType==kinematic2D);
};


bool cBody::IsDynamic()
{
	return (bType==dynamic2D);
};


unsigned cBody::GetPolygonsNumber()
{
	return PolygonsNumber;
};

vector<Polygon2>& cBody::GetPolygonsList()
{
	return PolygonsList;
};


bool  cBody::IsCollidable()
{
	if( cType == None )
		return false;
	else
		return true;
	//return cType != None ;
};


float  cBody::GetRestitutionCoefficient()
{
	return RestitutionCoefficient;
};

	
void cBody::SetFrictionCoefficient(const float &NewFrictionCoefficient)
{
	FrictionCoefficient = NewFrictionCoefficient;
};


float cBody::GetFrictionCoefficient()
{
	return FrictionCoefficient;
};


void cBody::SetCollisionType(const CollisionType &NewCollisionType)	
{
	cType = NewCollisionType;
};


CollisionType cBody::GetCollisionType()
{
	return cType;
};


void cBody::AccumulateTranslation(const Vector2 &Vector) 
{
	CumulativeTranslationVector+=Vector;
	NumberOfTranslations++;
};


Vector2 cBody::GetTotalTranslationVector()					 
{
	if(NumberOfTranslations > 0.0f) 
	{
		TotalTranslationVector = CumulativeTranslationVector/NumberOfTranslations;
		CumulativeTranslationVector = ZeroVector;
		NumberOfTranslations = 0.0f;
		return TotalTranslationVector;
	} else {
		return ZeroVector;
	};
};
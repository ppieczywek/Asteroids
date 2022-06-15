#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include <set>

using namespace std;

//dodaæ none ?

class cCollide {
protected:
	int					ID;
	float				RestitutionCoefficient;
	float				FrictionCoefficient;
	float				NumberOfTranslations;
	Vector2				TotalTranslationVector;
	Vector2				CumulativeTranslationVector;
		CollisionType		cType;
	set<int>			NonCollidable;
	
	set<int>			SensorList;
	vector<int>			CollisionSolutionList;

	bool				IsColliding;
	bool				Collidable;

	inline void			SetCollidable(const bool&);

public:
	friend class CollisionManagerClass;
	friend class ConstraintManagerClass;
	friend class PhysicsManagerClass;
	friend class GameObjectManagerClass;
	friend class SoftConstraint;
	friend class HingeConstraint;
	friend class WeldConstraint;
	friend const shared_ptr<Entity> LoadModel(const char* FileName);
	//friend bool  AreExcluded(const shared_ptr<Entity const> &P1, const shared_ptr<Entity const> &P2);

							cCollide(const bool &CollidableValue);
							~cCollide();
	
		
	inline void				SetID(const int&); 
	inline int				GetID(); 

	inline bool				IsCollidable();
	void					SetRestitutionCoefficient(const float&);//			{RestitutionCoefficient = val;};
	inline float			GetRestitutionCoefficient();//							{return RestitutionCoefficient;};
	
	inline void				SetFrictionCoefficient(const float&);//				{FrictionCoefficient = val;};
	inline float			GetFrictionCoefficient();//							{return FrictionCoefficient;};

	inline void				SetCollisionType(const CollisionType&);
	inline CollisionType	GetCollisionType();
		
	inline void				AccumulateTranslation(const Vector2&);
	inline Vector2			GetTotalTranslationVector();
	
	void					AddNonCollidable(const int&);
	void					RemoveNonCollidable(const int&);
	bool					IsExcluded(const int&);

	inline void				ClearCollisionSolutionList();
	inline void				AddCollisionSolution(const int&);

};


void cCollide::ClearCollisionSolutionList()
{
	CollisionSolutionList.clear();
	CollisionSolutionList.reserve(20);
};


void cCollide::AddCollisionSolution(const int& SolitionId)
{
	CollisionSolutionList.push_back(SolitionId);
};


void cCollide::SetID(const int& NewID) 
{
	ID = NewID;
};


int	cCollide::GetID() 
{
	return ID;
};


void cCollide::SetCollidable(const bool &NewCollidable)
{
	Collidable = NewCollidable;
};


bool cCollide::IsCollidable()
{
	return Collidable;
};


float cCollide::GetRestitutionCoefficient()
{
	return RestitutionCoefficient;
};

	
void cCollide::SetFrictionCoefficient(const float &NewFrictionCoefficient)
{
	FrictionCoefficient = NewFrictionCoefficient;
};


float cCollide::GetFrictionCoefficient()
{
	return FrictionCoefficient;
};


void cCollide::SetCollisionType(const CollisionType &NewCollisionType)	
{
	cType = NewCollisionType;
};


CollisionType cCollide::GetCollisionType()
{
	return cType;
};


void cCollide::AccumulateTranslation(const Vector2 &Vector) 
{
	CumulativeTranslationVector+=Vector;
	NumberOfTranslations++;
};


Vector2 cCollide::GetTotalTranslationVector()					 
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
#pragma once
#include "stdafx.h"
#include "cCollide.h"


cCollide::cCollide(const bool &CollidableValue) : cType(Both), RestitutionCoefficient(1.2f), FrictionCoefficient(0.6f), TotalTranslationVector(0.0f,0.0f), CumulativeTranslationVector(0.0f,0.0f), IsColliding(false), Collidable(CollidableValue), NumberOfTranslations(0.0f) { };


cCollide::~cCollide() 
{
	NonCollidable.clear();
};


void cCollide::SetRestitutionCoefficient(const float &NewRestitutionCoefficient)
{
	if (NewRestitutionCoefficient < 1.0f) 
	{
		RestitutionCoefficient = 1.0f;
	}
	else if (NewRestitutionCoefficient > 2.0f) 
	{
		RestitutionCoefficient = 2.0f;
	}
	else 
	{
		RestitutionCoefficient = NewRestitutionCoefficient;
	}
};


void cCollide::AddNonCollidable(const int &Id)
{
	if(Id > -1 && !NonCollidable.empty())  
	{
		NonCollidable.insert(Id);
	}
};


void cCollide::RemoveNonCollidable(const int &Id)
{
	if(Id > -1 && !NonCollidable.empty()) 
	{
		NonCollidable.erase(Id);
	}
};


bool cCollide::IsExcluded(const int &Id)
{	
	if(!NonCollidable.empty()) 
	{
		if( bool is_in = NonCollidable.find(Id) != NonCollidable.end() ) return true;
	}
	return false;
};



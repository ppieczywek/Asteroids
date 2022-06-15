#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "cConstrained.h"
#include "cLogics.h"

void Entity::SetID(const long long& NewID) 
{
	ID = NewID;
	if(p_cBody != nullptr) p_cBody->SetID(NewID);
	if(p_cRender != nullptr) p_cRender->SetID(NewID);
	if(p_cConstrained != nullptr) p_cConstrained->SetID(NewID);
	if(p_cLogics != nullptr) p_cLogics->SetID(NewID);
};

Entity::~Entity()
{
	p_cRender.reset();
	p_cBody.reset();
	p_cConstrained.reset();
	p_cLogics.reset();
};
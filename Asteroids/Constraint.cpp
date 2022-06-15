#include "stdafx.h"
#include "Constraint.h"

Constraint::Constraint(const ConstraintType &type, shared_ptr<Entity> &B1, shared_ptr<Entity> &B2) : ID(-1), cType(type), Body1(B1), Body2(B2) 
{
	//empty
};


Constraint::~Constraint() 
{
	Body1.reset(); Body2.reset();
};


ConstraintType  Constraint::GetConstraintType()
{
	return cType;
};
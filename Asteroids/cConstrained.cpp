#pragma once
#include "stdafx.h"
#include "cConstrained.h"


cConstrained::cConstrained() : IsConstrained(false) 
{

}; 


cConstrained::~cConstrained() 
{
	ConstraintsList.clear();
}; 
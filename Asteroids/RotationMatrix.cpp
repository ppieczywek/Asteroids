#include "stdafx.h"
#include "RotationMatrix.h"

RotationMatrix::RotationMatrix() : a(1.0f), b(0.0f), c(0.0f), d(1.0f) { };
RotationMatrix::RotationMatrix(const float &AA, const float &BB, const float &CC, const float &DD) : a(AA), b(BB), c(CC), d(DD) { };

RotationMatrix::RotationMatrix(const float &angle) 
{
	d = a =  cos(angle);
	c =  sin(angle);
	b = -c;
};

RotationMatrix& RotationMatrix::operator() (const float &angle)
{
	d = a =  cos(angle);
	c =  sin(angle);
	b = -c;
	return *this;
};


#include "stdafx.h"
#include "TransformationMatrix.h"

TransformationMatrix::TransformationMatrix() : a(1.0f), b(0.f), c(0.0f), d(0.0f), e(1.0f), f(0.0f) { };

TransformationMatrix::TransformationMatrix(const float &angle, const Vector2 &T) : c(T.x) , f(T.y) {
	e = a =  cos(angle);
	d =  sin(angle);
	b = -d;
};

TransformationMatrix& TransformationMatrix::operator() (const float &angle, const Vector2 &T) {
	e = a =  cos(angle);
	d =  sin(angle);
	b = -d;
	c = T.x;
	f = T.y;
	return *this;
};
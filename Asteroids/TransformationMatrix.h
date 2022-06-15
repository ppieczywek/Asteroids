#pragma once
#include "stdafx.h"
#include "Vector2.h"

class TransformationMatrix {
	float a,b,c,d,e,f;
public:
	TransformationMatrix();
	TransformationMatrix(const float &angle, const Vector2 &T);
	const Vector2 operator*(const Vector2 &V) const {	return Vector2( a * V.x + b * V.y + c  , d * V.x + e * V.y + f ); };
	TransformationMatrix& operator() (const float &angle, const Vector2 &T);

	const Vector2 operator() (const Vector2 &V) const { return (*this)*V;/*Vector2D( float(a * V.x + b * V.y + c)  , float(d * V.x + e * V.y + f) );*/ };
};
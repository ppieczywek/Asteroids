#pragma once
#include "stdafx.h"
#include "Vector2.h"

class RotationMatrix {
	float a,b,c,d;
public:
	RotationMatrix();
	RotationMatrix(const float &angle);
	RotationMatrix(const float &AA, const float &BB, const float &CC, const float &DD);
	const Vector2 operator*(const Vector2 &V) const { return Vector2( a * V.x + b * V.y  , c * V.x + d * V.y ); };
	RotationMatrix& operator() (const float &angle);
	const Vector2 operator() (const Vector2 &V) const { return (*this)*V;};
};

const RotationMatrix CCW(0.0f,-1.0f,1.0f,0.0f);
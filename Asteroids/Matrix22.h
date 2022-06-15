#pragma once
#include "stdafx.h"
#include "Vector2.h"

class Matrix22 {
public:
	float a11, a12, a21, a22;
	Matrix22() : a11(0.0f), a12(0.0f), a21(0.0f), a22(0.0f) {};
	Matrix22(const float &v11, const float &v12, const float &v21, const float &v22) : a11(v11), a12(v12), a21(v21), a22(v22) {};
	Matrix22(const Matrix22& M) : a11(M.a11), a12(M.a12), a21(M.a21), a22(M.a22) {};

	float Det() { return a11*a22 - a12*a21;};
	const Matrix22& Transpose();
	bool Inverse();

	const Vector2 operator*(const Vector2 &v) { return Vector2(a11*v.x + a12*v.y, a21*v.x + a22*v.y); }; 
	const Matrix22 operator-() const { return Matrix22(-a11,-a12,-a21,-a22); };
	
};
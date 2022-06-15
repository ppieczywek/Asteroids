#pragma once
#include "stdafx.h"
#include "Vector2.h"
#include "Vector3.h"

class Matrix33 {
public:
	float a11, a12, a13, a21, a22, a23, a31, a32, a33;
	Matrix33() : a11(0.0f), a12(0.0f), a13(0.0f), a21(0.0f), a22(0.0f), a23(0.0f), a31(0.0f), a32(0.0f), a33(0.0f) {};
	Matrix33(const float &v11,const float &v12,const float &v13,const float &v21,const float &v22,const float &v23,const float &v31,const float &v32,const float &v33) : a11(v11), a12(v12), a13(v13), a21(v21), a22(v22), a23(v23), a31(v31), a32(v32), a33(v33) {};
	Matrix33(const Matrix33 &M) : a11(M.a11), a12(M.a12), a13(M.a13), a21(M.a21), a22(M.a22), a23(M.a23), a31(M.a31), a32(M.a32), a33(M.a33) {};
	
	Matrix33& operator=(const Matrix33 &M);
	const Matrix33 operator-() const { return Matrix33(-a11,-a12,-a13,-a21,-a22, -a23, -a31, -a32, -a33); };
	
	const Vector3 operator*(const Vector3 &v) { return Vector3(a11*v.x + a12*v.y + a13*v.z, a21*v.x + a22*v.y + a23*v.z, a31*v.x + a32*v.y + a33 * v.z); }; 

	const Matrix33 operator*(const Matrix33 &M) const;  
	bool Inverse();
	const Matrix33& Transpose();
	float Det();
};
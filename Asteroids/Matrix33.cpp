#pragma once
#include "stdafx.h"
#include "Matrix33.h"


const Matrix33 Matrix33::operator*(const Matrix33 &M) const { 
	
	Matrix33 Result;
	Result.a11 = a11*M.a11 + a12*M.a21 + a13*M.a31;
	Result.a12 = a11*M.a12 + a12*M.a22 + a13*M.a32;
	Result.a13 = a11*M.a13 + a12*M.a23 + a13*M.a33;

	Result.a21 = a21*M.a11 + a22*M.a21 + a23*M.a31;
	Result.a22 = a21*M.a12 + a22*M.a22 + a23*M.a32;
	Result.a23 = a21*M.a13 + a22*M.a23 + a23*M.a33;

	Result.a31 = a31*M.a11 + a32*M.a21 + a33*M.a31;
	Result.a32 = a31*M.a12 + a32*M.a22 + a33*M.a32;
	Result.a33 = a31*M.a13 + a32*M.a23 + a33*M.a33;
	return Result;



}; 
Matrix33& Matrix33::operator=(const Matrix33 &M) {
		if(this == &M) return *this;
		a11 = M.a11;
		a12 = M.a12;
		a13 = M.a13;
		a21 = M.a21;
		a22 = M.a22;
		a23 = M.a23;
		a31 = M.a31;
		a32 = M.a32;
		a33 = M.a33;
		return *this; 
};
bool Matrix33::Inverse() {

	auto A = (*this).Det();
	A = A != 0.0f ? 1.0f/A : 0.0f;
	if(A == 0.0f) return false; //irreversible matrix

	Matrix33 Tmp(*this);
	Tmp.Transpose();
	
	a11 = A*(Tmp.a22*Tmp.a33 - Tmp.a23*Tmp.a32);
	a12 = A*(Tmp.a13*Tmp.a32 - Tmp.a12*Tmp.a33);
	a13 = A*(Tmp.a12*Tmp.a23 - Tmp.a13*Tmp.a22);

	a21 = A*(Tmp.a23*Tmp.a31 - Tmp.a21*Tmp.a33);
	a22 = A*(Tmp.a11*Tmp.a33 - Tmp.a13*Tmp.a31);
	a23 = A*(Tmp.a13*Tmp.a21 - Tmp.a11*Tmp.a23);

	a31 = A*(Tmp.a21*Tmp.a32 - Tmp.a22*Tmp.a31);
	a32 = A*(Tmp.a12*Tmp.a31 - Tmp.a11*Tmp.a32);
	a33 = A*(Tmp.a11*Tmp.a22 - Tmp.a12*Tmp.a21);
	return true;
};

float Matrix33::Det() {
	return a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a13*a22*a31 - a11*a23*a32 - a12*a21*a33;
};

const Matrix33& Matrix33::Transpose() {

	auto Tmp = a12;
	a12 = a21;
	a21 = Tmp;

	Tmp = a13;
	a13 = a31;
	a31 = Tmp;

	Tmp = a23;
	a23 = a32;
	a32 = Tmp;

	return *this;
};



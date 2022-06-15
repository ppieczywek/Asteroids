#pragma once
#include "stdafx.h"
#include "Matrix22.h"

const Matrix22& Matrix22::Transpose() {
	auto Tmp = a12;
	a12 = a21;
	a21 = Tmp;
	return *this;
};

bool Matrix22::Inverse() {

	auto A = a11*a22 - a12*a21;
	A = A != 0.0f ? 1.0f/A : 0.0f;
	if(A == 0.0f) return false; //irreversible matrix
	auto tmp = a22;

	a22 = a11*A;
	a11 = tmp*A;
	a12*=-A; 
	a21*=-A;
	return true;
};
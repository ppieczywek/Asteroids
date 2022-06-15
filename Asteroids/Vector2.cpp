#include "stdafx.h"
#include "Vector2.h"

const Vector2&	Vector2::normalize(void) {
	auto l = length(); 
	if(l>0){ 
		return (*this)/=l;
	} else {
		x = 0.0f; y=0.0f;
		return *this; };
};

const Vector2	Vector2::unit() const {
	auto l = length(); 
	if(l>0){ 
		return (*this)/l;
	} else {
		return Vector2();
	};		
};

void AngleNormalize(float &value) {
	if( value > tau ) {
		while(value > tau){ value-=tau;}
	}else if( value < taumin ) {
		while(value < taumin){ value+=tau;}
	};
};
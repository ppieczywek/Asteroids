#pragma once
#include "stdafx.h"
#include "Vector2.h"

class Rectangle2D{
public:
	Vector2 min,max;
	Rectangle2D() { min(0.0f,0.0f); max(0.0f,0.0f);};
	Rectangle2D(const float &xmin, const float &ymax, const float &xmax, const float &ymin) { min(xmin,ymin); max(xmin,ymin); };
};
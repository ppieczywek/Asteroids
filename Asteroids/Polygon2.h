#pragma once
#include "stdafx.h"
#include "Vector2.h"
#include "Rectangle2D.h"
#include "RotationMatrix.h"
#include <vector>
using namespace std;


class Polygon2 
{//A = std::move(B);
public:
					Polygon2();
					Polygon2( const vector<Vector2>& VertexList );
					Polygon2( const vector<Vector2>& VertexList, const vector<bool>& FlagList );
	
	unsigned		VertexNumber;
	unsigned		ValidEdgeNumber;
	Vector2			LocalCentroid;
	Vector2			GlobalCentroid;

	Vector2			LocalVertex[8];
	Vector2			LocalEdge[8];
	
	Vector2			LocalNormals[8];
	Vector2			GlobalVertex[8];
	Vector2			GlobalNormals[8];
	Vector2			Intervals[8];

	unsigned		ValidEdgeIndex[8];
		
	bool			EdgeFlag[8];
	bool			IsSensor;
};


		
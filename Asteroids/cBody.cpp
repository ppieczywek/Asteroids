#pragma once
#include "stdafx.h"
#include "RotationMatrix.h"
#include "cBody.h"



cBody::cBody() 
{
	SensorsNumber = 0;
	ID = -1;
	Position = ZeroVector;
	Mass = InverseMass = Inertia = InverseInertia = Angle = 0.0f;
	PolygonsNumber = 0;
	bType = dynamic2D;
	AngularMotion = true;
	AngularAcceleration = AngularVelocity = AngularVelocity = AngularAcceleration = AngularVelocity = LinearDamping = AngularDamping = 0.0f; 
	LinearAcceleration = LinearVelocity = ZeroVector;

	RestitutionCoefficient = 1.2f;
	FrictionCoefficient = 0.6f;
	TotalTranslationVector = ZeroVector;
	CumulativeTranslationVector = ZeroVector;
	NumberOfTranslations = 0.0f;

	PolygonsList.reserve(2);
};


cBody::~cBody() 
{	
	PolygonsList.clear();
};

void cBody::SetTransform(const float &NewAngle) 
{
	AngleNormalize(Angle=NewAngle);
	T(Angle,Position);
	BBox.min.x = BBox.max.x = Position.x;
	BBox.min.y = BBox.max.y = Position.y;
	
	for(unsigned ii = 0; ii < PolygonsNumber; ++ii)
	{
		for(unsigned jj = 0; jj < PolygonsList[ii].VertexNumber; ++jj)
		{
			PolygonsList[ii].GlobalVertex[jj] = T*PolygonsList[ii].LocalVertex[jj];
			if(BBox.max.x < PolygonsList[ii].GlobalVertex[jj].x) 
			{
				BBox.max.x = PolygonsList[ii].GlobalVertex[jj].x;
			}
			else
			{
				if(BBox.min.x > PolygonsList[ii].GlobalVertex[jj].x)
				{
					BBox.min.x = PolygonsList[ii].GlobalVertex[jj].x;
				}
			}
					
			if(BBox.max.y < PolygonsList[ii].GlobalVertex[jj].y) 
			{
				BBox.max.y = PolygonsList[ii].GlobalVertex[jj].y;
			}
			else
			{
				if(BBox.min.y > PolygonsList[ii].GlobalVertex[jj].y) 
				{
					BBox.min.y = PolygonsList[ii].GlobalVertex[jj].y;
				}
			}
		}
		PolygonsList[ii].GlobalCentroid = T * PolygonsList[ii].LocalCentroid;
	}
};


void cBody::SetTransform(const Vector2 &NewPosition)
{
	auto Offset = NewPosition - Position;
	Position = NewPosition;
	T(Angle,Position);
	for(unsigned ii = 0; ii < PolygonsNumber; ++ii)
	{
		for(unsigned jj = 0; jj < PolygonsList[ii].VertexNumber; ++jj)
		{
			PolygonsList[ii].GlobalVertex[jj] = T*PolygonsList[ii].LocalVertex[jj];
		};
		PolygonsList[ii].GlobalCentroid = T * PolygonsList[ii].LocalCentroid;
	}
	
	BBox.min+=Offset;
	BBox.max+=Offset;
};


void cBody::SetTransform(const Vector2 &NewPosition, const float &NewAngle) 
{
	Position = NewPosition;
	AngleNormalize(Angle=NewAngle);
	T(Angle,Position);
	BBox.min.x = BBox.max.x = Position.x;
	BBox.min.y = BBox.max.y = Position.y;

	for(unsigned ii = 0; ii < PolygonsNumber; ++ii)
	{
		for(unsigned jj = 0; jj < PolygonsList[ii].VertexNumber; ++jj)
		{
			PolygonsList[ii].GlobalVertex[jj] = T*PolygonsList[ii].LocalVertex[jj];
			if(BBox.max.x < PolygonsList[ii].GlobalVertex[jj].x) 
			{
				BBox.max.x = PolygonsList[ii].GlobalVertex[jj].x;
			}
			else
			{
				if(BBox.min.x > PolygonsList[ii].GlobalVertex[jj].x)
				{
					BBox.min.x = PolygonsList[ii].GlobalVertex[jj].x;
			}
			}
					
			if(BBox.max.y < PolygonsList[ii].GlobalVertex[jj].y) 
			{
				BBox.max.y = PolygonsList[ii].GlobalVertex[jj].y;
			}
			else
			{
				if(BBox.min.y > PolygonsList[ii].GlobalVertex[jj].y) 
				{
					BBox.min.y = PolygonsList[ii].GlobalVertex[jj].y;
				}
			}
		}
		PolygonsList[ii].GlobalCentroid = T * PolygonsList[ii].LocalCentroid;
	}
};
/*
void cBody::ScaleModel(const float &Xscale, const float &Yscale) 
{
	/*for(auto Model = Vertex.begin(); Model!=Vertex.end(); Model++) 
	{
		Model->x*=Xscale; 
		Model->y*=Yscale;
	};
	SetTransform(Position, Angle);*/
	//UpdateModel();
//};


void cBody::SetDensity(const float &M) 
{
	if(M <= 0.0f || PolygonsList.empty()) 
	{
		Mass = InverseMass = Inertia = InverseInertia = 0.0f;
	} else {


		vector<Vector2> TriangleCentroid;
		vector<float>	TriangleMass;

		float Area;
		float TotalArea = 0.0f;
		Density = M;
		float TotalCentroidX = 0.0f;
		float TotalCentroidY = 0.0f;
		Vector2 Centroid;
		float	Radius = 0.0f;


		Vector2 v1,v2;

		for each(auto Polygon in PolygonsList)
		{
			if(!Polygon.IsSensor)
			{
				
				for (unsigned ii = 1; ii< Polygon.VertexNumber; ++ii)
				{					
					v1 = Polygon.LocalVertex[ii-1] - Polygon.LocalCentroid;
					v2 = Polygon.LocalVertex[ii] - Polygon.LocalCentroid;
					
					Area =  abs( 0.5f * (v1.x*v2.y - v1.y*v2.x) );
					TotalArea += Area;

					Centroid = (Polygon.LocalVertex[ii-1] + Polygon.LocalVertex[ii] + Polygon.LocalCentroid) / 3.0f;
					TotalCentroidX += Centroid.x * Area;
					TotalCentroidY += Centroid.y * Area;

					TriangleMass.push_back(Area*Density);
					TriangleCentroid.push_back(Centroid);
				}

				v1 = Polygon.LocalVertex[Polygon.VertexNumber-1] - Polygon.LocalCentroid;
				v1 = Polygon.LocalVertex[0] - Polygon.LocalCentroid;
					
				Area =  abs( 0.5f * (v1.x*v2.y - v1.y*v2.x) );
				TotalArea += Area;
								
				Centroid = (Polygon.LocalVertex[Polygon.VertexNumber-1] + Polygon.LocalVertex[0] + Polygon.LocalCentroid) / 3.0f;
				TotalCentroidX += Centroid.x * Area;
				TotalCentroidY += Centroid.y * Area;

				TriangleMass.push_back(Area*Density);
				TriangleCentroid.push_back(Centroid);
			}
		}


		TotalCentroidX /= TotalArea;
		TotalCentroidY /= TotalArea;
		
		Vector2 TotalCentroid( TotalCentroidX, TotalCentroidY);

		Radius = 0.0f;
		Inertia = 0.0f;
		Mass = TotalArea * Density;

		for(unsigned ii = 0; ii <TriangleCentroid.size(); ++ii)
		{
			TriangleCentroid[ii] -= TotalCentroid;
			Radius = TriangleCentroid[ii].length();

			Inertia += Radius * Radius * TriangleMass[ii];
		}


		for (unsigned ii=0; ii < PolygonsNumber; ++ii)
		{
			PolygonsList[ii].LocalCentroid-=TotalCentroid;
			PolygonsList[ii].GlobalCentroid-=TotalCentroid;
			
			for (unsigned jj = 0; jj< PolygonsList[ii].VertexNumber; ++jj)
			{	
				PolygonsList[ii].LocalVertex[jj] -= TotalCentroid;
				PolygonsList[ii].GlobalVertex[jj] -= TotalCentroid;
			}
		}

		InverseInertia = 1.0f/Inertia;
		InverseMass = 1.0f/Mass;

	}
};


void cBody::UpdateNormals() {
	
	RotationMatrix T(Angle);
	
	for(unsigned ii = 0; ii < PolygonsNumber; ++ii)
	{
		for(unsigned jj = 0; jj < PolygonsList[ii].VertexNumber; ++jj)
		{
			PolygonsList[ii].GlobalNormals[jj] = T*PolygonsList[ii].LocalNormals[jj];
		}
		/*for(unsigned jj = 0; jj < PolygonsList[ii].ValidEdgeNumber; ++jj)
		{
			PolygonsList[ii].ValidBuffer[jj] = T*PolygonsList[ii].ValidEdges[jj];
		}*/
	}
};

void cBody::AddPolygon(const Polygon2& NewPolygon)
{
	PolygonsList.push_back(move(NewPolygon));
	PolygonsNumber++;
	if(NewPolygon.IsSensor) ++SensorsNumber;
	UpdateBodyBBox();
};


void cBody::UpdateBodyBBox() {

	if(PolygonsNumber)
	{
			BBox.min.x = BBox.max.x = Position.x;
			BBox.min.y = BBox.max.y = Position.y;
			for(unsigned ii = 0; ii < PolygonsNumber; ++ii)
			{
				for(unsigned jj = 0; jj < PolygonsList[ii].VertexNumber; ++jj)
				{
					if(BBox.max.x < PolygonsList[ii].GlobalVertex[jj].x) 
					{
						BBox.max.x = PolygonsList[ii].GlobalVertex[jj].x;
					}
					else
					{
						if(BBox.min.x > PolygonsList[ii].GlobalVertex[jj].x)
						{
							BBox.min.x = PolygonsList[ii].GlobalVertex[jj].x;
						}
					}
					
					if(BBox.max.y < PolygonsList[ii].GlobalVertex[jj].y) 
					{
						BBox.max.y = PolygonsList[ii].GlobalVertex[jj].y;
					}
					else
					{
						if(BBox.min.y > PolygonsList[ii].GlobalVertex[jj].y) 
						{
							BBox.min.y = PolygonsList[ii].GlobalVertex[jj].y;
						}
					}
					
				}
			}
	}
};



void cBody::UpdateFaceBBox() 
{
	/*for(int ii = 0; ii < FaceNumber; ii++)
	{
		
		FaceBBox[ii].min.x = VertexBuffer[ FaceList[ii].index[0]].x;
		FaceBBox[ii].max.x = VertexBuffer[ FaceList[ii].index[0]].x;
		FaceBBox[ii].min.y = VertexBuffer[ FaceList[ii].index[0]].y;
		FaceBBox[ii].max.y = VertexBuffer[ FaceList[ii].index[0]].y;

		if(FaceBBox[ii].min.x > VertexBuffer[ FaceList[ii].index[1]].x)
			FaceBBox[ii].min.x = VertexBuffer[ FaceList[ii].index[1]].x;
		else
			if(FaceBBox[ii].max.x < VertexBuffer[ FaceList[ii].index[1]].x)
				FaceBBox[ii].max.x = VertexBuffer[ FaceList[ii].index[1]].x;

		if(FaceBBox[ii].min.x > VertexBuffer[ FaceList[ii].index[2]].x)
			FaceBBox[ii].min.x = VertexBuffer[ FaceList[ii].index[2]].x;
		else
			if(FaceBBox[ii].max.x < VertexBuffer[ FaceList[ii].index[2]].x)
				FaceBBox[ii].max.x = VertexBuffer[ FaceList[ii].index[2]].x;


		if(FaceBBox[ii].min.y > VertexBuffer[ FaceList[ii].index[1]].y)
			FaceBBox[ii].min.y = VertexBuffer[ FaceList[ii].index[1]].y;
		else
			if(FaceBBox[ii].max.y < VertexBuffer[ FaceList[ii].index[1]].y)
				FaceBBox[ii].max.y = VertexBuffer[ FaceList[ii].index[1]].y;

		if(FaceBBox[ii].min.y > VertexBuffer[ FaceList[ii].index[2]].y)
			FaceBBox[ii].min.y = VertexBuffer[ FaceList[ii].index[2]].y;
		else
			if(FaceBBox[ii].max.y < VertexBuffer[ FaceList[ii].index[2]].y)
				FaceBBox[ii].max.y = VertexBuffer[ FaceList[ii].index[2]].y;
	};*/
};


void cBody::SetRestitutionCoefficient(const float &NewRestitutionCoefficient)
{
	if (NewRestitutionCoefficient < 1.0f) 
	{
		RestitutionCoefficient = 1.0f;
	}
	else if (NewRestitutionCoefficient > 2.0f) 
	{
		RestitutionCoefficient = 2.0f;
	}
	else 
	{
		RestitutionCoefficient = NewRestitutionCoefficient;
	}
};


void cBody::AddNonCollidable(const long long &Id)
{
	NonCollidable.insert(Id);
};


void cBody::RemoveNonCollidable(const long long &Id)
{
	if(!NonCollidable.empty()) 
	{
		NonCollidable.erase(Id);
	}
};


bool cBody::IsExcluded(const long long &Id)
{	
	if(!NonCollidable.empty()) 
	{
		if( bool is_in = NonCollidable.find(Id) != NonCollidable.end() ) return true;
	}
	return false;
};
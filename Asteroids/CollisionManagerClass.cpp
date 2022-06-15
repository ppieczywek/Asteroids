#pragma once
#include "stdafx.h"
#include "CollisionManagerClass.h"
#include "GameEngineClass.h"

#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>

ContactSolution::ContactSolution() : Body1(0), 
									 Body2(0), 
									 Face1(0), 
									 Face2(0), 
									 CumulativeMass(0.0f), 
									 Restitution(0.0f), 
									 Penetration(0.0f),
									 MinimumTranlationVector(0.0f,0.0f), 
									 CollisionNormal(0.0f,0.0f), 
									 CollisionTangent(0.0f,0.0f),
									 CollisionPoint(0.0f,0.0f), 
									 TangentImpulse(0.0f,0.0f), 
									 NormalImpulse(0.0f,0.0f), 
									 Overlap(0), 
									 Impulse(0),  
									 CollisionPointNumber(0), 
									 NormalImpulseMagnitude(0.0f),
									 TangentImpulseMagnitude(0.0f)
{

}; 




void ContactSolution::Clear() 
{
	CollisionPointNumber = Body1 = Body2 = Face1 = Face2 = 0;
	TangentImpulseMagnitude = NormalImpulseMagnitude = CumulativeMass = Restitution = Penetration = 0.0f;
	MinimumTranlationVector = CollisionPoint = CollisionTangent = CollisionNormal = NormalImpulse = TangentImpulse = ZeroVector;//translation
	Overlap = Impulse = 0;
};


CollisionManagerClass::CollisionManagerClass() : PenetrationTolerance(0.007f), 
												 BiasCoeff(8.0f) 
{
	MaxNumberOfSolutions = 300; 
	PhysicalContactList.assign(MaxNumberOfSolutions, ContactSolution()); 
	LogicalContactList.assign(MaxNumberOfSolutions, ContactSolution()); 
	ContactList.reserve(300);
};


CollisionManagerClass::~CollisionManagerClass() 
{ 
	ContactList.clear(); 
	
	PhysicalContactList.clear();
	LogicalContactList.clear();
};


void CollisionManagerClass::PreCalculateIntervals(vector<shared_ptr<cBody>>& BodyList)
{
	float d, min, max;
	unsigned Index;
	Vector2 Normal;
	for(unsigned kk=0; kk < BodyList.size();++kk)
	{
		for(unsigned ii=0; ii < BodyList[kk]->PolygonsNumber;++ii)
		{
			for(unsigned jj=0; jj< BodyList[kk]->PolygonsList[ii].ValidEdgeNumber; ++jj)
			{
				Index = BodyList[kk]->PolygonsList[ii].ValidEdgeIndex[jj];
				Normal = BodyList[kk]->PolygonsList[ii].GlobalNormals[Index];
				min = max = d = BodyList[kk]->PolygonsList[ii].GlobalVertex[0] * Normal;

				for(unsigned pp=1; pp<BodyList[kk]->PolygonsList[ii].VertexNumber; ++pp)
				{
					d = BodyList[kk]->PolygonsList[ii].GlobalVertex[pp]  * Normal; 
					if (d < min) min = d;	
					if (d > max) max = d; 
				}

				BodyList[kk]->PolygonsList[ii].Intervals[Index].x = min;//Index ->jj
				BodyList[kk]->PolygonsList[ii].Intervals[Index].y = max;
			}
		}
	}
};

void CollisionManagerClass::CalculateInterval(const Vector2 &Normal, const Polygon2 &Polygon, float& min, float& max)
{
   float d = Polygon.GlobalVertex[0] * Normal;
   min = max = d; 
   
   for(unsigned ii=1; ii<Polygon.VertexNumber; ++ii)
   {
		d = Polygon.GlobalVertex[ii]  * Normal; 
		if (d < min) min = d;	
		if (d > max) max = d; 
   }
};


bool CollisionManagerClass::IsAxisSeparating(const Polygon2& Polygon1, const Polygon2& Polygon2, const int& PolyIndex, const int& NormalIndex, Vector2 &Normal) //czêœc obliczeñ jest na darmo// powtarzaj¹ siê
{
	float mina, maxa; 
    float minb, maxb; 
	if(PolyIndex == 1)
	{
		mina = Polygon1.Intervals[NormalIndex].x;
		maxa = Polygon1.Intervals[NormalIndex].y;
		CalculateInterval(Normal, Polygon2, minb, maxb); 
	}

	if(PolyIndex == 2)
	{
		minb = Polygon2.Intervals[NormalIndex].x;
		maxb = Polygon2.Intervals[NormalIndex].y;
		CalculateInterval(Normal, Polygon1, mina, maxa); 
	}

    if (mina > maxb || minb > maxa) 
		      return true; 
    // find the interval overlap 
    float d0 = maxa - minb; 
    float d1 = maxb - mina; 
    float depth = (d0 < d1)? d0 : d1; 
	
	if(depth > PenetrationTolerance)
		depth-=PenetrationTolerance;
	
	if (depth > 0.0f) {
		Normal *= depth; 
		return false; 
	} else {
		return true;
	}
};


bool  CollisionManagerClass::AreTrianglesSeparated(const Polygon2& Polygon1, const Polygon2& Polygon2, Vector2 &PushVector,  int& Owner, int& Index ) 
{	
	Vector2 Normal(0.0f,0.0f);
	auto PushVectorLength = 400000.0f;
	auto NormalLength = 0.0f;
			
	for(unsigned ii=0; ii<Polygon1.ValidEdgeNumber; ++ii) 
	{

		Normal = Polygon1.GlobalNormals[ Polygon1.ValidEdgeIndex[ii] ]; 
		if( IsAxisSeparating(Polygon1, Polygon2, 1, ii, Normal)) return true;

		if(Polygon1.EdgeFlag[ Polygon1.ValidEdgeIndex[ii] ] == true)
		{
			NormalLength = (Normal.x*Normal.x)+(Normal.y*Normal.y);
			if (PushVectorLength > NormalLength) 
			{
				PushVector = Normal;	
				PushVectorLength = NormalLength;
				Owner = 1;
				Index =  Polygon1.ValidEdgeIndex[ii];//i; //ii - > Index
			}
		}
	}
		
	
	for(unsigned ii=0; ii<Polygon2.ValidEdgeNumber; ++ii) 
	{
		Normal = Polygon2.GlobalNormals[ Polygon2.ValidEdgeIndex[ii] ];
		if( IsAxisSeparating(Polygon1, Polygon2, 2, ii, Normal)) return true;

		if(Polygon2.EdgeFlag[ Polygon2.ValidEdgeIndex[ii] ] == true)
		{
			NormalLength = (Normal.x*Normal.x)+(Normal.y*Normal.y);
			if (PushVectorLength > NormalLength) 
			{
				PushVector = Normal;	
				PushVectorLength = NormalLength;
				Owner = 2;
				Index =  Polygon2.ValidEdgeIndex[ii];//ii;
			}
		}
	}

	return false;
};



bool CollisionManagerClass::IsPointInside(const Vector2 &pt, const vector<Polygon2>& PolygonsList, const unsigned& PolygonsNumber) {
	float PointProjection = 0.0f;
	for(unsigned ii=0; ii < PolygonsNumber; ++ii)
	{
		for(unsigned jj=0; jj < PolygonsList[ii].VertexNumber; ++jj)
		{
			PointProjection = (pt - PolygonsList[ii].GlobalVertex[jj]) * PolygonsList[ii].GlobalNormals[jj] ;
			if ( PointProjection >= 0.001f ) return false;
		}
	}	
	return true;
};


bool CollisionManagerClass::IsPointInside2(const Vector2 &pt, const Polygon2& Polygon) {
	float PointProjection = 0.0f;
	for(unsigned jj=0; jj < Polygon.VertexNumber; ++jj)
	{
		PointProjection = (pt - Polygon.GlobalVertex[jj]) * Polygon.GlobalNormals[jj] ;
		if ( PointProjection >= 0.001f ) return false;
	}
	
	return true;
};


void CollisionManagerClass::UpdateCollisionPoints(vector<shared_ptr<cBody>>& BodyList)
{
	unsigned B1,B2;
	float NumberOfPoints = 0.0f;
	ContactSolution *Solution;

	for(unsigned kk = 0; kk < PhysicalCollisionNumber; ++kk) //ContactList.size()
	{
		Solution =  &PhysicalContactList[ kk ];
		if(Solution->Overlap == 1) 
		{
			NumberOfPoints = 0.0f;
			B1 = Solution->Body1;
			B2 = Solution->Body2;
			Solution->CollisionPoint = ZeroVector;

			for(unsigned ii = 0; ii < BodyList[B1]->PolygonsNumber; ++ii)
			{
				for(unsigned jj = 0; jj < BodyList[B1]->PolygonsList[ii].VertexNumber; ++jj)
				{
					if(IsPointInside(BodyList[B1]->PolygonsList[ii].GlobalVertex[jj], BodyList[B2]->PolygonsList, BodyList[B2]->PolygonsNumber)) 
					{
						Solution->CollisionPoint += BodyList[B1]->PolygonsList[ii].GlobalVertex[jj];
						++NumberOfPoints;
					}
				}
			}

	
			for(unsigned ii = 0; ii < BodyList[B2]->PolygonsNumber; ++ii)
			{
				for(unsigned jj = 0; jj < BodyList[B2]->PolygonsList[ii].VertexNumber; ++jj)
				{
					if(IsPointInside(BodyList[B2]->PolygonsList[ii].GlobalVertex[jj], BodyList[B1]->PolygonsList, BodyList[B1]->PolygonsNumber)) 
					{
						Solution->CollisionPoint += BodyList[B2]->PolygonsList[ii].GlobalVertex[jj];
						++NumberOfPoints;
					}
				}
			}

			if(NumberOfPoints>0.0f) 
			{
				Solution->CollisionPoint /= NumberOfPoints;
				Solution->Overlap = 1;
				//return true;
			} 
			else 
			{
				Solution->Overlap = 0;
				//return false;
			}
		}
	}
};


void CollisionManagerClass::UpdateCollisionPoints2(vector<shared_ptr<cBody>>& BodyList)
{
	unsigned B1,B2;
	float NumberOfPoints = 0.0f;
	ContactSolution *Solution;
	Vector2 Buffer;
	float CentroidProjection = 0.0f;

	for(unsigned kk = 0; kk < PhysicalCollisionNumber; ++kk) 
	{
		Solution =  &PhysicalContactList[ kk ];
		if(Solution->Overlap == 1) 
		{
			NumberOfPoints = 0.0f;
			B1 = Solution->Body1;
			B2 = Solution->Body2;
			Solution->CollisionPoint = ZeroVector;

			
				CentroidProjection =  BodyList[B1]->PolygonsList[Solution->Face1].GlobalCentroid * Solution->CollisionNormal;
				for(unsigned jj = 0; jj < BodyList[B1]->PolygonsList[Solution->Face1].VertexNumber; ++jj)
				{
					Buffer =  BodyList[B1]->PolygonsList[Solution->Face1].GlobalVertex[jj];
					if( Buffer.x >= BodyList[B2]->BBox.min.x &&	Buffer.x <= BodyList[B2]->BBox.max.x &&
						Buffer.y <= BodyList[B2]->BBox.max.y &&	Buffer.y >= BodyList[B2]->BBox.min.y)
					{
						if( (Buffer * Solution->CollisionNormal) > CentroidProjection)
						{
							if(IsPointInside2(Buffer, BodyList[B2]->PolygonsList[Solution->Face2])) 
							{
								Solution->CollisionPoint += Buffer;
								++NumberOfPoints;
							}
						}
					}
				}
			
							
			
				CentroidProjection =  BodyList[B2]->PolygonsList[Solution->Face2].GlobalCentroid * Solution->CollisionNormal;
				for(unsigned jj = 0; jj < BodyList[B2]->PolygonsList[Solution->Face2].VertexNumber; ++jj)
				{
					Buffer =  BodyList[B2]->PolygonsList[Solution->Face2].GlobalVertex[jj];
					if( Buffer.x >= BodyList[B1]->BBox.min.x &&	Buffer.x <= BodyList[B1]->BBox.max.x &&
						Buffer.y <= BodyList[B1]->BBox.max.y &&	Buffer.y >= BodyList[B1]->BBox.min.y)
					{
						if( (Buffer * Solution->CollisionNormal) < CentroidProjection)
						{
							if(IsPointInside2(Buffer, BodyList[B1]->PolygonsList[Solution->Face1])) 
							{
								Solution->CollisionPoint += Buffer;
								++NumberOfPoints;
							}
						}
					}
				}
			
			if(NumberOfPoints>0.0f) 
			{
				Solution->CollisionPoint /= NumberOfPoints;
				Solution->Overlap = 1;
			} 
			else 
			{
				Solution->Overlap = 0;
			}
		}
	}
};



void CollisionManagerClass::UpdateCollisionPoints3(vector<shared_ptr<cBody>>& BodyList)
{
	unsigned B1,B2;
	float NumberOfPoints = 0.0f;
	ContactSolution *Solution;
	Vector2 Buffer;


	float TempProjection = 0.0f;
	float NormalProjection = 0.0f;
	unsigned EdgeIndex = 0;

	for(unsigned kk = 0; kk < PhysicalCollisionNumber; ++kk) 
	{
		Solution =  &PhysicalContactList[ kk ];
		if(Solution->Overlap == 1) 
		{
			NumberOfPoints = 0.0f;
			B1 = Solution->Body1;
			B2 = Solution->Body2;
			Solution->CollisionPoint = ZeroVector;
			
			TempProjection = 0.0f;
			NormalProjection = 0.0f;
			EdgeIndex = 0;
			
			for(unsigned jj = 0; jj < BodyList[B1]->PolygonsList[Solution->Face1].VertexNumber; ++jj)
			{
				TempProjection =  BodyList[B1]->PolygonsList[Solution->Face1].GlobalNormals[jj] * Solution->CollisionNormal;
				if( TempProjection > NormalProjection ) 
				{
					NormalProjection = TempProjection;
					EdgeIndex = jj;
				}
			}
		
			if(EdgeIndex == (BodyList[B1]->PolygonsList[Solution->Face1].VertexNumber-1))
			{
				Buffer = BodyList[B1]->PolygonsList[Solution->Face1].GlobalVertex[EdgeIndex];
				if(	Buffer.x >= BodyList[B2]->BBox.min.x &&	Buffer.x <= BodyList[B2]->BBox.max.x &&
					Buffer.y <= BodyList[B2]->BBox.max.y &&	Buffer.y >= BodyList[B2]->BBox.min.y)
				{
					if(IsPointInside2(Buffer, BodyList[B2]->PolygonsList[Solution->Face2])) 
					{
						Solution->CollisionPoint += Buffer;
						++NumberOfPoints;
					}
				}

				Buffer = BodyList[B1]->PolygonsList[Solution->Face1].GlobalVertex[0];
				if( Buffer.x >= BodyList[B2]->BBox.min.x &&	Buffer.x <= BodyList[B2]->BBox.max.x &&
					Buffer.y <= BodyList[B2]->BBox.max.y &&	Buffer.y >= BodyList[B2]->BBox.min.y)
				{
					if(IsPointInside2(Buffer, BodyList[B2]->PolygonsList[Solution->Face2])) 
					{
						Solution->CollisionPoint += Buffer;
						++NumberOfPoints;
					}
				}
			} 
			else
			{
				Buffer = BodyList[B1]->PolygonsList[Solution->Face1].GlobalVertex[EdgeIndex];
				if(	Buffer.x >= BodyList[B2]->BBox.min.x &&	Buffer.x <= BodyList[B2]->BBox.max.x &&
					Buffer.y <= BodyList[B2]->BBox.max.y &&	Buffer.y >= BodyList[B2]->BBox.min.y)
				{
					if(IsPointInside2(Buffer, BodyList[B2]->PolygonsList[Solution->Face2])) 
					{
						Solution->CollisionPoint += Buffer;
						++NumberOfPoints;
					}
				};

				Buffer = BodyList[B1]->PolygonsList[Solution->Face1].GlobalVertex[EdgeIndex+1];
				if(	Buffer.x >= BodyList[B2]->BBox.min.x &&	Buffer.x <= BodyList[B2]->BBox.max.x &&
					Buffer.y <= BodyList[B2]->BBox.max.y &&	Buffer.y >= BodyList[B2]->BBox.min.y)
				{
					if(IsPointInside2(Buffer, BodyList[B2]->PolygonsList[Solution->Face2])) 
					{
						Solution->CollisionPoint += Buffer;
						++NumberOfPoints;
					}
				}
			}

		
			TempProjection = 0.0f;
			NormalProjection = 0.0f;
			EdgeIndex = 0;
			for(unsigned jj = 0; jj < BodyList[B2]->PolygonsList[Solution->Face2].VertexNumber; ++jj)
			{
				TempProjection =  BodyList[B2]->PolygonsList[Solution->Face2].GlobalNormals[jj] * Solution->CollisionNormal;
				if( TempProjection < NormalProjection ) 
				{
					NormalProjection = TempProjection;
					EdgeIndex = jj;
				}
			}

			if(EdgeIndex == (BodyList[B2]->PolygonsList[Solution->Face2].VertexNumber-1))
			{
				Buffer = BodyList[B2]->PolygonsList[Solution->Face2].GlobalVertex[EdgeIndex];
				if(	Buffer.x >= BodyList[B1]->BBox.min.x &&	Buffer.x <= BodyList[B1]->BBox.max.x &&
					Buffer.y <= BodyList[B1]->BBox.max.y &&	Buffer.y >= BodyList[B1]->BBox.min.y)
				{
					if(IsPointInside2(Buffer, BodyList[B1]->PolygonsList[Solution->Face1])) 
					{
						Solution->CollisionPoint += Buffer;
						++NumberOfPoints;
					}
				}

				Buffer = BodyList[B2]->PolygonsList[Solution->Face2].GlobalVertex[0];
				if(	Buffer.x >= BodyList[B1]->BBox.min.x && Buffer.x <= BodyList[B1]->BBox.max.x &&
					Buffer.y <= BodyList[B1]->BBox.max.y &&	Buffer.y >= BodyList[B1]->BBox.min.y)
				{
					if(IsPointInside2(Buffer, BodyList[B1]->PolygonsList[Solution->Face1])) 
					{
						Solution->CollisionPoint += Buffer;
						++NumberOfPoints;
					}
				}
			} 
			else
			{
				Buffer = BodyList[B2]->PolygonsList[Solution->Face2].GlobalVertex[EdgeIndex];
				if(	Buffer.x >= BodyList[B1]->BBox.min.x &&	Buffer.x <= BodyList[B1]->BBox.max.x &&
					Buffer.y <= BodyList[B1]->BBox.max.y &&	Buffer.y >= BodyList[B1]->BBox.min.y)
				{
					if(IsPointInside2(Buffer, BodyList[B1]->PolygonsList[Solution->Face1])) 
					{
						Solution->CollisionPoint += Buffer;
						++NumberOfPoints;
					}
				}

				Buffer = BodyList[B2]->PolygonsList[Solution->Face2].GlobalVertex[EdgeIndex+1];
				if(	Buffer.x >= BodyList[B1]->BBox.min.x &&	Buffer.x <= BodyList[B1]->BBox.max.x &&
					Buffer.y <= BodyList[B1]->BBox.max.y &&	Buffer.y >= BodyList[B1]->BBox.min.y)
				{
					if(IsPointInside2(Buffer, BodyList[B1]->PolygonsList[Solution->Face1])) 
					{
						Solution->CollisionPoint += Buffer;
						++NumberOfPoints;
					}
				}
			}


			if(NumberOfPoints>0.0f) 
			{
				Solution->CollisionPoint /= NumberOfPoints;
				Solution->Overlap = 1;
				//return true;
			} 
			else 
			{
				Solution->Overlap = 0;
				//return false;
			}
		}
	}
};


void CollisionManagerClass::UpdatePhysicalContact(vector<shared_ptr<cBody>>& BodyList)
{	
	unsigned B1,B2,F1,F2, N;
	ContactSolution *Solution;
	

	for(unsigned ii = 0; ii < PhysicalCollisionNumber; ++ii) //ContactList.size()
	{
		Solution =  &PhysicalContactList[ ii ];
		if(Solution->Overlap == 1) 
		{
			if(abs(Solution->NormalImpulseMagnitude) > MinImpulse) 
			{
				B1 = Solution->Body1;
				B2 = Solution->Body2;
				F1 = Solution->Face1;
				F2 = Solution->Face2;
				N  = Solution->NormalIndex;

				Solution->CollisionPointNumber = 0;
				Solution->CollisionPoint = ZeroVector;
				TempMinimumTranslationVector = ZeroVector;

				if(Solution->OwnerIndex == 1)														
					TempMinimumTranslationVector = BodyList[B1]->PolygonsList[F1].GlobalNormals[N]; 
				else if(Solution->OwnerIndex == 2)
					TempMinimumTranslationVector = BodyList[B2]->PolygonsList[F2].GlobalNormals[N]; 

				
				if(!IsAxisSeparating(BodyList[B1]->PolygonsList[F1], BodyList[B2]->PolygonsList[F2], Solution->OwnerIndex, N, TempMinimumTranslationVector))
				{
					Solution->MinimumTranlationVector = TempMinimumTranslationVector;
					if ( (BodyList[B2]->Position - BodyList[B1]->Position) * Solution->MinimumTranlationVector < 0.0f) 
					{
						Solution->MinimumTranlationVector *= -1.0f;
					}
					Solution->Penetration =  -0.5f*Solution->MinimumTranlationVector.length();
					Solution->CollisionNormal = Solution->MinimumTranlationVector.unit();
					Solution->Overlap = 1;	
				}
				else 
				{
					Solution->Overlap = 0;	
				}
			} 
			else
			{
				Solution->Overlap = 0;	
			}
		}
	}
};


void CollisionManagerClass::AdjustContainersSize()
{
	CurrentNumberOfSolutions = ContactList.size();
	if(CurrentNumberOfSolutions > MaxNumberOfSolutions) 
	{
		PhysicalContactList.resize(CurrentNumberOfSolutions, ContactSolution() );
		LogicalContactList.resize(CurrentNumberOfSolutions, ContactSolution() );
		MaxNumberOfSolutions = CurrentNumberOfSolutions;
	};

	if(CurrentNumberOfSolutions < 2*MaxNumberOfSolutions) 
	{
		PhysicalContactList.resize(CurrentNumberOfSolutions, ContactSolution() );
		LogicalContactList.resize(CurrentNumberOfSolutions, ContactSolution() );
		MaxNumberOfSolutions = CurrentNumberOfSolutions;
	};

}


void CollisionManagerClass::BroadPhaseDetection(vector< shared_ptr<cBody>>& BodyList)
{		
	ContactList.clear();
	ContactList.reserve(300);
		
	set<pair<int,int>> ContactSet;	
    //Grid.ResolveGrid(BodyList, ContactSet);
	QuadTree.ResolveTree(BodyList, ContactSet);

	if(!ContactSet.empty())
	{
		ContactList.resize(ContactSet.size());
		ContactList.assign(ContactSet.begin(), ContactSet.end());
	}
};

void CollisionManagerClass::NarrowPhaseDetection(vector< shared_ptr<cBody> >  &BodyList)
{
	
	PhysicalCollisionNumber = LogicalCollisionNumber = 0;

	if(!ContactList.empty()){
			
		AdjustContainersSize();

		for(unsigned ii = 0; ii<BodyList.size(); ++ii) 
		{
			BodyList[ ii ]->UpdateNormals();
		};

		PreCalculateIntervals(BodyList);
		
		unsigned			B1,B2;
		ContactSolution*	Solution;

		for(unsigned ii = 0; ii < ContactList.size(); ++ii)
		{
		
			B1 = ContactList[ii].first;
 			B2 = ContactList[ii].second;
			
 		  if( !BodyList[B1]->IsExcluded(BodyList[B2]->GetID()) &&  !BodyList[B2]->IsExcluded(BodyList[B1]->GetID()) )
		  {

			BodyOverlap = 0;
			TempMinimumTranslationVector = ZeroVector;
			MinimumTranslationVectorLength = 0.0f;
 			LongestMinimumTranslationVector = 0.0f;

			/************************BODY PHYSICAL COLLISIONS*********************/
			for(unsigned qq=0; qq < BodyList[B1]->PolygonsNumber; ++qq) 
			{
				if(BodyList[B1]->PolygonsList[qq].IsSensor == false)
				{
					for(unsigned pp=0; pp < BodyList[B2]->PolygonsNumber; ++pp) 
					{
						if(BodyList[B2]->PolygonsList[pp].IsSensor == false)
						{						
							if(!AreTrianglesSeparated( BodyList[B1]->PolygonsList[qq], BodyList[B2]->PolygonsList[pp], TempMinimumTranslationVector, OIndex, NIndex)) 
							{		
												
								BodyOverlap = 1;//body overlap
								MinimumTranslationVectorLength = TempMinimumTranslationVector.x*TempMinimumTranslationVector.x +
				  											     TempMinimumTranslationVector.y*TempMinimumTranslationVector.y;

								if( LongestMinimumTranslationVector < MinimumTranslationVectorLength) 
								{							
									LongestMinimumTranslationVector =  MinimumTranslationVectorLength;
									MinimumTranlationVector = TempMinimumTranslationVector;
									tempFace1 = qq;
									tempFace2 = pp;
									tempOIndex = OIndex;
									tempNIndex = NIndex;
								}
							}
						}
					}
				TempMinimumTranslationVector = ZeroVector;
				}	
			}

			if(BodyOverlap == 1) 
			{				
				if(BodyList[B1]->cType==Logical || BodyList[B2]->cType==Logical) 
				{
					Solution = &LogicalContactList[LogicalCollisionNumber];					
					Solution->Clear();
					Solution->cType = Logical;
				} 
				else 
				{
					Solution = &PhysicalContactList[PhysicalCollisionNumber];					
					Solution->Clear();
					Solution->cType = Physical;
				};

				
  				Solution->Body1 = B1;
				Solution->Body2 = B2;
				Solution->Overlap = BodyOverlap;
				Solution->MinimumTranlationVector = MinimumTranlationVector;
				Solution->Face1 = tempFace1;
				Solution->Face2 = tempFace2;
				Solution->OwnerIndex = tempOIndex;
				Solution->NormalIndex = tempNIndex;

				if ( (BodyList[B2]->PolygonsList[tempFace2].GlobalCentroid - BodyList[B1]->PolygonsList[tempFace1].GlobalCentroid) * Solution->MinimumTranlationVector < 0.0f) 
				{
					Solution->MinimumTranlationVector *= -1;
				}

				Solution->Penetration =  -0.5f*sqrt(LongestMinimumTranslationVector);
				Solution->CollisionNormal = Solution->MinimumTranlationVector.unit();
				Solution->CumulativeMass = BodyList[B1]->InverseMass + BodyList[B2]->InverseMass;
				
				if(BodyList[B1]->bType == dynamic2D &&  BodyList[B2]->bType == dynamic2D){
					Solution->CorrectionCoeff1 = 0.5f;
					Solution->CorrectionCoeff2 = 0.5f;
				} else if(BodyList[B1]->bType != dynamic2D) {
					Solution->CorrectionCoeff1 = 0.0f;
					Solution->CorrectionCoeff2 = 2.0f;
				} else {
					Solution->CorrectionCoeff1 = 2.0f;
					Solution->CorrectionCoeff2 = 0.0f;
				};	

				Solution->Restitution = max(BodyList[B1]->RestitutionCoefficient, BodyList[B2]->RestitutionCoefficient);
									
				Solution->NormalImpulseMagnitude = 100000.0f;
				if(Solution->cType == Logical)
				{
					++LogicalCollisionNumber;
				}
				else
				{
					++PhysicalCollisionNumber;
				}
			}

			/************************SENSOR AND BODY LOGICAL COLLISIONS*********************/
			if( BodyList[B1]->SensorsNumber > 0)
			{
				SensorOverlap = 0;
				for(unsigned qq=0; qq < BodyList[B1]->PolygonsNumber; ++qq) 
				{
					if(BodyList[B1]->PolygonsList[qq].IsSensor == true)
					{
						TempMinimumTranslationVector = ZeroVector;
						MinimumTranslationVectorLength = 0.0f;
						LongestMinimumTranslationVector = 0.0f;

						for(unsigned pp=0; pp < BodyList[B2]->PolygonsNumber; ++pp) 
						{
								if(!AreTrianglesSeparated( BodyList[B1]->PolygonsList[qq], BodyList[B2]->PolygonsList[pp], TempMinimumTranslationVector, OIndex, NIndex)) 
								{
						
									SensorOverlap = 1;//body overlap
									MinimumTranslationVectorLength = TempMinimumTranslationVector.x*TempMinimumTranslationVector.x +
				  												     TempMinimumTranslationVector.y*TempMinimumTranslationVector.y;

									if( LongestMinimumTranslationVector < MinimumTranslationVectorLength) 
									{							
										LongestMinimumTranslationVector =  MinimumTranslationVectorLength;
										MinimumTranlationVector = TempMinimumTranslationVector;
										tempFace1 = qq;
										tempFace2 = pp;
										tempOIndex = OIndex;
										tempNIndex = NIndex;
									}
								}
						}

						if(SensorOverlap == 1)
						{

							Solution = &LogicalContactList[LogicalCollisionNumber];					
							Solution->Clear();
							Solution->cType = Logical;
				
							Solution->Body1 = B1;
							Solution->Body2 = B2;
							Solution->Overlap = SensorOverlap;
							Solution->MinimumTranlationVector = MinimumTranlationVector;
							Solution->Face1 = tempFace1;
							Solution->Face2 = tempFace2;
							Solution->OwnerIndex = tempOIndex;
							Solution->NormalIndex = tempNIndex;

							//if ( (BodyList[B2]->Position - BodyList[B1]->Position) * Solution->MinimumTranlationVector < 0.0f) 
							if ( (BodyList[B2]->PolygonsList[tempFace2].GlobalCentroid - BodyList[B1]->PolygonsList[tempFace1].GlobalCentroid) * Solution->MinimumTranlationVector < 0.0f) 
							{
								Solution->MinimumTranlationVector *= -1;
							}

							Solution->Penetration =  -0.5f*sqrt(LongestMinimumTranslationVector);
							Solution->CollisionNormal = Solution->MinimumTranlationVector.unit();
							++LogicalCollisionNumber;
							if(LogicalCollisionNumber >= CurrentNumberOfSolutions)
							{
								LogicalContactList.push_back(ContactSolution());
							}
							SensorOverlap = 0;
						}
					}
				}
			}



			if( BodyList[B2]->SensorsNumber > 0)
			{
				SensorOverlap = 0;
				for(unsigned qq=0; qq < BodyList[B2]->PolygonsNumber; ++qq) 
				{
					if(BodyList[B2]->PolygonsList[qq].IsSensor == true)
					{
						TempMinimumTranslationVector = ZeroVector;
						MinimumTranslationVectorLength = 0.0f;
						LongestMinimumTranslationVector = 0.0f;

						for(unsigned pp=0; pp < BodyList[B1]->PolygonsNumber; ++pp) 
						{
							//if(BodyList[B1]->PolygonsList[pp].IsSensor == false)
							//{
								if(!AreTrianglesSeparated( BodyList[B2]->PolygonsList[qq], BodyList[B1]->PolygonsList[pp], TempMinimumTranslationVector, OIndex, NIndex)) 
								{						
									SensorOverlap = 1;//body overlap
									MinimumTranslationVectorLength = TempMinimumTranslationVector.x*TempMinimumTranslationVector.x +
				  												     TempMinimumTranslationVector.y*TempMinimumTranslationVector.y;

									if( LongestMinimumTranslationVector < MinimumTranslationVectorLength) 
									{							
										LongestMinimumTranslationVector =  MinimumTranslationVectorLength;
										MinimumTranlationVector = TempMinimumTranslationVector;
										tempFace2 = qq;
										tempFace1 = pp;
										tempOIndex = OIndex;
										tempNIndex = NIndex;
									}
								}
							//}
						}

						if(SensorOverlap == 1)
						{

							Solution = &LogicalContactList[LogicalCollisionNumber];					
							Solution->Clear();
							Solution->cType = Logical;
				
							Solution->Body1 = B1;
							Solution->Body2 = B2;
							Solution->Overlap = SensorOverlap;
							Solution->MinimumTranlationVector = MinimumTranlationVector;
							Solution->Face1 = tempFace1;
							Solution->Face2 = tempFace2;
							Solution->OwnerIndex = tempOIndex;
							Solution->NormalIndex = tempNIndex;

							//if ( (BodyList[B2]->Position - BodyList[B1]->Position) * Solution->MinimumTranlationVector < 0.0f) 
							if ( (BodyList[B2]->PolygonsList[tempFace2].GlobalCentroid - BodyList[B1]->PolygonsList[tempFace1].GlobalCentroid) * Solution->MinimumTranlationVector < 0.0f) 
							{
								Solution->MinimumTranlationVector *= -1;
							}

							Solution->Penetration = -0.5f*sqrt(LongestMinimumTranslationVector);
							Solution->CollisionNormal = Solution->MinimumTranlationVector.unit();
							++LogicalCollisionNumber;
							if(LogicalCollisionNumber >= CurrentNumberOfSolutions)
							{
								LogicalContactList.push_back(ContactSolution());
							}
							SensorOverlap = 0;
						}
					}
				}
			}

		  }

		}
	}
};


void CollisionManagerClass::SolveCollisions(GameObjectManagerClass& ObjectManager, WorldPropertiesClass& WorldProperties) 
{
	
	vector< shared_ptr<cBody> > &BodyList = ObjectManager.cBodyList;
	
	ContactSolution* Solution;
	unsigned B1,B2;
	if(PhysicalCollisionNumber != 0)
	{
	
		PreCalculateIntervals(BodyList);
		UpdatePhysicalContact(BodyList);
		UpdateCollisionPoints3(BodyList);
		UpdateCollisionImpulse(BodyList,WorldProperties.GravityValue, WorldProperties.TimeStep);
						
		for(unsigned ii = 0; ii < PhysicalCollisionNumber; ++ii) 
			{					
				Solution = &PhysicalContactList[ ii ];
				B1 = Solution->Body1;
				B2 = Solution->Body2;
								
				if(Solution->Overlap == 1) {
					if(Solution->Impulse == 1) 
					{
						BodyList[B1]->ApplyImpulse( -Solution->NormalImpulse, Solution->CollisionPoint);
						BodyList[B2]->ApplyImpulse(  Solution->NormalImpulse, Solution->CollisionPoint);
						if(WorldProperties.Friction) 
						{
							BodyList[B1]->ApplyImpulse( -Solution->TangentImpulse, Solution->CollisionPoint);
							BodyList[B2]->ApplyImpulse(  Solution->TangentImpulse, Solution->CollisionPoint);
						}
					}
											
					BodyList[B1]->AccumulateTranslation(-Solution->CorrectionCoeff1 * Solution->MinimumTranlationVector);
					BodyList[B2]->AccumulateTranslation( Solution->CorrectionCoeff2 * Solution->MinimumTranlationVector);
				}
			}
			
			for(unsigned ii=0; ii < BodyList.size(); ++ii) 
			{
				BodyList[ii]->SetTransform( (BodyList[ii]->GetPosition() +  BodyList[ii]->GetTotalTranslationVector()) );
			}
	};
};


void CollisionManagerClass::UpdateCollisionImpulse(vector<shared_ptr<cBody>>& BodyList,const Vector2& GravityValue, const float& TimeStep)
{
	unsigned B1,B2;
	ContactSolution *Solution;

	for(unsigned ii = 0; ii < PhysicalCollisionNumber; ++ii) //ContactList.size()
	{
		Solution =  &PhysicalContactList[ ii ];
		if(Solution->Overlap == 1) 
		{
			B1 = Solution->Body1;
			B2 = Solution->Body2;
			RelativeVelocity = BodyList[B2]->GetLinearVelocityAtPoint(Solution->CollisionPoint) - BodyList[B1]->GetLinearVelocityAtPoint(Solution->CollisionPoint);
			VelocityMagnitude = RelativeVelocity * Solution->CollisionNormal;
	
			if(VelocityMagnitude < VelocityThreshold) 
			{
	
				VelocityMagnitude += Solution->Penetration*BiasCoeff;
				R1 = Solution->CollisionPoint - BodyList[B1]->Position;
				R2 = Solution->CollisionPoint - BodyList[B2]->Position;
				RN_1 = VectorProduct(R1, Solution->CollisionNormal);
				RN_2 = VectorProduct(R2, Solution->CollisionNormal);
		
				Denominator = Solution->CumulativeMass + (RN_1*RN_1)* (BodyList[B1]->InverseInertia) + (RN_2*RN_2)*(BodyList[B2]->InverseInertia);
				Denominator = 1.0f / Denominator;

				if(VelocityMagnitude < PlasticCollisionThreshold)
				{
					Solution->NormalImpulseMagnitude = -1.0f*(Solution->Restitution*VelocityMagnitude)*Denominator;
				}
				else
				{
					Solution->NormalImpulseMagnitude = -1.0f*VelocityMagnitude*Denominator;
				}
				Solution->NormalImpulse = Solution->NormalImpulseMagnitude*Solution->CollisionNormal;
		

		/********TANGENT IMPULSE*********/
				NormalPart = VectorProduct(RelativeVelocity, Solution->CollisionNormal);
				if(  NormalPart < 0.0f )	
				{
					Solution->CollisionTangent.x = -Solution->CollisionNormal.y;
					Solution->CollisionTangent.y =  Solution->CollisionNormal.x;
				}
				else
				{
					Solution->CollisionTangent.x =  Solution->CollisionNormal.y;
					Solution->CollisionTangent.y = -Solution->CollisionNormal.x;
				};
			
				RN_1 = VectorProduct(R1,Solution->CollisionTangent);
				RN_2 = VectorProduct(R2,Solution->CollisionTangent);
				
				Denominator = Solution->CumulativeMass + ( ((RN_1*RN_1) * BodyList[B1]->InverseInertia) + ((RN_2*RN_2) * BodyList[B2]->InverseInertia) ) ;
				//if( abs(NormalPart) > 0.05f ) 
				{
					Bias = 2.0f*GravityValue * TimeStep * Solution->CollisionTangent * Solution->CollisionTangent; //grawitacja

					Solution->TangentImpulseMagnitude = -1.0f*((RelativeVelocity+Bias) * Solution->CollisionTangent)/Denominator;
		 		 
					if( 0.55f*abs(Solution->NormalImpulseMagnitude) < abs(Solution->TangentImpulseMagnitude) ) 
					{
						Solution->TangentImpulse = -0.4f*abs(Solution->NormalImpulseMagnitude) * Solution->CollisionTangent;// + Bias;
					} 
					else
					{
						Solution->TangentImpulse = Solution->TangentImpulseMagnitude*Solution->CollisionTangent;
					};
				};	
				Solution->Impulse = 1;
			} 
			else 
			{
				Solution->Impulse = 0;
			};

		};
	};
};


#pragma once
#include "stdafx.h"
#include "TCGrid.h"


TCGrid::TCGrid()
{
	for(auto ii=0;ii<16;++ii)
	{
		Buckets[ii].assign(100,0);
		BucketSize[ii] = 0;
	};

	IdToRegions.reserve(300);
	IdRegions.reserve(300);


	PreviousBodyCount = 0;
	LargeRegionsSubs[0][0] = 0;
	LargeRegionsSubs[0][1] = 1;
	LargeRegionsSubs[0][2] = 4;
	LargeRegionsSubs[0][3] = 5;

	LargeRegionsSubs[1][0] = 2;
	LargeRegionsSubs[1][1] = 3;
	LargeRegionsSubs[1][2] = 6;
	LargeRegionsSubs[1][3] = 7;

	LargeRegionsSubs[2][0] = 8;
	LargeRegionsSubs[2][1] = 9;
	LargeRegionsSubs[2][2] = 12;
	LargeRegionsSubs[2][3] = 13;

	LargeRegionsSubs[3][0] = 10;
	LargeRegionsSubs[3][1] = 11;
	LargeRegionsSubs[3][2] = 14;
	LargeRegionsSubs[3][3] = 15;
	
};


TCGrid::~TCGrid()
{
	IdToRegions.clear();	
	IdRegions.clear();	
	ToCheck.clear();
};

void TCGrid::Split(const Rectangle2D& Rec, Rectangle2D QTreeRec[4])
{	
	float HalfWidth = (Rec.max.x - Rec.min.x)/2;
	float HalfHeight = (Rec.max.y - Rec.min.y)/2;
			
	QTreeRec[0].min.x = Rec.min.x;
	QTreeRec[0].max.x = Rec.min.x + HalfWidth;
	QTreeRec[0].max.y = Rec.max.y;
	QTreeRec[0].min.y = Rec.max.y - HalfHeight;
	
	QTreeRec[1].min.x = QTreeRec[0].max.x;
	QTreeRec[1].max.x = Rec.max.x;
	QTreeRec[1].max.y = Rec.max.y;
	QTreeRec[1].min.y = QTreeRec[0].min.y;
	
	QTreeRec[2].min.x = QTreeRec[0].min.x;
	QTreeRec[2].max.x = QTreeRec[0].max.x;
	QTreeRec[2].max.y = QTreeRec[0].min.y;
	QTreeRec[2].min.y = Rec.min.y;
	
	QTreeRec[3].min.x = QTreeRec[0].max.x;
	QTreeRec[3].max.x = Rec.max.x;
	QTreeRec[3].min.y =  Rec.min.y;
	QTreeRec[3].max.y = QTreeRec[0].min.y;

}
void TCGrid::DropBody(const Rectangle2D& Rec, const unsigned& Index)
{
	if (BoxesIntersect(WorldBox, Rec))
	{
	for(auto ii=0; ii<4; ++ii)
	{
		if (BoxesIntersect( LargeRegions[ii], Rec ))
		{
			for(auto jj=0; jj<4; ++jj)
			{	
				CellIndex = LargeRegionsSubs[ii][jj];
				
				if (BoxesIntersect( SmallRegions[CellIndex], Rec ))
				{
					IdToRegions[Index][IdRegions[Index]] = CellIndex;
					IdRegions[Index]++;
					
				}
				if(Rec.min.y > SmallRegions[CellIndex].min.y && Rec.max.x < SmallRegions[CellIndex].max.x) break;
			}
		}
		if(Rec.min.y > LargeRegions[ii].min.y && Rec.max.x < LargeRegions[ii].max.x) break;
	}
	
	} 
	else
	{
		IdRegions[Index] = -1;
	}

};


void TCGrid::SetWorldBox(const Rectangle2D& Box)
{
	WorldBox = Box;
	Split(WorldBox, LargeRegions);

	auto IncrementX = (WorldBox.max.x - WorldBox.min.x) / 4.0f; //4
	auto IncrementY = (WorldBox.max.y - WorldBox.min.y) / 4.0f;

	for(auto jj=0; jj<4; ++jj)  //4
	{
		for(auto ii=0; ii<4; ++ii) //4
		{

			SmallRegions[jj*4 + ii].min.x =  WorldBox.min.x + ii*IncrementX;
			SmallRegions[jj*4 + ii].max.x =  WorldBox.min.x + (ii+1)*IncrementX;

			SmallRegions[jj*4 + ii].min.y =  WorldBox.max.y - (jj+1)*IncrementY;
			SmallRegions[jj*4 + ii].max.y =  WorldBox.max.y - jj*IncrementY;

		}
	}
};


void TCGrid::Initialize( vector<shared_ptr<cBody>> &BodyList)
{

};

void TCGrid::ResolveGrid( vector<shared_ptr<cBody>> &BodyList, set<pair<int,int>> &ContactList)
{


	unsigned BodyCount = BodyList.size();
		
	if(BodyCount != 0)
	{		
		unsigned Index1,Index2;
		

		if(Buckets[0].size() < BodyCount)
		{
			for(auto ii=0;ii<16;++ii)
			{
				Buckets[ii].resize(BodyCount,0);
			}
		}

		for(auto ii=0;ii<16;++ii)
		{
			BucketSize[ii] = 0;
		}
		
		if(PreviousBodyCount == 0)
		{
			IdToRegions.resize(BodyCount);
			IdRegions.resize(BodyCount);
			IdRegions.assign(BodyCount,0);
		
			for(unsigned ArrayIndex=0; ArrayIndex < BodyCount; ++ArrayIndex)
			{		
				if(BodyList[ ArrayIndex ]->IsCollidable())
				{
					DropBody(BodyList[ ArrayIndex ]->BBox, ArrayIndex);
				}
				else
				{
					IdRegions[ArrayIndex] = -1;
				}

			}

		} 
		else if(PreviousBodyCount != 0)
		{
			if(PreviousBodyCount != BodyCount)
			{
				IdToRegions.resize(BodyCount);
				IdRegions.resize(BodyCount,0);
			}
			ToCheck.clear();
			ToCheck.reserve(150);

			unsigned Count = min(BodyCount, PreviousBodyCount);

			for(unsigned ii=0; ii<Count; ++ii)
			{	
				if( IdRegions[ii] > 1  || IdRegions[ii] < 0 )
				 {
					 IdRegions[ii] = 0;
					 ToCheck.push_back(ii);
				 }
				 else if( IdRegions[ii] == 1 )
				 {	
					 if(!BoxesEmbedd(  SmallRegions[IdToRegions[ii][0]]  ,  BodyList[ii]->BBox  ))
					 {
						IdRegions[ii] = 0;
						ToCheck.push_back(ii);
					 }
				 }
			}

			for(unsigned ii=0;ii<ToCheck.size(); ++ii)
			{			
				if(BodyList[ ToCheck[ii] ]->IsCollidable())
				{
					DropBody(BodyList[ ToCheck[ii] ]->BBox, ToCheck[ii] );
				}
				else
				{
					IdRegions[ToCheck[ii]] = -1;
				}
			}

			if(PreviousBodyCount < BodyCount)
			{
				for(unsigned ii = PreviousBodyCount; ii<BodyCount; ++ii)
				{
					if(BodyList[ ii ]->IsCollidable())
					{	
						DropBody(BodyList[ ii ]->BBox, ii );
					}
					else
					{
						IdRegions[ii] = -1;
					}
				}

			}

		}


		int CellIndex;
		for(int ii=0; ii < BodyCount; ++ii)
		{
			for(int jj=0; jj < IdRegions[ii]; ++jj)
			{
				CellIndex = IdToRegions[ii][jj];
				if(CellIndex > 0)
				{
					Buckets[ CellIndex ][ BucketSize[CellIndex] ] = ii;
					BucketSize[ CellIndex ]++;
				}
			}
		}
				
		PreviousBodyCount = BodyCount;

		for(auto ii=0; ii<16; ++ii)
		{
			if(BucketSize[ii] > 1)
			{
				for(unsigned kk = 0; kk < BucketSize[ii]-1 ; ++kk) 
				{
					Index1 = Buckets[ii][kk];
					for(unsigned jj = kk+1 ;jj < BucketSize[ii]; ++jj) 
					{
					
						Index2 = Buckets[ii][jj];
							
						if (BoxesIntersect( BodyList[Index1]->BBox, BodyList[Index2]->BBox )) 
						{
							if( BodyList[Index1]->IsDynamic() || BodyList[Index2]->IsDynamic() ) 
							{
								if( BodyList[Index1]->IsCollidable() && BodyList[Index2]->IsCollidable() ) 
								{ 
									ContactList.insert( pair<int,int>( Index1 , Index2 ) ); 
								}
							}
						}

					}
				}
			}
		}
	}
};


void TCGrid::Render()
{
		glLineWidth(1.5f);
		glBegin(GL_LINES);
			glColor3f(0.0f, 0.0f, 0.0f);	
			
			glVertex3f( WorldBox.min.x ,  WorldBox.max.y  , 0.0f );
			glVertex3f( WorldBox.max.x ,  WorldBox.max.y  , 0.0f );
						
			glVertex3f( WorldBox.max.x ,  WorldBox.max.y  , 0.0f );
			glVertex3f( WorldBox.max.x ,  WorldBox.min.y  , 0.0f );

			glVertex3f( WorldBox.max.x ,  WorldBox.min.y  , 0.0f );
			glVertex3f( WorldBox.min.x ,  WorldBox.min.y  , 0.0f );

			glVertex3f( WorldBox.min.x ,  WorldBox.min.y  , 0.0f );
			glVertex3f( WorldBox.min.x ,  WorldBox.max.y  , 0.0f );
			
		glEnd();
};
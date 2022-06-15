#pragma once
#include "stdafx.h"
#include "Polygon2.h"

Polygon2::Polygon2()
{
		
		VertexNumber = 0;
		IsSensor = false;
};


Polygon2::Polygon2( const vector<Vector2>& VertexList ) 
{
	 VertexNumber = VertexList.size(); 
	 ValidEdgeNumber = 0;
	 bool ValidEdgesFlags[8];
	 for(unsigned jj = 0; jj < VertexNumber; ++jj)
	 {
		LocalVertex[jj] = VertexList[jj];
		GlobalVertex[jj] = VertexList[jj];
		EdgeFlag[jj] = true;
		ValidEdgesFlags[jj] = true;
	 };
	 
	 LocalCentroid = ZeroVector;

	 for(unsigned jj = 0; jj < (VertexNumber-1); ++jj)
	 {
		LocalEdge[jj] = LocalVertex[jj+1] - LocalVertex[jj];
		LocalCentroid +=  LocalVertex[jj];
	 }
	 auto Last = VertexNumber-1;
	 LocalEdge[Last] = LocalVertex[0] - LocalVertex[Last];
	 LocalCentroid +=  LocalVertex[Last];

	 LocalCentroid /=  static_cast<float>(VertexNumber);
	 GlobalCentroid =  LocalCentroid;
	
	 
	 for(unsigned ii = 0; ii < VertexNumber; ++ii)
	 {
		LocalNormals[ii] = CCW*LocalEdge[ii].unit();
		GlobalNormals[ii] = LocalNormals[ii];
	 }

	 
	 for(unsigned ii=0; ii < VertexNumber-1; ++ii)
	 {
		for(unsigned jj=ii+1; jj < VertexNumber; ++jj)
		{
			if ((1 - abs(LocalNormals[ii] * LocalNormals[jj])) < 0.01f)
			{
				ValidEdgesFlags[jj] = false; 
				/*if (EdgeFlag[ii] == true  && EdgeFlag[jj] == true)
				{
					ValidEdgesFlags[jj] = false;               
				}
				
          		if( EdgeFlag[ii] == false  && EdgeFlag[jj] == true)
				{
					ValidEdgesFlags[ii] = false;               
				}
           
			    if(EdgeFlag[ii] == true  && EdgeFlag[jj] == false)
				{
					ValidEdgesFlags[jj] = false;               
				}
                      
				if(EdgeFlag[ii] == false  && EdgeFlag[jj] == false)
				{
					ValidEdgesFlags[ii] = false;               
					ValidEdgesFlags[jj] = false;               
				}*/
			}
		}
	 }

	 for(unsigned ii=0; ii < VertexNumber;++ii)
	 {
		 if(ValidEdgesFlags[ii])
		 {
			ValidEdgeIndex[ValidEdgeNumber] = ii;
			ValidEdgeNumber++;
		 }
	 }
	             
         
	 IsSensor = false;	
};

Polygon2::Polygon2( const vector<Vector2>& VertexList, const vector<bool>& FlagList )
{
	 VertexNumber = VertexList.size(); 
	 ValidEdgeNumber = 0;
	 bool ValidEdgesFlags[8];
	 for(unsigned jj = 0; jj < VertexNumber; ++jj)
	 {
		LocalVertex[jj] = VertexList[jj];
		GlobalVertex[jj] = VertexList[jj];
		EdgeFlag[jj] = FlagList[jj];
		ValidEdgesFlags[jj] = true;
	 };
	 
	 LocalCentroid = ZeroVector;

	 for(unsigned jj = 0; jj < (VertexNumber-1); ++jj)
	 {
		LocalEdge[jj] = LocalVertex[jj+1] - LocalVertex[jj];
		LocalCentroid +=  LocalVertex[jj];
	 }
	 auto Last = VertexNumber-1;
	 LocalEdge[Last] = LocalVertex[0] - LocalVertex[Last];
	 LocalCentroid +=  LocalVertex[Last];

	 LocalCentroid /=  static_cast<float>(VertexNumber);
	 GlobalCentroid =  LocalCentroid;
	
	 
	 for(unsigned ii = 0; ii < VertexNumber; ++ii)
	 {
		LocalNormals[ii] = CCW*LocalEdge[ii].unit();
		GlobalNormals[ii] = LocalNormals[ii];
	 }

	 
	 for(unsigned ii=0; ii < VertexNumber-1; ++ii)
	 {
		for(unsigned jj=ii+1; jj < VertexNumber; ++jj)
		{
			if ((1 - abs(LocalNormals[ii] * LocalNormals[jj])) < 0.01f)
			{
				ValidEdgesFlags[jj] = false;  
				/*if (EdgeFlag[ii] == true  && EdgeFlag[jj] == true)
				{
					ValidEdgesFlags[jj] = false;               
				}
				
          		if( EdgeFlag[ii] == false  && EdgeFlag[jj] == true)
				{
					ValidEdgesFlags[ii] = false;               
				}
           
			    if(EdgeFlag[ii] == true  && EdgeFlag[jj] == false)
				{
					ValidEdgesFlags[jj] = false;               
				}
                      
				if(EdgeFlag[ii] == false  && EdgeFlag[jj] == false)
				{
					ValidEdgesFlags[ii] = false;               
					ValidEdgesFlags[jj] = false;               
				}*/
			}
		}
	 }

	 for(unsigned ii=0; ii < VertexNumber;++ii)
	 {
		 if(ValidEdgesFlags[ii])
		 {
			ValidEdgeIndex[ValidEdgeNumber] = ii;
			ValidEdgeNumber++;
		 }
	 }
	             
         
	 IsSensor = false;	

}
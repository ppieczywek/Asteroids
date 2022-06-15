#pragma once
#include "stdafx.h"
#include "cRender.h"


cRender::cRender() : IsVisible(false), LineWidth(3.8f) {
	
	DrawLines = true;
	DrawPolygons = true;

	Alpha = 1.0f;
	Layer = 1.0f;
	BodyColor.x = 0.95f;
	BodyColor.y = 0.95f;
	BodyColor.z = 0.95f;

	LineColor.x = 0.0f;
	LineColor.y = 0.0f;
	LineColor.z = 0.0f;
};

void cRender::Draw(const vector<Polygon2>& PolygonList, const unsigned& PolygonsNumber)
{
	if(IsVisible) 
	{
		if( PolygonsNumber != 0 )
		{
		
			if(DrawPolygons)
			{
			for(unsigned ii=0; ii < PolygonsNumber; ++ii)
			{
		
				glBegin(GL_TRIANGLE_FAN);
				glColor4f(BodyColor.x, BodyColor.y, BodyColor.z, Alpha);	
				glVertex3f(  PolygonList[ii].GlobalCentroid.x ,  PolygonList[ii].GlobalCentroid.y  , Layer );
				

				for( unsigned jj = 0; jj < PolygonList[ii].VertexNumber; ++jj ) 
				{ 
					glColor4f(BodyColor.x, BodyColor.y, BodyColor.z, Alpha);	
					glVertex3f( PolygonList[ii].GlobalVertex[jj].x ,  PolygonList[ii].GlobalVertex[jj].y  , Layer );
				};

				glColor4f(BodyColor.x, BodyColor.y, BodyColor.z, Alpha);	
				glVertex3f( PolygonList[ii].GlobalVertex[0].x ,  PolygonList[ii].GlobalVertex[0].y  , Layer );
				glEnd();
			}
			}

		if(DrawLines)
		{
			glLineWidth(LineWidth);
			glBegin(GL_LINES);
			glColor4f(LineColor.x, LineColor.y, LineColor.z, Alpha);	
			for(unsigned ii=0; ii < PolygonsNumber; ++ii)
			{
				for(unsigned jj = 0; jj < PolygonList[ii].VertexNumber-1; ++jj) 
				{	
					glVertex3f( PolygonList[ii].GlobalVertex[jj].x ,  PolygonList[ii].GlobalVertex[jj].y  , Layer - 0.05f );
					glVertex3f( PolygonList[ii].GlobalVertex[jj+1].x ,  PolygonList[ii].GlobalVertex[jj+1].y  , Layer - 0.05f );
				}
				auto Last =  PolygonList[ii].VertexNumber-1;
				glVertex3f( PolygonList[ii].GlobalVertex[Last].x ,  PolygonList[ii].GlobalVertex[Last].y  , Layer - 0.05f );
				glVertex3f( PolygonList[ii].GlobalVertex[0].x ,  PolygonList[ii].GlobalVertex[0].y  , Layer - 0.05f );
			};
			
			glEnd();
		}
		}
	};
};


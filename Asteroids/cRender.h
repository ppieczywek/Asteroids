#pragma once
#define GLEW_STATIC
#include "Vector2.h"
#include "Vector3.h"
#include "Entity.h"
#include "cBody.h"
#include <vector>

#include <GL/glew.h>
#include <GL/wglew.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using namespace std;


class cRender {
protected:
	long long	ID;
	bool		IsVisible;
	float		LineWidth;
	float		Alpha;
	Vector3		BodyColor;
	Vector3		LineColor;
		
	float		Layer;
	bool		DrawLines;
	bool		DrawPolygons;

public:
		
						cRender();
	virtual				~cRender() {};

	inline void			SetID(const long long&); 
	inline long long	GetID(); 
	inline void			SetVisible(const bool&);
	inline bool			GetVisible();
	inline void			SetLineWidth(const float&);
	inline float		GetLineWidth();
	inline void			SetTranspercyValue(const float&);
	inline float		GetTranspercyValue();

	inline void			SetLayer(const float&);
	inline float		GetLayer();


	inline void			SetBodyColor(Vector3&);
	inline Vector3		GetBodyColor();

	inline void			SetLineColor(Vector3&);
	inline Vector3		GetLineColor();

	inline void			SetDrawLines(const bool&);
	inline bool			GetDrawLines();

	inline void			SetDrawPolygons(const bool&);
	inline bool			GetDrawPolygons();

	virtual void		Draw(const vector<Polygon2>&, const unsigned&);
};


void cRender::SetDrawLines(const bool& VisibleFlag) 
{
	DrawLines = VisibleFlag;
};


bool cRender::GetDrawLines()
{
	return DrawLines;
};

void cRender::SetDrawPolygons(const bool& VisibleFlag) 
{
	DrawPolygons = VisibleFlag;
};


bool cRender::GetDrawPolygons()
{
	return DrawPolygons;
};


void cRender::SetBodyColor(Vector3& Value)
{
	BodyColor = Value;
};


Vector3	cRender::GetBodyColor()
{
	return BodyColor;
};


void cRender::SetLineColor(Vector3& Value)
{
	LineColor = Value;		
};


Vector3 cRender::GetLineColor()
{
	return LineColor;
};


void cRender::SetID(const long long& NewID) 
{
	ID = NewID;
};


long long	cRender::GetID() 
{
	return ID;
};


void cRender::SetVisible(const bool& VisibleFlag) 
{
	IsVisible = VisibleFlag;
};


bool cRender::GetVisible()
{
	return IsVisible;
};


void cRender::SetLineWidth(const float& NewLineWidth)  
{
	LineWidth = NewLineWidth;
};


float cRender::GetLineWidth()
{
	return LineWidth;
};

void cRender::SetTranspercyValue(const float& Value)
{
	if(Value >= 0.0f)
	{
		Alpha = Value <= 1.0f ? Value : 1.0f;
	}
	else
	{
		Alpha = 0.0f;
	}
};

float cRender::GetTranspercyValue()
{
	return Alpha;
};

void  cRender::SetLayer(const float& Value)
{
	Layer = Value;
};

float cRender::GetLayer()
{
	return Layer;
};
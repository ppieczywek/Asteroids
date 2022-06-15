
#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "glu32.lib") 
#pragma comment(lib, "opengl32.lib") 



class CameraClass {
private:
	shared_ptr<Entity>		CameraTarget;
	Vector2					TargetPosition;
	Vector2					CumulativeMean;
	Vector2					CameraPosition;
	Vector2					CameraOffset;
	float					AspecRatio;
	float					Width;
	float					Height;
	float					Alpha;
	float					Zoom;
	bool					AutoZoom;

public:
							CameraClass();
							~CameraClass();
	
	void					UpdateCameraPosition();
	void					SetFocusAtEntity(shared_ptr<Entity>&);
	void					SetFocusAtFixedPoint(const Vector2&);
	void					SetOffset(const Vector2&);
	void					SetAlpha(const float&);
	void					SetAspecRatio(const float&);
	void					SetWidth(const float&);
	void					SetHeight(const float&);
	void					SetAutoZoom(bool&);

	inline Vector2			GetCameraOffset();
	inline Vector2			GetCameraPosition();
	inline float			GetAlpha();
	inline float			GetAspecRatio();
	inline float			GetWidth();
	inline float			GetHeight();
	inline bool				GetAutoZoom();
};


Vector2 CameraClass::GetCameraOffset()
{
	return CameraOffset;
};


Vector2 CameraClass::GetCameraPosition()	
{
	return CameraPosition;
};


float CameraClass::GetAlpha()
{
	return Alpha;
};


float CameraClass::GetAspecRatio()
{
	return AspecRatio;
};


float CameraClass::GetWidth()
{
	return Width;
};


float CameraClass::GetHeight()
{
	return Height;
};


bool CameraClass::GetAutoZoom()		
{
	return AutoZoom;
};
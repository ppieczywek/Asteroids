#pragma once
#include "stdafx.h"
#include "CameraClass.h"

CameraClass::CameraClass() : CameraTarget(nullptr),
							 TargetPosition(0.0f,0.0f),
							 CumulativeMean(0.0f,0.0f),
							 CameraPosition(0.0f,0.0f),
							 CameraOffset(0.0f,0.0f),
							 AspecRatio(1.333f),
							 Width(60.0f),
							 Alpha(0.01f),
							 Zoom(1.0f),
							 AutoZoom(true)
{ 
	Height = Width/AspecRatio;
};


CameraClass::~CameraClass() 
{
	CameraTarget=nullptr;
};


void CameraClass::SetAlpha(const float &NewAlpha) 
{
	if(NewAlpha<0.0f) 
	{ 
		Alpha = 0.0f;
	} 
	else if(NewAlpha>1.0f) 
	{
		Alpha = 1.0f; 
	}
	else 
	{
		Alpha = NewAlpha;
	}
};


void CameraClass::SetAspecRatio(const float &NewAspecRatio) 
{
	if(NewAspecRatio>0.0f) 
	{
		AspecRatio = NewAspecRatio;
		Height = Width/NewAspecRatio;
	};
};


void CameraClass::SetHeight(const float &NewHeight) 
{
	if(NewHeight>0.0f) 
	{
		Height = NewHeight;
		Width = Height * AspecRatio;
	};
};


void CameraClass::SetWidth(const float &NewWidth) 
{
	if(NewWidth>0.0f) 
	{
		Width = NewWidth;
		Height = Width/AspecRatio;
	};
};


void CameraClass::SetFocusAtEntity(shared_ptr<Entity> &Entity)  
{
	if(Entity!=nullptr) 
	{
		CameraTarget = Entity;
		TargetPosition = CameraTarget->p_cBody->GetPosition();
		CameraPosition = CumulativeMean = TargetPosition + CameraOffset;
	};
};


void CameraClass::SetFocusAtFixedPoint(const Vector2 &FocusPoint) 
{
	CameraTarget = nullptr;
	TargetPosition = FocusPoint;
	CameraPosition = CumulativeMean = TargetPosition + CameraOffset;
};


void CameraClass::SetOffset(const Vector2 &OffsetValue) 
{
	CameraOffset = OffsetValue;
};


void CameraClass::UpdateCameraPosition()
{

	if(CameraTarget!=nullptr)
	{
		TargetPosition = CameraTarget->p_cBody->GetPosition();
	};
	
	CumulativeMean = (Alpha * TargetPosition) + (1.0f - Alpha) * CumulativeMean;
	auto prevPos = CameraPosition;
	
	CameraPosition = CumulativeMean + CameraOffset;
	
	if(AutoZoom)
	{
		auto kk  = ((CameraPosition - prevPos).length()*0.2f / 0.005f) / 1.5f;
		if(kk > 1.8f) kk = 1.8f;
		if(kk < 1.0f) kk = 1.0f;

		Zoom = (0.006f * kk) + (1.0f - 0.006f) * Zoom;
	} 
	else
	{
		Zoom = 1.0f;
	}
	auto HW = Zoom*Width/2.0f;
	auto HH = Zoom*Height/2.0f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(CameraPosition.x-HW, CameraPosition.x+HW, CameraPosition.y-HH, CameraPosition.y+HH, -50.0f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

};
#pragma once
#include "stdafx.h"
#include "CTime.h"


CTime::CTime() : 
		m_pfnUpdate(0),
		m_fBaseTime(0.0f),
		m_fLastTime(0.0f),
		m_fSecsPerTick(0.0f),
		m_fCurrentTime(0.0f),
		m_fFrameTime(0.0f),
		m_dTimerStart(0),
		m_dCurTime(0)
{
}

CTime::~CTime()
{	m_pfnUpdate = 0;
}


void CTime::Init()
{
	_int64	dTicksPerSec =0;

	if (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&dTicksPerSec)))
	{ 
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&m_dTimerStart));
		m_fSecsPerTick = (1.0f)/(static_cast<double>(dTicksPerSec));
		m_pfnUpdate = &CTime::GetPerformanceCounterTime;
	}
}


void CTime::Update()
{
	m_fCurrentTime =  (this->*m_pfnUpdate)() - m_fBaseTime;
	m_fFrameTime = m_fCurrentTime - m_fLastTime;
	m_fLastTime = m_fCurrentTime;
}

void CTime::Reset()
{
	m_fBaseTime =  (this->*m_pfnUpdate)();
	m_fLastTime = m_fCurrentTime = m_fFrameTime = 0.0;
}


double CTime::GetPerformanceCounterTime()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&m_dCurTime));
	return (m_fSecsPerTick * (m_dCurTime - m_dTimerStart));
}

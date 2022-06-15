// COTD Entry submitted by Gaz Iqbal [devvoid@home.com]
#pragma once
#include "stdafx.h"
#include <Windows.h>

/*
====================================================
A Simple Timer class which tries to use the 
Win32 HighPerformance Timer. Will default to 
the Windows Multimedia timer if the High Performance
Timer is not found.

I call the GetCurrent/Frame time functions from a 
pair of globally accessible functions which I define 
in namespace System{} to hide the implementation of 
CTime from all the other game code.
====================================================
*/
class CTime
{
public:

	CTime();
	~CTime();

	void Init();
	void Reset();
	
	//Call this onces per game frame
	void Update();

	//Access funcs
	inline const double & GetCurrentTime() const { return m_fCurrentTime; }
	inline const double & GetFrameTime()   const { return m_fFrameTime;   } 
	double	GetPerformanceCounterTime();

private:

	typedef double (CTime::* TimeUpdateFunc) ();
	TimeUpdateFunc m_pfnUpdate;

	double	m_fBaseTime;
	double	m_fLastTime;
	double	m_fSecsPerTick;

	double	m_fCurrentTime;
	double	m_fFrameTime;

	//QueryPerformancesCounter related 64bit integers.
	//These are Microsoft specific, and will have to be changed for
	//different compilers.
	_int64	m_dTimerStart;
	_int64  m_dCurTime;

	//One of these gets bound to the TimeUpdateFunc pointer
	
};

#pragma once
#include "stdafx.h"
#include "ContactEventClass.h"

ContactEvent::ContactEvent()
{
	Source = 0;
	SourcePolygon = 0;
	Target = 0;
	TargetPolygon = 0;
	Solution = nullptr;
};


ContactEvent::ContactEvent(int& NewSource, int& NewSourcePolygon, int& NewTarget, int& NewTargetPolygon, ContactSolution& NewSolution)
{
	Source = NewSource;
	SourcePolygon = NewSourcePolygon;
	Target = NewTarget;
	TargetPolygon = NewTargetPolygon;
	Solution = &NewSolution;
};
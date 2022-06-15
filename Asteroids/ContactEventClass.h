#pragma once
#include "stdafx.h"
#include "CollisionManagerClass.h"

class ContactEvent
{
public:
						ContactEvent();
						ContactEvent(int&, int&, int&, int&, ContactSolution& );
	unsigned			Source;
	unsigned			SourcePolygon;
	unsigned			Target;
	unsigned			TargetPolygon;
	ContactSolution*	Solution;	
};
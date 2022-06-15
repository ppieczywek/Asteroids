#pragma once
#include "stdafx.h"
#include "LinearizedQuadTreeClass.h"


LinearizedQuadTreeClass::LinearizedQuadTreeClass() : DefaultTreeDepth(3), NodesNumber(0) {

	TreeDepth = DefaultTreeDepth;

	IterationStepSize.assign(TreeDepth,0);
	
	auto Step = TreeDepth - 1;
	int StepSize = 0;
	for(auto ii=0; ii < TreeDepth; ++ii) {

		NodesNumber+= pow(float(4),ii+1);
		StepSize += pow(float(4),ii);
		IterationStepSize[Step--] = StepSize;
	};

	Nodes.assign(NodesNumber, Rectangle2D());

};

void LinearizedQuadTreeClass::Resolve( vector< shared_ptr<Entity> > &BodyList, set<int> &IndexList, vector<pair<int,int>> &ContactList) {


	//set<int> Index(IndexList.begin(), IndexList.end());

	//auto ArraySize = static_cast<unsigned>(BodyList.size()); 
	//dynamic_array<bool> CoincidenceMatrix(ArraySize,ArraySize,false);

	Vector2 Position;
	Position = BodyList[ *(IndexList.begin()) ]->p_cBody->GetPosition();
	WorldBox.max.x = WorldBox.min.x = Position.x;
	WorldBox.max.y = WorldBox.min.y = Position.y;

	for each(auto BodyIndex in IndexList) 
	{
		Position = BodyList[ BodyIndex ]->p_cBody->GetPosition();

		if( WorldBox.min.x > Position.x ) WorldBox.min.x = Position.x;
		if( WorldBox.max.x < Position.x ) WorldBox.max.x = Position.x;
		if( WorldBox.min.y > Position.y ) WorldBox.min.y = Position.y;
		if( WorldBox.max.y < Position.y ) WorldBox.max.y = Position.y;

	};

	unsigned Index = 0;
	for(auto Level=0; Level < TreeDepth; ++Level) 
	{
		
		for(auto ii=0; ii<4; ++ii)
		{



		}


	};
};
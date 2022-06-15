#pragma once
#include "stdafx.h"
#include "dynamic_array.h"
#include "QTree2.h"
#include <array>
#include <vector>
#include <set>


class TCGrid
{
	
	

	Rectangle2D		 WorldBox;
	Rectangle2D		 LargeRegions[4];
	unsigned		 LargeRegionsSubs[4][4];
	Rectangle2D		 SmallRegions[16];//16
	unsigned		PreviousBodyCount;
	int CellIndex;
	
	vector<array<int,16U>>	IdToRegions;	
	vector<int>	IdRegions;	


	vector<int> ToCheck;

	vector<unsigned>	Buckets[16];//16
	unsigned			BucketSize[16];//16

	vector<bool>	 Mask;

	void			Split(const Rectangle2D& Rec, Rectangle2D GridRec[4]);
	void			Initialize( vector<shared_ptr<cBody>> &BodyList);
	void			DropBody(const Rectangle2D& Rec, const unsigned&);
public:
					TCGrid();
					~TCGrid();
	void			SetWorldBox(const Rectangle2D&);
	void			ResolveGrid( vector<shared_ptr<cBody>> &BodyList, set<pair<int,int>> &ContactList);
	void			Render();

};
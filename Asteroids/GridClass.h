#pragma once
#include "stdafx.h"
#include "Ententity.h"
#include "cBody.h"
#include "cMove.h"
#include "cRender.h"
#include "cCollide.h"
#include "QTree.h"
#include "dynamic_array.h"
#include <vector>
#include <set>

//bool BoxesIntersect(const Rectangle2D &A, const Rectangle2D &B);
//bool AreExcluded(const shared_ptr<cBody const> &P1, const shared_ptr<cBody const> &P2);

class GridClass{
private:
	//Rectangle2D		GridMainRec[2][2];
	//vector<int>		GridMainRec[4];
	//GridCell		GridCells[4][4];
	vector<int>		EntityList;
	unsigned		EntityNumber;
	unsigned		DefaultCappacity;
	Rectangle2D		WorldBox;
	float Xmin[4];
	float Xmax[4];
	float Ymin[4];
	float Ymax[4];
public:
					GridClass() {
						EntityNumber = 0;
						DefaultCappacity = 50;
						EntityList.assign(DefaultCappacity,0);
					};
					~GridClass() {};
	void			GridClass::Resolve( vector< shared_ptr<Entity> > &BodyList, set<int> &IndexList, vector<pair<int,int>> &ContactList);
};
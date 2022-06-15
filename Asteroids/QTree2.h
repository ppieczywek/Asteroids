#pragma once
#include "stdafx.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "dynamic_array.h"
#include <vector>
#include <set>

bool BoxesIntersect(const Rectangle2D &A, const Rectangle2D &B);
bool BoxesEmbedd(const Rectangle2D &A, const Rectangle2D &B);


class Node
{
public:
						Node();
	unsigned			NumberOfElements;
	vector<int>			ElementsIndexList;
	Rectangle2D			Region;
	unique_ptr<Node>	ChildrenNodes[4];

};


class QTree2{
private:
	Rectangle2D			WorldBox;
	static unsigned		MaxNodeCapacity;
	static int			MaxLevel;
	static unsigned		MaxListSize;
	unique_ptr<Node>	Root;

	void			Create(unique_ptr<Node>&, int Level);
	void			ResolveNode(unique_ptr<Node>&, vector< shared_ptr<cBody> >&, set<pair<int,int>> &, int Level = MaxLevel);
	void			Split(const Rectangle2D& Rec, Rectangle2D QTreeRec[4]);
	void			Split(const Rectangle2D& Rec, const float &XValue, const float &YValue, Rectangle2D QTreeRec[4]);
public:
					QTree2();
					~QTree2();
	void			ResolveTree( vector< shared_ptr<cBody> >&, set<pair<int,int>> &ContactList);
	static void		SetMaxLevel( int Level );
	static void		SetNodeCapacity( int NodeCapacity );
};
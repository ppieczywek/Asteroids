#pragma once
#include "stdafx.h"
#include "QTree2.h"



bool BoxesIntersect(const Rectangle2D &A, const Rectangle2D &B)
{
    if (A.max.x < B.min.x) return false;
    if (A.min.x > B.max.x) return false;
    if (A.max.y < B.min.y) return false;
    if (A.min.y > B.max.y) return false;
    return true;
};

bool BoxesEmbedd(const Rectangle2D &A, const Rectangle2D &B)
{
    if ( (A.max.x > B.max.x) && (A.min.x < B.min.x) && (A.max.y > B.max.y) && (A.min.y < B.min.y)  ) 
		return true;
	else
		return false;
};

Node::Node()
{
	NumberOfElements = 0;
	ElementsIndexList.assign(60,0);
	ChildrenNodes[0] = nullptr;
	ChildrenNodes[1] = nullptr;
	ChildrenNodes[2] = nullptr;
	ChildrenNodes[3] = nullptr;
};

QTree2::QTree2()
{	

	Root = unique_ptr<Node>( new Node());
	Create(Root, MaxLevel);

};


void QTree2::Create(unique_ptr<Node>& TreeNode, int Level)
{
	for(auto ii=0; ii<4; ++ii)
	{
		if(Level != 0)
		{
			TreeNode->ChildrenNodes[ii] = unique_ptr<Node>( new Node());
			Create(TreeNode->ChildrenNodes[ii], Level-1);
		}
	}
};

void QTree2::Split(const Rectangle2D& Rec, Rectangle2D QTreeRec[4])
{	
	float HalfWidth = (Rec.max.x - Rec.min.x)/2;
	float HalfHeight = (Rec.max.y - Rec.min.y)/2;
			
	QTreeRec[0].min.x = Rec.min.x;
	QTreeRec[0].max.x = Rec.min.x + HalfWidth;
	QTreeRec[0].max.y = Rec.max.y;
	QTreeRec[0].min.y = Rec.max.y - HalfHeight;
	
	QTreeRec[1].min.x = QTreeRec[0].min.x;
	QTreeRec[1].max.x = QTreeRec[0].max.x;
	QTreeRec[1].max.y = QTreeRec[0].min.y;
	QTreeRec[1].min.y = Rec.min.y;
	
	QTreeRec[2].min.x = QTreeRec[1].max.x;
	QTreeRec[2].max.x = Rec.max.x;
	QTreeRec[2].max.y = QTreeRec[1].max.y;
	QTreeRec[2].min.y = QTreeRec[1].min.y;
	
	QTreeRec[3].min.x = QTreeRec[2].min.x;
	QTreeRec[3].max.x = QTreeRec[2].max.x;
	QTreeRec[3].min.y = QTreeRec[2].max.y;
	QTreeRec[3].max.y = Rec.max.y;
};

void QTree2::Split(const Rectangle2D& Rec, const float &XValue, const float &YValue, Rectangle2D QTreeRec[4])
{					
	QTreeRec[0].min.x = Rec.min.x;
	QTreeRec[0].max.x = XValue;
	QTreeRec[0].max.y = Rec.max.y;
	QTreeRec[0].min.y = YValue;
	
	QTreeRec[1].min.x = QTreeRec[0].min.x;
	QTreeRec[1].max.x = QTreeRec[0].max.x;
	QTreeRec[1].max.y = QTreeRec[0].min.y;
	QTreeRec[1].min.y = Rec.min.y;
	
	QTreeRec[2].min.x = QTreeRec[1].max.x;
	QTreeRec[2].max.x = Rec.max.x;
	QTreeRec[2].max.y = QTreeRec[1].max.y;
	QTreeRec[2].min.y = QTreeRec[1].min.y;
	
	QTreeRec[3].min.x = QTreeRec[2].min.x;
	QTreeRec[3].max.x = QTreeRec[2].max.x;
	QTreeRec[3].min.y = QTreeRec[2].max.y;
	QTreeRec[3].max.y = Rec.max.y;
};

QTree2::~QTree2()
{

};


void QTree2::ResolveTree(vector< shared_ptr<cBody> >& BodyList, set<pair<int,int>> &ContactList) 
{
	if(BodyList.size() > 1)
	{
		Vector2 Position;
		Position = BodyList[ 0 ]->GetPosition();
		WorldBox.max.x = WorldBox.min.x = Position.x;
		WorldBox.max.y = WorldBox.min.y = Position.y;
		Root->NumberOfElements = 0;
		auto MeanX = 0.0f;
		auto MeanY = 0.0f;
		auto BodyNum = float(BodyList.size());

		for (unsigned ii=0; ii<BodyList.size();++ii) 
		{
			Position = BodyList[ ii ]->GetPosition();

			if( WorldBox.min.x > Position.x ) WorldBox.min.x = Position.x;
			if( WorldBox.max.x < Position.x ) WorldBox.max.x = Position.x;
			if( WorldBox.min.y > Position.y ) WorldBox.min.y = Position.y;
			if( WorldBox.max.y < Position.y ) WorldBox.max.y = Position.y;
		
			if(BodyList[ ii ]->IsCollidable())
			{
			
				if( Root->NumberOfElements >= Root->ElementsIndexList.size())
				{
					Root->ElementsIndexList.resize(Root->ElementsIndexList.size()+20);
				}
				Root->ElementsIndexList[Root->NumberOfElements] = ii;
				Root->NumberOfElements++; 
			}
		};
		Root->Region = WorldBox;
		ResolveNode( Root, BodyList, ContactList);
	}
};

void QTree2::ResolveNode(unique_ptr<Node>& Node, vector<shared_ptr<cBody>> &BodyList, set<pair<int,int>> &ContactList, int Level )
{
	unsigned Index1, Index2;
	if(Level!=0)
	{
		Rectangle2D	SubRegions[4];
		Split(Node->Region, SubRegions);
		for(auto ii=0; ii < 4; ++ii)
		{
			Node->ChildrenNodes[ii]->Region = SubRegions[ii]; 
			Node->ChildrenNodes[ii]->NumberOfElements = 0;
		};				
		
		for( unsigned jj = 0; jj < Node->NumberOfElements; ++jj) 
		{	
			Index1 = Node->ElementsIndexList[jj];
			for ( int ii = 0 ; ii < 4 ; ii++ ) 
			{			
				if ( BoxesIntersect( SubRegions[ii], BodyList[ Index1 ]->BBox )) 
				{
					if( Node->ChildrenNodes[ii]->NumberOfElements >= Node->ChildrenNodes[ii]->ElementsIndexList.size())
					{
						Node->ChildrenNodes[ii]->ElementsIndexList.resize(Node->ChildrenNodes[ii]->ElementsIndexList.size()+20);
					}
					Node->ChildrenNodes[ii]->ElementsIndexList[ Node->ChildrenNodes[ii]->NumberOfElements++ ] =  Index1;
				}
			}
		}
	}

	if( (Node->NumberOfElements > MaxNodeCapacity) && (Level != 0) )
	{
		for ( int ii = 0 ; ii < 4 ; ii++ ) 
		{
			ResolveNode( Node->ChildrenNodes[ii], BodyList, ContactList, Level-1);
		}
	}
	else
	{
		if(Node->NumberOfElements>1) 
		{
			for(unsigned kk = 0; kk < Node->NumberOfElements-1 ; ++kk) 
			{
				for(unsigned jj = kk+1 ;jj < Node->NumberOfElements; ++jj) 
				{
					Index1 = Node->ElementsIndexList[kk];
					Index2 = Node->ElementsIndexList[jj];
						
					if (BoxesIntersect( BodyList[ Index1 ]->BBox , BodyList[ Index2 ]->BBox )) 
					{
						if( BodyList[ Index1 ]->IsDynamic() || BodyList[ Index2 ]->IsDynamic() ) 
						{
							ContactList.insert( pair<int,int>( Index1 , Index2 ) ); 
						}
					}
				}
			}
		}
	}
};

void QTree2::SetMaxLevel( int Level) { MaxLevel = Level; };
void QTree2::SetNodeCapacity( int NodeCapacity) { MaxNodeCapacity = NodeCapacity; };
unsigned QTree2::MaxNodeCapacity = 20; 
int QTree2::MaxLevel = 2;		
unsigned QTree2::MaxListSize = 100;	



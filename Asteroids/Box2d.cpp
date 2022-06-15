#pragma once
#include "stdafx.h"
#include "Box2d.h"

Box2D::Box2D(float W, float H, float M, BodyType bType,  bool Collision, bool Render) {
			
	p_cBody = shared_ptr<cBody>(new cBody());		
	p_cBody->SetBodyType(bType);

	vector<Vector2> BoxVertices;

	BoxVertices.push_back( Vector2( -W/2.0f,  H/2.0f ));
	BoxVertices.push_back( Vector2(  W/2.0f,  H/2.0f ));
	BoxVertices.push_back( Vector2(  W/2.0f, -H/2.0f ));
	BoxVertices.push_back( Vector2( -W/2.0f, -H/2.0f ));
	
	Polygon2 SomeShape = Polygon2(BoxVertices);
	
	p_cBody->AddPolygon(SomeShape);
	p_cBody->SetDensity(M);

	p_cBody->SetCollisionType(Physical);

	p_cRender = shared_ptr<cRender>(new cRender());
	p_cRender->SetVisible(Render);
		
	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());

	p_cLogics = shared_ptr<cLogics>(new cLogics());
};

Box2D::~Box2D() {
	p_cRender.reset();
	p_cBody.reset();
	p_cConstrained.reset();
	p_cLogics.reset();
};

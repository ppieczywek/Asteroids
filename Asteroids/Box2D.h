#pragma once
#include "Vector2.h"
#include "Entity.h"
#include "cBody.h"
#include "cRender.h"
#include "cConstrained.h"
#include "cLogics.h"
#include <vector>
using namespace std;

class Box2D : public Entity {
public:
	Box2D(float W, float H, float M, BodyType bType,  bool Collision, bool Render);
	~Box2D();
	virtual void render() { };
};
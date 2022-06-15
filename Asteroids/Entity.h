#pragma once
#include <memory>
#include <vector>
using namespace std;
class cBody;
class cMove;
class cRender;
class cConstrained;
class cLogics;

class Entity
{	
public:
	
	long long					ID;
	shared_ptr<cBody>			p_cBody;
	shared_ptr<cRender>			p_cRender;
	shared_ptr<cConstrained>	p_cConstrained;
	shared_ptr<cLogics>			p_cLogics;
	
								Entity() : ID(-1) {};
			void				SetID(const long long&);
	inline	long long			GetID();
	virtual						~Entity();
};

long long	Entity::GetID()
{
	return ID;
};





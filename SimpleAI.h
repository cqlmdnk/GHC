#pragma once
#include "Character.h"
class SimpleAI :
	public Character
{
public:
	virtual void act(int dir) = 0;
	SimpleAI(HDC hDC);
};


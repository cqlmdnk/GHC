#pragma once
#include "Character.h"
class SimpleAI :
	public Character
{
public:
	void act(int dir);
	SimpleAI(HDC hDC);
};


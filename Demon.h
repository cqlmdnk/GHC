#pragma once
#include "SimpleAI.h"
class Demon :
	public SimpleAI
{
public:
	Demon(HDC hDC);
	~Demon();
	void changeState(STATE state);
	void act(int a);
};


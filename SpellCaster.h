#pragma once
#include "SimpleAI.h"
#include "Spell.h"
class SpellCaster :
	public SimpleAI
{
public:
	SpellCaster(HDC hDC);
	void act(int a);
	Spell* fire(POINT target);
	void changeState(STATE state);
	Bitmap* _bSpell;
};


#pragma once
#include "SimpleAI.h"
#include "Spell.h"
class SpellCaster :
	public SimpleAI
{
public:
	SpellCaster(HDC hDC, HINSTANCE _hInstance);
	~SpellCaster();
	void act(int a);
	Spell* fire(POINT target);
	void changeState(STATE state);
	
	bool lastFrame()
	{
		return m_iCurFrame == 7;
	};
	void die();
};


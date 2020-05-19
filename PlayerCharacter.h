#pragma once
#include "Character.h"
#include "FireBurst.h"
class PlayerCharacter :
	public Character{

public:
	PlayerCharacter(HDC hDC, HINSTANCE _hInstance);
	void changeState(STATE state);
	void die();
	FireBurst* fire();
	int fireCounter;
	bool lastFrame()
	{
		return m_iCurFrame == m_iNumFrames - 2;
	};
};


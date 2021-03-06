#pragma once
#include "Character.h"
#include "FireBurst.h"
class PlayerCharacter :
	public Character{

public:
	PlayerCharacter(HDC hDC, HINSTANCE _hInstance);
	void changeState(STATE state);
	SPRITEACTION Update(int** map, int x);
	void die();
	FireBurst* fire(BOOL enemy);
	int fireCounter;
	int magazine = 3;
	int life = 5;
	int losingLifeTime = 0; 
	int prevSpeed = 0;
	Bitmap* GetBitmap() {
		return m_pBitmap;
	}
	bool lastFrame()
	{
		return m_iCurFrame == m_iNumFrames - 2;
	};
};


#pragma once
#include "Sprite.h"
typedef WORD STATE;
const STATE S_IDLE = 0,
			S_RUN = 1,
			S_JUMP = 2;
//diğer durumlar buraya eklenecek

class Character :
	public Sprite
{
	using Sprite::Sprite;
	Bitmap *_bCharAnimIdle, *_bCharAnimRun, *_bCharAnimJump;
	STATE _bCharState;
public:
	void loadChar(HDC hDC);
	void changeState(STATE state);
	bool checkState(STATE state);
};


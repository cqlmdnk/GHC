#pragma once
#include "Sprite.h"
typedef WORD STATE;
const STATE S_IDLE = 0,
S_RUNR = 1,
S_RUNL = 2,
S_LJUMP = 3,
S_RJUMP = 4;
//diğer durumlar buraya eklenecek

class Character :
	public Sprite
{
	using Sprite::Sprite;
	Bitmap *_bCharAnimIdle, *_bCharAnimRunR, * _bCharAnimRunL, *_bCharAnimJump;
	STATE _bCharState;
public:
	void loadChar(HDC hDC);
	void changeState(STATE state);
	bool checkState(STATE state);
};


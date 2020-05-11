#pragma once
#include "Sprite.h"
typedef WORD STATE;
const STATE S_IDLE = 0,
S_RUNR = 1,
S_RUNL = 2,
S_LJUMP = 3,
S_RJUMP = 4,
S_RFIRE = 5,
S_LFIRE = 6;
//diğer durumlar buraya eklenecek

class Character :
	public Sprite
{
	using Sprite::Sprite;
protected:
	Bitmap *_bCharAnimIdle, *_bCharAnimRunR, * _bCharAnimRunL, *_bCharAnimJumpR, * _bCharAnimJumpL, *_bCharFireBurstR, *_bCharFireBurstL, *_bCharFire, * _bCharFireL;;
	STATE _bCharState;
public:
	Character(HDC hDC);
	virtual void changeState(STATE state) = 0;
	bool checkState(STATE state);
};


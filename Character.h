#pragma once
#include "Sprite.h"
typedef WORD STATE;
const STATE S_IDLE = 0,
S_IDLEL = 1,
S_RUNR = 2,
S_RUNL = 3,
S_LJUMP = 4,
S_RJUMP = 5,
S_RFIRE = 6,
S_LFIRE = 7,
S_RDEATH = 8,
S_LDEATH = 9,
S_RATT = 10,
S_LATT = 11;
//diğer durumlar buraya eklenecek

class Character :
	public Sprite
{
	using Sprite::Sprite;
protected:
	Bitmap* _bCharAnimIdle,* _bCharAnimIdleL, * _bCharAnimRunR, * _bCharAnimRunL, * _bCharAnimJumpR, * _bCharAnimJumpL, * _bCharFireBurstR, * _bCharFireBurstL, * _bCharFire, * _bCharFireL, * _bCharAnimDeathR, * _bCharAnimDeathL, * _bCharAnimAttR, * _bCharAnimAttL;
	STATE _bCharState;
public:
	Character(HDC hDC);
	virtual void die() = 0;
	virtual void changeState(STATE state) = 0;
	bool checkState(STATE state);
	int life = 50;
	virtual bool lastFrame() = 0;
};


#pragma once
#include "Sprite.h"
class Spell :
	public Sprite
{
public:

	Spell(Bitmap* _bitmap, POINT target, int x, int y);
	void calcNextPos(int y);
	POINT target;
	POINT startPos;
	int lifeTime = 0;
};


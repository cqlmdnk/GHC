#pragma once
#include "Sprite.h"
class Spell :
	public Sprite
{
public:

	Spell(Bitmap* _bitmap, POINT target);
	void calcNextPos(int x, int y);
	SPRITEACTION Update(int** map, int x);
	POINT target;
	POINT startPos;
	int lifeTime = 0;
};


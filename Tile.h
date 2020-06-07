#pragma once
#include "Sprite.h"
class Tile :
	public Sprite
{
public:
	Tile(HDC hDC, HINSTANCE _hInstance);
	~Tile();
	int type;
	SPRITEACTION Update(int** map, int x);


};


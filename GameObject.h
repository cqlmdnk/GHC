#pragma once
#include"Bitmap.h"
class GameObject
{
public:
	GameObject(Bitmap* obj, int x, int y);
	int getX() { return x; };
	int getY() { return y; };
private:
	Bitmap* obj;
	int x = 0;
	int y = 0;
};


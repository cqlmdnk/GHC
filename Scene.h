#pragma once
#include<windows.h>
#include <vector>
#include "GameObject.h"
#include "Sprite.h"

// Bu class daha yar�m i�ine scenede olmas� gereken her�ey olucak
// D��manlarda dahil.

class Scene
{
public:
	Scene() {};
	void addBackground(Bitmap* img);
	void addGameObject(GameObject* obj);
	void drawBackground(HDC hDc, int x);

private:
	std::vector<Bitmap*> background; // resimlerin tutuldu�u array
	std::vector <GameObject*> objects;// objeler tutulacak
};


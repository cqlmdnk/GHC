#pragma once
#include<windows.h>
#include <vector>
#include "GameObject.h"
#include "Sprite.h"

// Bu class daha yarým içine scenede olmasý gereken herþey olucak
// Düþmanlarda dahil.

class Scene
{
public:
	Scene() {};
	void addBackground(Bitmap* img);
	void addGameObject(GameObject* obj);
	void drawBackground(HDC hDc, int x);

private:
	std::vector<Bitmap*> background; // resimlerin tutulduðu array
	std::vector <GameObject*> objects;// objeler tutulacak
};


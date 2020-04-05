#pragma once
#include<windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "GameObject.h"
#include "Character.h"


// Bu class daha yarým içine scenede olmasý gereken herþey olucak
// Düþmanlarda dahil.

class Scene
{
public:
	int p_iPlatform[6000][30] = { 0 }; // 200 -> x ekseni // 14 -> y ekseni
	Scene(HDC hDC);
	void addBackground(Bitmap* img);
	void addGameObject(GameObject* obj);
	void drawBackground(HDC hDc, int x);
	void addTile(int x, int y, int type);
	void saveLevel(char* levelName);
	void loadLevel(char* levelName);
	Bitmap* tiles[3];

private:
	std::vector<Bitmap*> background; // resimlerin tutulduðu array
	std::vector <GameObject*> objects;// objeler tutulacak
};


#pragma once
#include<windows.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "GameObject.h"
#include "PlayerCharacter.h"
#include "SpellCaster.h"
#include "Demon.h"
#include "Tile.h"


// Bu class daha yarým içine scenede olmasý gereken herþey olucak
// Düþmanlarda dahil.

class Scene
{
public:
	int p_iPlatform[40000][18]; 
	Scene(HDC hDC);
	void addBackground(Bitmap* img);
	void addSpellCaster(SpellCaster* obj);
	void addSpell(Spell* obj);
	void addDemon(Demon* obj);
	void drawScene(HDC hDc);
	void drawBackground(HDC hDc, int x);
	HBITMAP CreateOffscreenBmp(int wd, int hgt, int x);
	void BlitToHdc(HDC hdcDst, HBITMAP hbmSrc, int x, int y, int wd, int hgt);
	bool** getMap(int x);
	void addTile(int xCur, int yCur, int type, int x);
	void saveLevel(char* levelName);
	void loadLevel(char* levelName);
	int testCollisionRight(int x, int y);
	int testCollisionLeft(int x, int y);
	int p = 0;
	Bitmap* tiles[3];
	HBITMAP platform;
	std::vector <SpellCaster*> spCasters;
	std::vector <Spell*> spells;
	std::vector <Demon*> demons;
	std::vector<Sprite*> updateScene(int x, int charYPos, HDC hDC);
private:
	std::vector<Bitmap*> background; // resimlerin tutulduðu array
};


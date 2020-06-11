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
#include <time.h>       

#define SPELL_RATE 10 // 0 -> NONE, UP TO 100
#define FIRE_RATE 10  // 0 -> NONE, UP TO 100


class Scene
{
public:
	int p_iPlatform[40000][18]; 
	Scene(HDC hDC, HINSTANCE  _hInstance);
	void addBackground(Bitmap* img);
	void addSpellCaster(SpellCaster* obj);
	void addSpell(Spell* obj);
	void addSpriteTile(Tile* obj);
	void addDemon(Demon* obj);
	void drawScene(HDC hDc);
	void drawBackground(HDC hDc, int x);
	HBITMAP CreateOffscreenBmp(int wd, int hgt, int x);
	void BlitToHdc(HDC hdcDst, HBITMAP hbmSrc, int x, int y, int wd, int hgt);
	int** getMap(int x);
	void addTile(int xCur, int yCur, int type, int x);
	void saveLevel(char* levelName);
	void loadLevel(char* levelName);
	void createNextScreen(int x);
	void clearTiles();
	int testCollisionRight(int x, int y);
	int testCollisionLeft(int x, int y);
	Spell* spellF(POINT target, POINT pos);
	FireBurst* fireF(POINT target, POINT pos, int dir);
	int p = 0;
	Bitmap* tiles[3];
	Bitmap* _bSpell, * _bCharFireBurstR, * _bCharFireBurstL;
	HBITMAP platform;
	std::vector <SpellCaster*> spCasters;
	std::vector <Spell*> spells;
	std::vector <Demon*> demons;
	std::vector <Tile*> tilesSprites;

	std::vector<Sprite*> updateScene(int x, int charXPos, int charYPos, HDC hDC, HINSTANCE _hInstance);
private:
	std::vector<Bitmap*> background; // resimlerin tutulduðu array
};


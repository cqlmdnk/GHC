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
	//int array yerine uint32_t[6000] oluşturulacak ve bitwise işlem yapılacak
	int p_iPlatform[40000][27]; // 200 -> x ekseni // 14 -> y ekseni
	Scene(HDC hDC);
	void addBackground(Bitmap* img);
	void addGameObject(Sprite* obj);
	void drawScene(HDC hDc);
	void drawBackground(HDC hDc, int x);
	HBITMAP CreateOffscreenBmp(int wd, int hgt, int x);
	void BlitToHdc(HDC hdcDst, HBITMAP hbmSrc, int x, int y, int wd, int hgt);
	bool** getMap(int x);
	void addTile(int xCur, int yCur, int type, int x);
	void saveLevel(char* levelName);
	void loadLevel(char* levelName);
	int p = 0;
	Bitmap* tiles[3];
	HBITMAP platform;
private:
	std::vector<Bitmap*> background; // resimlerin tutulduðu array
	std::vector <Sprite*> objects;// objeler tutulacak
};


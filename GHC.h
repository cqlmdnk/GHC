﻿//-----------------------------------------------------------------
// UFO Application
// C++ Header - UFO.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Resources.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Scene.h"
#include "SimpleAI.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE   _hInstance;
HBITMAP		_hOffscreenBitmap;
HDC	        _hOffscreenDC;
GameEngine* _pGame;
const int   _iMAXSPEED = 8;
Bitmap* _pBackground;
Bitmap *_pLife, *_pGameOver;
PlayerCharacter* _sCharacter;
Scene* _Scene;   // Scene pointer her chapter bir scene den oluşur diye düþündüm ama birden fazla oluşabilir.
std::vector<SimpleAI*> ais;
bool editMod = false; // mod kontrol için tutuluyor
int x = 0;
int vx = 0;
int sceneBlock = 2;
int xThreshold = 32;
int maxSp = 0; 
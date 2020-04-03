//-----------------------------------------------------------------
// UFO Application
// C++ Header - UFO.h
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Scene.h";

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE   _hInstance;
HBITMAP		_hOffscreenBitmap;
HDC	        _hOffscreenDC;
GameEngine* _pGame;
const int   _iMAXSPEED = 8;
Bitmap* _pBackground;
Bitmap* _bCharAnim;
Sprite* _sCharacter;
Scene* _Scene;   // Scene pointer her cheapter bir scene den olu�ur diye d���nd�m ama birden fazla olu�abilir.


int x = 0;

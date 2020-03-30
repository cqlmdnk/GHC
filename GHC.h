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

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE   _hInstance;
HBITMAP		_hOffscreenBitmap;
HDC	        _hOffscreenDC;
GameEngine* _pGame;
const int   _iMAXSPEED = 8;
Bitmap*     _pBackground;
Bitmap*     _bCharAnim;
Sprite*		_sCharacter;

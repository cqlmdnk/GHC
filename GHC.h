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
#include "Sprite.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
HINSTANCE   _hInstance;
GameEngine* _pGame;
const int   _iMAXSPEED = 8;
Bitmap*     _pBackground;

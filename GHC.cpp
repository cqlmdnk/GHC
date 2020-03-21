//-----------------------------------------------------------------
// UFO Application
// C++ Source - UFO.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "GHC.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
  // Create the game engine
  _pGame = new GameEngine(hInstance, TEXT("GHC"),
    TEXT("GHC"), IDI_UFO, IDI_UFO_SM, 1280, 720);
  if (_pGame == NULL)
    return FALSE;
  // Set the frame rate
  _pGame->SetFrameRate(30);

  // Store the instance handle
  _hInstance = hInstance;

  return TRUE;
}

void GameStart(HWND hWindow)
{
  // Create and load the background and saucer bitmaps
  HDC hDC = GetDC(hWindow);
  _pBackground = new Bitmap(hDC, TEXT("BG.bmp")); // will be changed

  // Set the initial saucer position and speed
 
}

void GameEnd()
{
  // Cleanup the background and saucer bitmaps
  delete _pBackground;
  

  // Cleanup the game engine
  delete _pGame;
}

void GameActivate(HWND hWindow)
{
}

void GameDeactivate(HWND hWindow)
{
}

void GamePaint(HDC hDC)
{
  // Draw the background and saucer bitmaps
  _pBackground->Draw(hDC, 0, 0);
  
}

void GameCycle()
{
  // Update the saucer position
  
  // Force a repaint to redraw the saucer
  InvalidateRect(_pGame->GetWindow(), NULL, FALSE);
}

void HandleKeys()
{
  // Change the speed of the saucer in response to arrow key presses
	if (GetAsyncKeyState(VK_LEFT) < 0) {

  }
    
  else if (GetAsyncKeyState(VK_RIGHT) < 0) {

  }

	  if (GetAsyncKeyState(VK_UP) < 0) {

  }

  else if (GetAsyncKeyState(VK_DOWN) < 0) {

  }

  else if (GetAsyncKeyState(VK_SPACE) < 0) {
	  HDC hDC = GetDC(_pGame->GetWindow());
	  
	  

  }
	  
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
  if (bLeft)
  {
   
  }
  else
  {
   
  }
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

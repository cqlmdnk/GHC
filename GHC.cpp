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
    TEXT("GHC"), IDI_UFO, IDI_UFO_SM, 860, 720);
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
  Bitmap** _bCharAnim = new Bitmap*[3];
  *(_bCharAnim) = new Bitmap(hDC, TEXT("char_idle/adventurer-idle-00.bmp"));
  *(_bCharAnim+1) = new Bitmap(hDC, TEXT("char_idle/adventurer-idle-01.bmp"));
  *(_bCharAnim+2) = new Bitmap(hDC, TEXT("char_idle/adventurer-idle-02.bmp"));
  _sCharacter = new Sprite(_bCharAnim);
  _sCharacter->SetPosition(300, 400);
  

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
  _sCharacter->Draw(hDC);
}

void GameCycle()
{
  // Update the saucer position
	_sCharacter->Update();
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

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
        TEXT("GHC"), IDI_UFO, IDI_UFO_SM, 1920, 1080); // Bununla oynarsan scenede de deðiþiklik yapman lazým!!!!
    if (_pGame == NULL)
        return FALSE;
    // Set the frame rate
    _pGame->SetFrameRate(60);

    // Store the instance handle
    _hInstance = hInstance;

    return TRUE;
}

void GameStart(HWND hWindow)
{
    _hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
    _hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
        _pGame->GetWidth(), _pGame->GetHeight());
    SelectObject(_hOffscreenDC, _hOffscreenBitmap);

    // Create and load the background and saucer bitmaps
    HDC hDC = GetDC(hWindow);
    // Arkaplan resimleri sceneye ekleme iþlemleri
    //-------------------------------------------------------------------------------------------------
    _pBackground = new Bitmap(hDC, TEXT("bg/1.bmp")); // will be changed
    _Scene = new Scene();
    _Scene->addBackground(_pBackground);
    _pBackground = new Bitmap(hDC, TEXT("bg/2.bmp"));
    _Scene->addBackground(_pBackground);
    _pBackground = new Bitmap(hDC, TEXT("bg/3.bmp"));
    _Scene->addBackground(_pBackground);
    _pBackground = new Bitmap(hDC, TEXT("bg/4.bmp"));
    _Scene->addBackground(_pBackground);
    _pBackground = new Bitmap(hDC, TEXT("bg/5.bmp"));
    _Scene->addBackground(_pBackground);
    //--------------------------------------------------------------------------------------------------
                                                                                                                  //ÖNERÝ
                                                                              //Actor diye class oluþturalým. Player extend etsin enemyde extend etsin
                                                                              // inharetance yapý iþimizi kolaylaþtýrýr.
    _bCharAnim = new Bitmap(hDC, TEXT("resources/character_idle.bmp"));
    _sCharacter = new Sprite(_bCharAnim);                                     //H.D Kendime not
                                                                             // Sprite scale etmeye bak!!!!
    _sCharacter->SetPosition(200, 800);
    _sCharacter->SetNumFrames(4);
    _sCharacter->SetFrameDelay(5);


    // Set the initial saucer position and speed

}

void GameEnd()
{
    // Cleanup the background and saucer bitmaps
    delete _pBackground;

    DeleteObject(_hOffscreenBitmap);
    DeleteDC(_hOffscreenDC);
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
    _Scene->drawBackground(hDC, x);
    //_pBackground->Draw(hDC, 0, 0);
    _sCharacter->Draw(hDC);
}

void GameCycle()
{

    // Update the saucer position
    _sCharacter->Update();
    // Force a repaint to redraw the saucer

    HWND  hWindow = _pGame->GetWindow();
    HDC   hDC = GetDC(hWindow);

    GamePaint(_hOffscreenDC);

    BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
        _hOffscreenDC, 0, 0, SRCCOPY);

    ReleaseDC(hWindow, hDC);
}

void HandleKeys()
{
    // Change the speed of the saucer in response to arrow key presses
    if (GetAsyncKeyState(VK_LEFT) < 0) {

        if (x < 0)              // sola gidemesin diye böyle yapýldý.
            x += 10;                                                                                // Koordinat sistemi þöyle birþey oldu 
    }                                                                                             //      |+y
                                                                                                  //      |
    else if (GetAsyncKeyState(VK_RIGHT) < 0) {                                                    //      |
        x -= 10;                                                                                  //      |______________________ -x
                                                                                                // x bizim sahnenin neresinde olduðumuzu tutuyor.
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

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
    return 0;
}

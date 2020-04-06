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
    _pBackground = new Bitmap(hDC, TEXT("resources/bg/1.bmp")); // will be changed
    _Scene = new Scene(hDC);
    _Scene->addBackground(_pBackground);
    _pBackground = new Bitmap(hDC, TEXT("resources/bg/2.bmp"));  // scenein içine gömülebilir
    _Scene->addBackground(_pBackground);
    _pBackground = new Bitmap(hDC, TEXT("resources/bg/3.bmp"));
    _Scene->addBackground(_pBackground);
    _pBackground = new Bitmap(hDC, TEXT("resources/bg/4.bmp"));
    _Scene->addBackground(_pBackground);
    _pBackground = new Bitmap(hDC, TEXT("resources/bg/5.bmp"));
    _Scene->addBackground(_pBackground);
    //--------------------------------------------------------------------------------------------------
                                                                                                                  //ÖNERÝ
                                                                              //Actor diye class oluþturalým. Player extend etsin enemyde extend etsin  +1
                                                                              // inharetance yapý iþimizi kolaylaþtýrýr.
 

    _sCharacter = new Character();
	_sCharacter->loadChar(hDC);
																			//H.D Kendime not
                                                                     // Sprite scale etmeye bak!!!!
	


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
    _Scene->drawBackground(hDC, x);

	if (editMod) {
		LPCSTR message = TEXT("Dev Mod Activated");
		RECT rect = { 0,0,0,0 };
		DrawTextA(hDC, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
		//
	}
    //_pBackground->Draw(hDC, 0, 0);
    _sCharacter->Draw(hDC);
}

void GameCycle()
{

    _sCharacter->Update();

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
		if (x > 0)              // sola gidemesin diye böyle yapýldý.
			x -= 1;

		if (_sCharacter->IsAnimDef() && !_sCharacter->checkState(S_RUN))
			_sCharacter->changeState(S_RUN);
       

	}                                                                                             //      |+y
                                                                                                  //      |
    else if (GetAsyncKeyState(VK_RIGHT) < 0) {                                                    //      |
        x += 1;                                                                                  //      |______________________ +x
		if (_sCharacter->IsAnimDef() && !_sCharacter->checkState(S_RUN))
			_sCharacter->changeState(S_RUN);


																								// x bizim sahnenin neresinde olduðumuzu tutuyor.
    }
	else {
		if (_sCharacter->IsAnimDef() &&  !_sCharacter->checkState(S_IDLE)) {
			_sCharacter->changeState(S_IDLE);
		}
	}
	

    if (GetAsyncKeyState(VK_UP) < 0) {
		if(!_sCharacter->checkState(S_JUMP))
			_sCharacter->changeState(S_JUMP);

    }

    else if (GetAsyncKeyState(VK_DOWN) < 0) {

    }

    else if (GetAsyncKeyState(VK_SPACE) < 0) {
        HDC hDC = GetDC(_pGame->GetWindow());



    }
	else if (GetAsyncKeyState(VK_TAB) < 0) { // tab ile edit mod a giriliyor
		editMod = !editMod;
		Sleep(100);



	}
	else if (GetAsyncKeyState('S') < 0) { 
		if (editMod) {
			POINT p = {0,0};
			GetCursorPos(&p);
			_Scene->addTile(x + p.x / 35 ,p.y / 35, 1);
		}
		

		



	}
	else if (GetAsyncKeyState('A') < 0) { 
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile(x + p.x/35, p.y/35, 2);
			
		}



	}
	else if (GetAsyncKeyState('D') < 0) { //delete
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile(x + p.x / 35, p.y / 35, 0);

		}
	

	}
	else if (GetAsyncKeyState('L') < 0) { 
		if (editMod) {
			_Scene->loadLevel("leveld.dat");

		}


	}
	else if (GetAsyncKeyState('T') < 0) { 
		if (editMod) {
			_Scene->saveLevel("leveld.dat");

		}


	}

}

void MouseButtonDown(int x, int y, BOOL bLeft)
{

    if (bLeft )
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

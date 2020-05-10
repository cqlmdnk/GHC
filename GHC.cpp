﻿//-----------------------------------------------------------------
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
 

    _sCharacter = new Character(hDC);
    for (int i = 0; i < 2; i++)
    {
        SimpleAI* temp_ai = new SimpleAI(hDC);
        temp_ai->SetBoundsAction(BA_HALT);
        if (i == 0) {
            temp_ai->SetPosition(200, 1000);
            temp_ai->changeState(S_RUNR);
            temp_ai->SetVelocity(10, 0);
        }
        else{
            temp_ai->SetPosition(700, 1000);
            temp_ai->changeState(S_RUNL);
            temp_ai->SetVelocity(-10, 0);
        }
        ais.push_back(temp_ai);
        _pGame->AddSprite(temp_ai);

    }
    
    //H.D Kendime not
                                                                     // Sprite scale etmeye bak!!!!
    _pGame->AddSprite(_sCharacter);


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
    RECT rect = { 0,0,0,0 };


	if (editMod) {

		LPCSTR message = TEXT("Dev Mod Activated  X ");
		DrawTextA(hDC, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);

		//
	}
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "X cordinate : %d",  x);
    LPCSTR message1 = (buffer);
    rect.left = 0;
    rect.top = 25;
    DrawTextA(hDC, message1,sizeof(buffer), &rect, DT_SINGLELINE | DT_NOCLIP);
   
    POINT p = { 0,0 };
    GetCursorPos(&p);
    snprintf(buffer, sizeof(buffer), "mause X cordinate : %ld ", p.x);
   message1 = (buffer);
    rect.left = 0;
    rect.top = 50;
    DrawTextA(hDC, message1, sizeof(buffer), &rect, DT_SINGLELINE | DT_NOCLIP);
    snprintf(buffer, sizeof(buffer), "mause Y cordinate : %ld ", p.y);
    message1 = (buffer);
    rect.left = 0;
    rect.top = 75;
    DrawTextA(hDC, message1, sizeof(buffer), &rect, DT_SINGLELINE | DT_NOCLIP);

    snprintf(buffer, sizeof(buffer), "platform x : %d                      ", x /40+ p.x / 40);
    message1 = (buffer);
    rect.left = 0;
    rect.top = 100;
    DrawTextA(hDC, message1, sizeof(buffer), &rect, DT_SINGLELINE | DT_NOCLIP);


    snprintf(buffer, sizeof(buffer), "collison : %d", _Scene->testCollisionRight(x, (_sCharacter->GetPosition().top + _sCharacter->GetPosition().bottom) / 2));
    message1 = (buffer);
    rect.left = 0;
    rect.top = 125;
    DrawTextA(hDC, message1, sizeof(buffer), &rect, DT_SINGLELINE | DT_NOCLIP);



    //_pBackground->Draw(hDC, 0, 0);
    _pGame->DrawSprites(hDC);
    
}

void GameCycle()
{
    _pGame->UpdateSprites(_Scene->getMap(x), x, vx);
    for (int i = 0; i < 2; i++) // wil be removed
    {
        if (i == 0) {
            ais.at(i)->SetVelocity(5, 0);
        }
        else {
           
            ais.at(i)->SetVelocity(-10, 0);
        }

    }
    HWND  hWindow = _pGame->GetWindow();
    HDC   hDC = GetDC(hWindow);

    GamePaint(_hOffscreenDC);

    BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),//???
        _hOffscreenDC, 0, 0, SRCCOPY);
	
    ReleaseDC(hWindow, hDC);
}

void HandleKeys()
{
    //yatay collision burada hesaplanacak ona göre x te değişim yapılacak
   
    if (GetAsyncKeyState(VK_LEFT) < 0 && (!_sCharacter->checkState(S_RJUMP) || _sCharacter->checkState(S_RJUMP))) {
        if (_Scene->testCollisionLeft(x+180, (_sCharacter->GetPosition().top + _sCharacter->GetPosition().bottom) / 2) == 2) {
            vx = 0;

        }
        else {
            vx--;
        }
      
        vx = max(vx, -10);
        if (_sCharacter->IsAnimDef() && !_sCharacter->checkState(S_RUNL)) {
            _sCharacter->changeState(S_RUNL);
        }
        

	}                                                                                             
                                                                                                  
    else if (GetAsyncKeyState(VK_RIGHT) < 0 && (!_sCharacter->checkState(S_RJUMP) || _sCharacter->checkState(S_RJUMP))) {                                                    
        
        // x eklemeyi buradan çıkar
        // sadece hızı artırıp azaltmayı burada bırak
        //zıplamıyor iken sağa ve sola gidişler hızı artırsın (max a kadar)
        if (_Scene->testCollisionRight(x+120, (_sCharacter->GetPosition().top + _sCharacter->GetPosition().bottom) / 2) == 1) {
            vx = 0;

        }
        else {
            vx++;
        }
      
        vx = min(vx, 10);
        if (_sCharacter->IsAnimDef() && !_sCharacter->checkState(S_RUNR)) {
            _sCharacter->changeState(S_RUNR);
        }
    }
	else {
		if (_sCharacter->IsAnimDef()) {
			_sCharacter->changeState(S_IDLE);
            int sign = (vx > 0) - (vx < 0);
            vx = sign/*getting sign(- or +)*/ * (abs(vx) -3); //
            if (sign != ((vx > 0) - (vx < 0)))
                vx = 0;
		}
        
	}
	
    
    if (GetAsyncKeyState(VK_UP) < 0) {
        if (!_sCharacter->checkState(S_LJUMP) && !_sCharacter->checkState(S_RJUMP)) { // düşüyorsa yürüme ve koşma çalışmamalı
            if (GetAsyncKeyState(VK_LEFT) < 0) {
                _sCharacter->changeState(S_LJUMP);
            }
            else {
                _sCharacter->changeState(S_RJUMP);
            }
           

        }
			

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
			_Scene->addTile(x/40 + p.x / 40 ,p.y / 40, 1, x);
		}
		

		



	}
	else if (GetAsyncKeyState('A') < 0) { 
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile(x  + p.x/40, p.y/40, 2, x);
			
		}



	}
	else if (GetAsyncKeyState('D') < 0) { //delete
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile(x/40 + p.x / 40, p.y / 40, 0, x);

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
    else if (GetAsyncKeyState('Q') < 0) {
        if (editMod) {
           
        }


    }
    x += vx; // sınır kontrolleri eklenmeli ( p_iPlatform[x][])
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
    return TRUE;
}

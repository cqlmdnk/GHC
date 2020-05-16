//-----------------------------------------------------------------
// UFO Application
// C++ Source - UFO.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "GHC.h"


//-----GAME SPECIFIC FUNCTIONS-----//
void randomCastSpells();
void updateSpells();
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
	_pGame->SetFrameRate(30);

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


	_sCharacter = new PlayerCharacter(hDC);

	SpellCaster* temp_ai = new SpellCaster(hDC);
	_Scene->addSpellCaster(temp_ai);


	temp_ai->SetPosition(900, 700);


	temp_ai->SetBoundsAction(BA_HALT);
	ais.push_back(temp_ai);
	_pGame->AddSprite(temp_ai);


	_sCharacter->SetBoundsAction(BA_STOP);

	_pGame->AddSprite(_sCharacter);


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
	//  




	RECT rect = { 0,0,0,0 };

	if (editMod) {




		POINT p = { 0,0 };
		LPCSTR message = TEXT("Dev Mod Activated");
		DrawTextA(hDC, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);

		char buffer[30];
		snprintf(buffer, sizeof(buffer), "Character's X coordinate : %d", x);
		LPCSTR message1 = (buffer);
		rect.top = 25;
		DrawTextA(hDC, message1, -1, &rect, DT_SINGLELINE | DT_NOCLIP);


		GetCursorPos(&p);
		snprintf(buffer, sizeof(buffer), "Mouse X coordinate : %ld ", p.x);
		message1 = (buffer);
		rect.top = 50;
		DrawTextA(hDC, message1, -1, &rect, DT_SINGLELINE | DT_NOCLIP);


		snprintf(buffer, sizeof(buffer), "Mouse Y coordinate : %ld ", p.y);
		message1 = (buffer);
		rect.top = 75;
		DrawTextA(hDC, message1, -1, &rect, DT_SINGLELINE | DT_NOCLIP);


		snprintf(buffer, sizeof(buffer), "Platform on mouse X  : %d  ", x / PLATFORM_S + p.x / PLATFORM_S);
		message1 = (buffer);
		rect.top = 100;
		DrawTextA(hDC, message1, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
	}
	//_pBackground->Draw(hDC, 0, 0);
	_pGame->DrawSprites(hDC);
	RECT lifeBar = {349,_sCharacter->GetPosition().top-11,401, _sCharacter->GetPosition().top-4 };
	RECT lifeChar = { 350,_sCharacter->GetPosition().top - 10,350+_sCharacter->life, _sCharacter->GetPosition().top - 5 };
	HBRUSH hRed = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hGreen = CreateSolidBrush(RGB(0, 255, 0));

	FillRect(hDC, &lifeBar, hRed);
	FillRect(hDC, &lifeChar, hGreen);

}

void GameCycle()
{
	if (_sCharacter->fireCounter != 0) {
		_sCharacter->fireCounter--;
	}
	updateSpells();
	x += vx;
	_pGame->UpdateSprites(_Scene->getMap(x), x, vx);

	HWND  hWindow = _pGame->GetWindow();
	HDC   hDC = GetDC(hWindow);


	std::vector<Sprite*> addedSprites = _Scene->updateScene(x, _sCharacter->GetPosition().top, hDC);
	for (auto nSprite : addedSprites) {
		_pGame->AddSprite(nSprite);
	}

	if (rand() % 100 < 1) {
		if (rand() % 2 == 0) {

			SpellCaster* temp_spellC = new SpellCaster(hDC);
			temp_spellC->SetPosition((rand() % 700) + 1000, 900);
			_Scene->addSpellCaster(temp_spellC);
			_pGame->AddSprite(temp_spellC);
		}
		else {
			Demon* temp_demon = new Demon(hDC);
			temp_demon->SetPosition((rand() % 700) + 1000, 900);
			temp_demon->changeState(S_RUNL);
			_pGame->AddSprite(temp_demon);
			_Scene->addDemon(temp_demon);

		}

	}

	GamePaint(_hOffscreenDC);

	BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),//???
		_hOffscreenDC, 0, 0, SRCCOPY);

	ReleaseDC(hWindow, hDC);
}

void HandleKeys()
{

	if (GetAsyncKeyState(VK_LEFT) < 0 && (!_sCharacter->checkState(S_RJUMP) || _sCharacter->checkState(S_RJUMP))) {
		if (vx + x > 0) {
			if (_Scene->testCollisionLeft(x + 340, (_sCharacter->GetPosition().top + _sCharacter->GetPosition().bottom) / 2) == 2) {

				vx = 0;

			}
			else {
				vx -= 2;
			}

			vx = max(vx, -10);
			if (_sCharacter->IsAnimDef() && !_sCharacter->checkState(S_RUNL)) {
				_sCharacter->changeState(S_RUNL);
			}
		}
		else {
			vx = 0;
		}



	}

	else if (GetAsyncKeyState(VK_RIGHT) < 0 && (!_sCharacter->checkState(S_RJUMP) || _sCharacter->checkState(S_RJUMP))) {

		if (_Scene->testCollisionRight(x + 340, (_sCharacter->GetPosition().top + _sCharacter->GetPosition().bottom) / 2) == 1) {


			vx = 0;

		}
		else {
			vx += 2;
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
			vx = sign/*getting sign(- or +)*/ * (abs(vx) - 3); // 3 yavaşlaması için verilen ters yönlü ivme
			if (sign != ((vx > 0) - (vx < 0)))
				vx = 0;
		}

	}


	if (GetAsyncKeyState(VK_UP) < 0) {
		if (!_sCharacter->checkState(S_LJUMP) && !_sCharacter->checkState(S_RJUMP)) { // düşüyorsa yürüme ve koşma çalışmamalı mı ? :? //
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

	else if (GetAsyncKeyState(VK_SPACE) < 0 && _sCharacter->fireCounter == 0) {
		if (GetAsyncKeyState(VK_LEFT) < 0) {
			_sCharacter->changeState(S_LFIRE);
			_pGame->AddSprite(_sCharacter->fire());
		}
		else {
			_sCharacter->changeState(S_RFIRE);
			_pGame->AddSprite(_sCharacter->fire());

		}
		_sCharacter->fireCounter = 10;

	}
	else if (GetAsyncKeyState(VK_CONTROL)) {
		if (GetAsyncKeyState(VK_LEFT) < 0) {
			_sCharacter->changeState(S_LATT);
		}
		else {
			_sCharacter->changeState(S_RATT);

		}
	}
	else if (GetAsyncKeyState(VK_TAB) < 0) { // tab ile edit mod a giriliyor
		editMod = !editMod;
		Sleep(100);



	}
	else if (GetAsyncKeyState('S') < 0) {
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x) / PLATFORM_S, p.y / PLATFORM_S, 1, x);
		}






	}
	else if (GetAsyncKeyState('A') < 0) {
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x) / PLATFORM_S, p.y / PLATFORM_S, 2, x);

		}



	}
	else if (GetAsyncKeyState('D') < 0) { //delete
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x) / PLATFORM_S, p.y / PLATFORM_S, 0, x);

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

	// sınır kontrolleri eklenmeli ( p_iPlatform[x][])
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

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee) // All collision actions handled here
{
	if (instanceof<Spell>(pSpriteHitter) || instanceof<FireBurst>(pSpriteHitter)) { // spell veya fireburst işlemleri

		if (instanceof<Spell>(pSpriteHitter) && instanceof<PlayerCharacter>(pSpriteHittee)) { // spell mi vurdu

			_Scene->spells.erase(std::remove(_Scene->spells.begin(), _Scene->spells.end(), pSpriteHitter), _Scene->spells.end());
			pSpriteHitter->SetHidden(TRUE);
			Character* pChar = dynamic_cast<Character*>(pSpriteHittee);
			pChar->life -= 5;

			//vurulan playerın canını düşür

		}
		else if ((instanceof<Demon>(pSpriteHittee) && instanceof<FireBurst>(pSpriteHitter))) { // fireburst demona vurduğunda hasar ver (ya da öldür)
			//initiate death animation of spellcaster
			Demon* demon = dynamic_cast<Demon*>(pSpriteHittee);
			demon->die();
			_Scene->demons.erase(std::remove(_Scene->demons.begin(), _Scene->demons.end(), pSpriteHittee), _Scene->demons.end());
			pSpriteHitter->SetHidden(TRUE);




		}
		else if ((instanceof<SpellCaster>(pSpriteHittee) && instanceof<FireBurst>(pSpriteHitter))) {// fireburst spellcaster vurduğunda hasar ver (ya da öldür)
			//initiate death animation of spellcaster
			SpellCaster* spellCaster = dynamic_cast<SpellCaster*>(pSpriteHittee);
			spellCaster->die();

			pSpriteHitter->SetHidden(TRUE);

			_Scene->spCasters.erase(std::remove(_Scene->spCasters.begin(), _Scene->spCasters.end(), pSpriteHittee), _Scene->spCasters.end());


		}
		else if (instanceof<FireBurst>(pSpriteHitter)) { //düşman harici bir yere çarpan fireburst silinir
			pSpriteHitter->SetHidden(TRUE);

		}

		return FALSE;
	}
	else if (instanceof<PlayerCharacter>(pSpriteHittee)) {
		if (instanceof<Demon>(pSpriteHitter) || instanceof<SpellCaster>(pSpriteHitter)) {
			PlayerCharacter* pChar = dynamic_cast<PlayerCharacter*>(pSpriteHittee);
			if ((pChar->checkState(S_RATT) || pChar->checkState(S_LATT)) && pChar->lastFrame()) {//player saldırıyorsa düşamnı öldür
				Character* dyingChar = dynamic_cast<Character*>(pSpriteHitter);
				dyingChar->die(); // öldürme animasyonu ve hidden olma
				if (instanceof<SpellCaster>(pSpriteHitter)) // scene vektöründen silme
					_Scene->spCasters.erase(std::remove(_Scene->spCasters.begin(), _Scene->spCasters.end(), pSpriteHitter), _Scene->spCasters.end());
				else
					_Scene->demons.erase(std::remove(_Scene->demons.begin(), _Scene->demons.end(), pSpriteHitter), _Scene->demons.end());

			}

		}
		else if (instanceof<Tile>(pSpriteHitter)) {
			if (pSpriteHitter->GetPosition().top < pSpriteHittee->GetPosition().top) {// blok üstten mi alttan mı geliyor
				PlayerCharacter* pChar = dynamic_cast<PlayerCharacter*>(pSpriteHittee);
				pChar->life -= 2;
				pSpriteHitter->SetHidden(TRUE);
			}
			else {
				//blok ile beraber üste gitme kodu
				pSpriteHittee->SetVelocity(pSpriteHittee->GetVelocity().x, pSpriteHittee->GetVelocity().y - 10);
				pSpriteHittee->SetPosition(pSpriteHittee->GetPosition().left, pSpriteHittee->GetPosition().top + pSpriteHitter->GetVelocity().y);

			}
			
		}
	}
	else if (instanceof<PlayerCharacter>(pSpriteHitter)) {
		if (instanceof<Demon>(pSpriteHittee) || instanceof<SpellCaster>(pSpriteHittee)) {
			PlayerCharacter* pChar = dynamic_cast<PlayerCharacter*>(pSpriteHitter);
			if ((pChar->checkState(S_RATT) || pChar->checkState(S_LATT)) && pChar->lastFrame()) { //player saldırıyorsa düşamnı öldür
				Character* dyingChar = dynamic_cast<Character*>(pSpriteHittee);
				dyingChar->die();
				if (instanceof<SpellCaster>(pSpriteHittee))
					_Scene->spCasters.erase(std::remove(_Scene->spCasters.begin(), _Scene->spCasters.end(), pSpriteHittee), _Scene->spCasters.end());
				else
					_Scene->demons.erase(std::remove(_Scene->demons.begin(), _Scene->demons.end(), pSpriteHittee), _Scene->demons.end());

			}
		}
		else if (instanceof<Tile>(pSpriteHittee)) {
			if (pSpriteHitter->GetPosition().top > pSpriteHittee->GetPosition().top) { // blok üstten mi alttan mı geliyor

				PlayerCharacter* pChar = dynamic_cast<PlayerCharacter*>(pSpriteHitter);
				pChar->life -= 2;
				pSpriteHittee->SetHidden(TRUE);
			}
			else {
				//blok ile beraber üste gitme kodu
				pSpriteHitter->SetVelocity(pSpriteHitter->GetVelocity().x, pSpriteHitter->GetVelocity().y-10);
				pSpriteHitter->SetPosition(pSpriteHitter->GetPosition().left, pSpriteHitter->GetPosition().top+ pSpriteHittee->GetVelocity().y);

			}
		}
	}
	else {
		if (pSpriteHittee->GetPosition().bottom == pSpriteHitter->GetPosition().bottom) { // character herhangi bir sprite ile çarpışırsa x'i sabit tut // aka scrolling blocked
			if (instanceof<PlayerCharacter>(pSpriteHittee)) {
				if ((pSpriteHitter->GetPosition().left + pSpriteHitter->GetPosition().right) / 2 < 120) {
					if (vx < 0) {
						vx = 0;
					}

				}
				else {
					if (vx > 0) {
						vx = 0;
					}
				}
			}
			if (instanceof<PlayerCharacter>(pSpriteHitter)) {

				if ((pSpriteHittee->GetPosition().left + pSpriteHittee->GetPosition().right) / 2 < 120) {
					if (vx < 0) {
						vx = 0;
					}


				}
				else {
					if (vx > 0) {
						vx = 0;
					}

				}
			}
		}
	}
	return TRUE;

}


void updateSpells() {
	for (Spell* spell : _Scene->spells) {
		int y = (spell->GetPosition().left - 340 == 10 || spell->GetPosition().left - 340 == -10) ? _sCharacter->GetPosition().top : _sCharacter->GetPosition().top + rand() % 10 - 5 + (_sCharacter->GetHeight() / 2);
		spell->calcNextPos(y);
		spell->lifeTime--;
		if (spell->lifeTime < 0) {
			_Scene->spells.erase(std::remove(_Scene->spells.begin(), _Scene->spells.end(), spell), _Scene->spells.end());
			spell->SetHidden(TRUE);
		}
	}
}
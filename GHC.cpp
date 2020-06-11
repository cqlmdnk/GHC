//-----------------------------------------------------------------
// UFO Application
// C++ Source - UFO.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------

#include "GHC.h"


#define SPELL_RATE 20 // 0 -> NONE, UP TO 100
#define FIRE_RATE 10  // 0 -> NONE, UP TO 100

//-----GAME SPECIFIC FUNCTIONS-----//
void drawHUD(HDC hDC);
void drawEditMode(HDC hDC);
void createScreenElements(int** map, HDC hDC);
void addParallaxBgs(HDC hDC);
void UpdateGame();
//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
	// Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("GHC"),
		TEXT("GHC"), NULL, NULL, 1920, 1080); // Bununla oynarsan scenede de deðiþiklik yapman lazým!!!!
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

	_Scene = new Scene(hDC, _hInstance);
	addParallaxBgs(hDC);

	_sCharacter = new PlayerCharacter(hDC, _hInstance);
	_Scene->createNextScreen(-1920);
	_Scene->createNextScreen(0);
	_Scene->addTile(_sCharacter->GetPosition().left + 50, _sCharacter->GetPosition().top + 200, 1, 0);
	

	_pGame->AddSprite(_sCharacter);
	
	
	createScreenElements(_Scene->getMap(x), hDC);

	_pGame->PlayMIDISong(TEXT("resources\\sounds\\background.mid"), TRUE);

}

void GameEnd()
{
	// Cleanup the background and saucer bitmaps

	DeleteObject(_hOffscreenBitmap);
	DeleteDC(_hOffscreenDC);
	// Cleanup the game engine
	delete _pBackground;
	delete _pLife;
	delete _pGameOver;
	delete _pFireIcon;
	delete char_null;
	delete _sCharacter;
	delete _Scene;
	delete _pGame;
	
	ais.clear();


}

void GameActivate(HWND hWindow)
{
	_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
	_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
	_Scene->drawBackground(hDC, x);


	drawEditMode(hDC);
	//_pBackground->Draw(hDC, 0, 0);
	_pGame->DrawSprites(hDC);
	
	drawHUD(hDC);

}

void GameCycle()
{
	if (sceneBlock < x / 1920) {
		sceneBlock = x / 1920;
		_Scene->createNextScreen(x);
	}
	Bitmap* char_bitmap = _sCharacter->GetBitmap();
	if (_sCharacter->fireCounter != 0) {
		_sCharacter->fireCounter--;
	}
	if (_sCharacter->magazine < 3) {
		fireReloadDelay--;
		if (fireReloadDelay == 0) {
			fireReloadDelay = 120;
			_sCharacter->magazine++;
		}
	}
	if (_sCharacter->losingLifeTime > 0) {
		if (_sCharacter->losingLifeTime % 10 == 0) {

			_sCharacter->SetBitmap(char_null);
		}
		_sCharacter->losingLifeTime--;

	}
	if (_sCharacter->prevSpeed > 70 && (_sCharacter->GetVelocity().y == 0 || _sCharacter->GetVelocity().y == 10)) {
		if (_sCharacter->losingLifeTime == 0) {
			_sCharacter->life--;
			_sCharacter->losingLifeTime = 60;
			PlaySound((LPCSTR)IDR_HIT_PLAYER, _hInstance, SND_ASYNC | SND_RESOURCE);
		}

	}
	_sCharacter->prevSpeed = _sCharacter->GetVelocity().y;
	
	HWND  hWindow = _pGame->GetWindow();
	HDC   hDC = GetDC(hWindow);
	SetBkMode(hDC, TRANSPARENT);

	std::vector<Sprite*> addedSprites = _Scene->updateScene(x, _sCharacter->GetPosition().left, _sCharacter->GetPosition().top, hDC, _hInstance); // GDI Leak
	for (auto nSprite : addedSprites) {
		_pGame->AddSprite(nSprite);
	}

	UpdateGame();

	GamePaint(_hOffscreenDC);

	BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),//???
		_hOffscreenDC, 0, 0, SRCCOPY);
	if (!_sCharacter->life > 0) {
		_pGame->PauseMIDISong();
		while (GetAsyncKeyState(VK_RETURN) == 0)
		{
			_pGameOver->Draw(hDC, 0, 0, TRUE);
		}
		_sCharacter->life = 5;
		_pGame->PlayMIDISong(TEXT("resources\\sounds\\background.mid"));

	}
	ReleaseDC(hWindow, hDC);
	_sCharacter->SetBitmap(char_bitmap);


}

void HandleKeys()
{

	if (GetAsyncKeyState(VK_LEFT) < 0 ) {

		if (vx + x > 0) {
			if (_Scene->testCollisionLeft(_sCharacter->GetPosition().left + 20 + x, _sCharacter->GetPosition().bottom - 20) == 2) {

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

	else if (GetAsyncKeyState(VK_RIGHT) < 0 ) {

		if (_Scene->testCollisionRight(_sCharacter->GetPosition().right - 20 + x, _sCharacter->GetPosition().bottom - 20) == 1) {


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
		if (!_sCharacter->checkState(S_LJUMP) && !_sCharacter->checkState(S_RJUMP)) {
			if (GetAsyncKeyState(VK_LEFT) < 0) {
				_sCharacter->changeState(S_LJUMP);
				vx = -5;
			}
			else {
				if (GetAsyncKeyState(VK_RIGHT)) {
					vx += 5;
				}
				_sCharacter->changeState(S_RJUMP);
			}
			PlaySound((LPCSTR)IDR_JUMP_SOUND, _hInstance, SND_ASYNC | SND_RESOURCE);

		}

	}

	else if (GetAsyncKeyState(VK_DOWN) < 0) {

	}

	else if (GetAsyncKeyState(VK_SPACE) < 0 && _sCharacter->fireCounter == 0 && _sCharacter->magazine > 0) {
		if (GetAsyncKeyState(VK_LEFT) < 0) {
			_sCharacter->changeState(S_LFIRE);

			_pGame->AddSprite(_sCharacter->fire(FALSE));
		}
		else {
			_sCharacter->changeState(S_RFIRE);
			_pGame->AddSprite(_sCharacter->fire(FALSE));

		}
		_sCharacter->fireCounter = 10;
		_sCharacter->magazine--;
		PlaySound((LPCSTR)IDR_FIRE_PLAYER, _hInstance, SND_ASYNC | SND_RESOURCE);




	}
	else if (GetAsyncKeyState(VK_CONTROL)) {
		if (GetAsyncKeyState(VK_LEFT) < 0) {
			_sCharacter->changeState(S_LATT);
		}
		else {
			_sCharacter->changeState(S_RATT);

		}
		PlaySound((LPCSTR)IDR_ATTACK_PLAYER, _hInstance, SND_ASYNC | SND_RESOURCE);

	}
	else if (GetAsyncKeyState(VK_TAB) < 0) { // tab ile edit mod a giriliyor
		editMod = !editMod;
		Sleep(100);



	}
	else if (GetAsyncKeyState('S') < 0) { // spellcaster (s)
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x) / PLATFORM_S, p.y / PLATFORM_S, 5, x);
			HWND  hWindow = _pGame->GetWindow();
			HDC   hDC = GetDC(hWindow);
			SpellCaster* sp = new SpellCaster(hDC, _hInstance);

			sp->SetBoundsAction(BA_HALT);
			sp->SetPosition(p.x, p.y);
			_pGame->AddSprite(sp);
			_Scene->addSpellCaster(sp); // may not be necessary
			DeleteObject(hWindow);
			DeleteObject(hDC);
		}


	}

	else if (GetAsyncKeyState('D') < 0) { // demon (d)
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x) / PLATFORM_S, p.y / PLATFORM_S, 6, x);
			HWND  hWindow = _pGame->GetWindow();
			HDC   hDC = GetDC(hWindow);
			Demon* demon = new Demon(hDC, _hInstance);

			demon->SetBoundsAction(BA_BOUNCE);
			demon->SetPosition(p.x, p.y);
			_pGame->AddSprite(demon);
			_Scene->addDemon(demon); // may not be necessary
			DeleteObject(hWindow);
			DeleteObject(hDC);

		}


	}
	else if (GetAsyncKeyState(VK_DELETE) < 0) { // delete 
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x), p.y, 0, x);

		}



	}
	else if (GetAsyncKeyState('1') < 0) { // tile1
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x), p.y, 1, x);

		}



	}
	else if (GetAsyncKeyState('2') < 0) { // tile2 (if its needed)
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x), p.y, 2, x);

		}



	}
	else if (GetAsyncKeyState('V') < 0) { // vertical dynamic tile
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x) / PLATFORM_S, p.y / PLATFORM_S, 3, x);
			HWND  hWindow = _pGame->GetWindow();
			HDC   hDC = GetDC(hWindow);
			Tile* tile = new Tile(hDC, _hInstance);
			tile->type = 0;
			tile->SetVelocity(0, -10);
			tile->SetBoundsAction(BA_BOUNCE);
			tile->SetPosition(p.x, p.y);
			_pGame->AddSprite(tile);
			_Scene->addSpriteTile(tile); // may not be necessary
			DeleteObject(hWindow);
			DeleteObject(hDC);
		}

	}
	else if (GetAsyncKeyState('H') < 0) { // horizontal dynamic tile
		if (editMod) {
			POINT p = { 0,0 };
			GetCursorPos(&p);
			_Scene->addTile((x + p.x) / PLATFORM_S, p.y / PLATFORM_S, 4, x);
			HWND  hWindow = _pGame->GetWindow();
			HDC   hDC = GetDC(hWindow);
			Tile* tile = new Tile(hDC, _hInstance);
			tile->type = 1;
			tile->SetVelocity(-10, 0); // bouncing will be handled through sprite update platform collision
			tile->SetBoundsAction(BA_BOUNCE);
			tile->SetPosition(p.x, p.y);
			_pGame->AddSprite(tile);
			_Scene->addSpriteTile(tile); // may not be necessary
			DeleteObject(hWindow);
			DeleteObject(hDC);
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

}

void MouseButtonDown(int x, int y, BOOL bLeft)
{

	if (bLeft)
	{
		//_sCharacter->SetPosition(x, y);
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



			if (_sCharacter->losingLifeTime == 0) {
				pSpriteHitter->SetHidden(TRUE);
				_sCharacter->life--;
				_sCharacter->losingLifeTime = 60;
				PlaySound((LPCSTR)IDR_HIT_PLAYER, _hInstance, SND_ASYNC | SND_RESOURCE);
			}
			//vurulan playerın canını düşür

		}
		else if (instanceof<FireBurst>(pSpriteHitter) && instanceof<PlayerCharacter>(pSpriteHittee)) {
			FireBurst* fireBurst = dynamic_cast<FireBurst*>(pSpriteHitter);
			if (fireBurst->enemy) {
				if (_sCharacter->losingLifeTime == 0) {
					pSpriteHitter->SetHidden(TRUE);
					_sCharacter->life--;
					_sCharacter->losingLifeTime = 60;
					PlaySound((LPCSTR)IDR_HIT_PLAYER, _hInstance, SND_ASYNC | SND_RESOURCE);
				}

			}
		}
		else if ((instanceof<Demon>(pSpriteHittee) && instanceof<FireBurst>(pSpriteHitter))) { // fireburst demona vurduğunda hasar ver (ya da öldür)
			//initiate death animation of spellcaster
			FireBurst* fireBurst = dynamic_cast<FireBurst*>(pSpriteHitter);
			if (!fireBurst->enemy) {
				Demon* demon = dynamic_cast<Demon*>(pSpriteHittee);
				demon->die();
				score++;
				_Scene->demons.erase(std::remove(_Scene->demons.begin(), _Scene->demons.end(), pSpriteHittee), _Scene->demons.end());
				pSpriteHitter->SetHidden(TRUE);
			}
			




		}
		else if ((instanceof<SpellCaster>(pSpriteHittee) && instanceof<FireBurst>(pSpriteHitter))) {// fireburst spellcaster vurduğunda hasar ver (ya da öldür)
			//initiate death animation of spellcaster
			FireBurst* fireBurst = dynamic_cast<FireBurst*>(pSpriteHitter);
			if (!fireBurst->enemy) {
				SpellCaster* spellCaster = dynamic_cast<SpellCaster*>(pSpriteHittee);
				spellCaster->die();
				score++;
				pSpriteHitter->SetHidden(TRUE);

				_Scene->spCasters.erase(std::remove(_Scene->spCasters.begin(), _Scene->spCasters.end(), pSpriteHittee), _Scene->spCasters.end());
			}

		}
		
		else if (instanceof<FireBurst>(pSpriteHitter)) { //düşman harici bir yere çarpan fireburst silinir
			FireBurst* fireBurst = dynamic_cast<FireBurst*>(pSpriteHitter);
			if (!fireBurst->enemy) {
				pSpriteHitter->SetHidden(TRUE);
			}
		}

		return FALSE;
	}
	else if (instanceof<PlayerCharacter>(pSpriteHittee)) {
		if (instanceof<Demon>(pSpriteHitter) || instanceof<SpellCaster>(pSpriteHitter)) {
			PlayerCharacter* pChar = dynamic_cast<PlayerCharacter*>(pSpriteHittee);
			if ((pChar->checkState(S_RATT) || pChar->checkState(S_LATT)) && pChar->lastFrame()) {//player saldırıyorsa düşamnı öldür
				Character* dyingChar = dynamic_cast<Character*>(pSpriteHitter);
				dyingChar->die(); // öldürme animasyonu ve hidden olma
				score++;
				if (instanceof<SpellCaster>(pSpriteHitter)) // scene vektöründen silme
					_Scene->spCasters.erase(std::remove(_Scene->spCasters.begin(), _Scene->spCasters.end(), pSpriteHitter), _Scene->spCasters.end());
				else
					_Scene->demons.erase(std::remove(_Scene->demons.begin(), _Scene->demons.end(), pSpriteHitter), _Scene->demons.end());

			}
			if (instanceof<Demon>(pSpriteHitter)) {// ölü olup olmadığını kontrol et
				Demon* demon = dynamic_cast<Demon*>(pSpriteHitter);
				STATE att_state = pChar->GetPosition().left > demon->GetPosition().left ? S_RATT : S_LATT;
				if (demon->IsAnimDef()) {
					demon->changeState(att_state);
				}
				else {
					if (demon->lastFrame()) {
						if (_sCharacter->losingLifeTime == 0) {
							_sCharacter->life--;
							_sCharacter->losingLifeTime = 60;
							PlaySound((LPCSTR)IDR_HIT_PLAYER, _hInstance, SND_ASYNC | SND_RESOURCE);
						}
					}
				}

			}
			
			return FALSE;
		}
		
		else if (instanceof<Tile>(pSpriteHitter)) {

			//blok ile beraber hareket kodu
			if (pSpriteHitter->GetPosition().top + 5 > pSpriteHittee->GetPosition().top) {

				pSpriteHittee->SetVelocity(pSpriteHittee->GetVelocity().x, pSpriteHittee->GetVelocity().y - 10);

				pSpriteHittee->SetPosition(pSpriteHittee->GetPosition().left + pSpriteHitter->GetVelocity().x, pSpriteHitter->GetPosition().top - pSpriteHittee->GetHeight() - 1);


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
			
			return FALSE;
		}
		else if (instanceof<Tile>(pSpriteHittee)) {


			if (pSpriteHittee->GetPosition().top + 5 > pSpriteHitter->GetPosition().top) {

				//blok ile beraber hareket kodu
				pSpriteHitter->SetVelocity(pSpriteHitter->GetVelocity().x, pSpriteHitter->GetVelocity().y - 10);




				pSpriteHitter->SetPosition(pSpriteHitter->GetPosition().left + pSpriteHittee->GetVelocity().x, pSpriteHittee->GetPosition().top - pSpriteHitter->GetHeight() - 1);



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

void drawHUD(HDC hDC) {
	char buffer[50];
	snprintf(buffer, sizeof(buffer), "Score  : %d\0", score);
	HDC cDC = CreateCompatibleDC(hDC);
	HBITMAP hBmp = CreateCompatibleBitmap(hDC, 800, 800);
	HANDLE hOld = SelectObject(cDC, hBmp);
	long lfHeight = -MulDiv(30, GetDeviceCaps(cDC, LOGPIXELSY), 72);
	HFONT hFont = CreateFont(lfHeight, 30, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, "Impact");
	SetTextColor(cDC, RGB(0, 150, 0));
	SetBkMode(cDC, TRANSPARENT);
	SelectObject(cDC, hFont);
	RECT a = { 0,0,800,800 };
	HBRUSH h = CreateSolidBrush(RGB(255, 0, 255));
	FillRect(cDC, &a, h);

	//TextOut(cDC, 200, 20, buffer, -1);
	RECT rect = { 200,20,400, 70 };
	DrawTextA(cDC, buffer, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
	for (size_t i = 0; i < _sCharacter->life; i++) {

		_pLife->Draw(cDC, 20 + i * 30, 20, TRUE);
	}
	for (size_t i = 0; i < _sCharacter->magazine; i++) {

		_pFireIcon->Draw(cDC, 20 + i * 50, 51, TRUE);
	}

	TransparentBlt(hDC, 0, 0, 800, 800, cDC, 0, 0, 800, 800, RGB(255, 0, 255));
	SelectObject(cDC, hOld);
	DeleteObject(hBmp);
	DeleteObject(cDC);
	DeleteObject(h);
	DeleteObject(hFont);
}

void drawEditMode(HDC hDC) {
	if (editMod) {


		RECT rect = { 0,0,0,0 };

		POINT p = { 0,0 };
		LPCSTR message = TEXT("Dev Mod Activated");
		DrawTextA(hDC, message, -1, &rect, DT_SINGLELINE | DT_NOCLIP);

		char buffer[50];
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

		if (maxSp < _sCharacter->GetVelocity().y) {
			maxSp = _sCharacter->GetVelocity().y;
		}

		snprintf(buffer, sizeof(buffer), "Character velocity.y  : %d  ", maxSp);
		message1 = (buffer);
		rect.top = 100;
		DrawTextA(hDC, message1, -1, &rect, DT_SINGLELINE | DT_NOCLIP);



	}
}

void createScreenElements(int** map, HDC hDC) {
	for (size_t i = 0; i < 32; i++)
	{
		for (size_t j = 0; j < 18; j++)
		{
			if (map[i][j] == 3)
			{
				Tile* tile = new Tile(hDC, _hInstance);
				tile->type = 0;
				tile->SetVelocity(0, -10);
				tile->SetPosition((i * PLATFORM_S) + 20, j * PLATFORM_S);
				_pGame->AddSprite(tile);
				_Scene->addSpriteTile(tile); // may not be necessary

			} // moving tile(vertical)   // moving tiles bounces back in case of collision with platform ( case 1 and 2 )

			else if (map[i][j] == 4) {

			}
			else if (map[i][j] == 5) {
				SpellCaster* sp = new SpellCaster(hDC, _hInstance);

				sp->SetPosition(i * PLATFORM_S, j * PLATFORM_S - 40);

				_pGame->AddSprite(sp);
				_Scene->addSpellCaster(sp); // may not be necessary

			}
			else if (map[i][j] == 6) {
				Demon* demon = new Demon(hDC, _hInstance);

				demon->SetPosition(i * PLATFORM_S, j * PLATFORM_S - 40);
				_pGame->AddSprite(demon);
				_Scene->addDemon(demon); // may not be necessary

			}
		}
	}
}

void addParallaxBgs(HDC hDC) {

	_pBackground = new Bitmap(hDC, IDB_BG_LAYER1, _hInstance);
	_Scene->addBackground(_pBackground);
	_pBackground = new Bitmap(hDC, IDB_BG_LAYER2, _hInstance);
	_Scene->addBackground(_pBackground);
	_pBackground = new Bitmap(hDC, IDB_BG_LAYER3, _hInstance);
	_Scene->addBackground(_pBackground);
	_pBackground = new Bitmap(hDC, IDB_BG_LAYER4, _hInstance);
	_Scene->addBackground(_pBackground);
	_pBackground = new Bitmap(hDC, IDB_BG_LAYER5, _hInstance);
	_Scene->addBackground(_pBackground);

	_pLife = new Bitmap(hDC, IDB_LIFE, _hInstance);
	_pGameOver = new Bitmap(hDC, IDB_GAMEOVER, _hInstance);
	_pFireIcon = new Bitmap(hDC, IDB_FIREBURST_ICO, _hInstance);
	char_null = new Bitmap(hDC, IDB_CHAR_NULL, _hInstance);
}

void UpdateGame() {
	int** map = _Scene->getMap(x);

	if ((_sCharacter->GetPosition().left + vx < 800) || vx < 0) {
		_sCharacter->SetPosition(_sCharacter->GetPosition().left + vx, _sCharacter->GetPosition().top);
		_pGame->UpdateSprites(map, x, 0);

	}
	else {
		_pGame->UpdateSprites(map, x, vx);
		x += vx;

	}
	for (size_t i = 0; i < 38; i++)
	{

		delete map[i];


	}
	delete[] map;
}
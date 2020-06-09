#include "Scene.h"


// Bu class daha yarým içine scenede olmasý gereken herþey olucak
// Düþmanlarda dahil.
Scene::Scene(HDC hDC, HINSTANCE  _hInstance) {
	tiles[0] = new Bitmap(hDC, IDB_TILE1, _hInstance);
	tiles[1] = new Bitmap(hDC, IDB_TILE2, _hInstance);
	memset(p_iPlatform, 0, sizeof(p_iPlatform));
	_bSpell = new Bitmap(hDC, IDB_SPELL, _hInstance);

	_bCharFireBurstR = new Bitmap(hDC, IDB_FIREBURST_R, _hInstance);
	_bCharFireBurstL = new Bitmap(hDC, IDB_FIREBURST_L, _hInstance);
}


void Scene::addBackground(Bitmap* img) {
	background.push_back(img);
}
void Scene::addSpellCaster(SpellCaster* obj) { 
	this->spCasters.push_back(obj);

}
void Scene::addDemon(Demon* obj) { 
	this->demons.push_back(obj);

}
void Scene::addSpell(Spell* obj) { 
	this->spells.push_back(obj);
}

void Scene::addSpriteTile(Tile* obj) { // game objeler bir kez yüklenicek çok defa kullanýcak ram kullanýmý azalsýn diye böyle yapýldý
	this->tilesSprites.push_back(obj);
}

void Scene::drawScene(HDC hDc)
{
}

void Scene::drawBackground(HDC hDC, int x) {
	int sp = 1;// Burasý için class oluþturulabilir ama ben gerek duymadým. Background þeklinde attributeeleri bitmap ve speed olabilirdi ama onun yerine hýzý yavaþ olana göre ekleyip burda hýz 
				//oluþturup çarptým.
	for (Bitmap* i : background) {
		i->Draw(hDC, (sp * (-x) / 5 % 1920 + 960 + 1920) % 1920, 0, TRUE);
		i->Draw(hDC, (sp * (-x) / 5 % 1920 - 960) % 1920, 0, TRUE); // en öndeki katman ile platformun hızı eşit olmalı
		sp++;
	}

	platform = CreateOffscreenBmp(2000, 1080, x);
	
	

	BlitToHdc(hDC, platform, -(x % tiles[0]->GetHeight()), 0, 2000, 1080);
	DeleteObject(platform);

}
HBITMAP Scene::CreateOffscreenBmp(int wd, int hgt, int x) {
	// Get a device context to the screen.
	HDC hdcScreen = GetDC(NULL);
	// Create a device context
	HDC hdcBmp = CreateCompatibleDC(hdcScreen);

	// Create a bitmap and attach it to the device context we created above...
	HBITMAP bmp = CreateCompatibleBitmap(hdcScreen, wd, hgt);
	HBITMAP hbmOld = static_cast<HBITMAP>(SelectObject(hdcBmp, bmp));
	HBRUSH solidPurple = CreateSolidBrush(RGB(255, 0, 255));
	RECT r = { 0, 0, 2000, 1080 };
	FillRect(hdcBmp, &r, solidPurple);
	// Now, you can draw into bmp using the device context hdcBmp...0

	HBRUSH tile_0 = CreatePatternBrush(tiles[0]->GetHbitmap());
	HBRUSH tile_1 = CreatePatternBrush(tiles[1]->GetHbitmap());
	for (int i = 0; i < 34; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			RECT rect = { i * PLATFORM_S, j * PLATFORM_S,(i * PLATFORM_S) + PLATFORM_S, (j * PLATFORM_S) + 40 };

			switch (p_iPlatform[i + x / PLATFORM_S][j])
			{
			case 1:
				FillRect(hdcBmp, &rect, tile_0);
				break;
			case 2:
				FillRect(hdcBmp, &rect, tile_1);
				break;
			default:
				break;
			}

		}
	}



	// etc...

	// Clean up the GDI objects we've created.
	SelectObject(hdcBmp, hbmOld);
	DeleteDC(hdcBmp);
	ReleaseDC(NULL, hdcScreen);
	DeleteObject(solidPurple);
	DeleteObject(tile_0);
	DeleteObject(tile_1);
	DeleteObject(hdcScreen);
	DeleteObject(solidPurple);
	DeleteObject(hbmOld);

	return bmp;
}

void Scene::BlitToHdc(HDC hdcDst, HBITMAP hbmSrc, int x, int y, int wd, int hgt) {
	HDC hdcScreen = GetDC(NULL);
	HDC hdcSrc = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmOld = static_cast<HBITMAP>(SelectObject(hdcSrc, hbmSrc));

	//BitBlt(hdcDst, x, y, wd, hgt, hdcSrc, 0, 0, SRCCOPY);
	TransparentBlt(hdcDst, x, y, wd, hgt, hdcSrc, 0, 0, wd, hgt, RGB(255, 0, 255));
	SelectObject(hdcSrc, hbmOld);
	DeleteDC(hdcSrc);
	ReleaseDC(NULL, hdcScreen);
	DeleteObject(hbmOld);

}




int** Scene::getMap(int x)
{
	int** map = 0;
	map = new int* [38];
	int k = -2;
	if (x < 120)
		k = 0;
	for (size_t i = 0; i < 38; i++)
	{
		map[i] = new int[18];
		for (size_t j = 0; j < 18; j++)
		{
			map[i][j] = p_iPlatform[i+k + x / tiles[0]->GetHeight()][j];


		}

	}
	return map;
}
void Scene::addTile(int xCur, int yCur, int type, int x)
{
	p_iPlatform[xCur/PLATFORM_S][yCur / PLATFORM_S] = type;
}

void Scene::saveLevel(char* levelName)
{
	std::ofstream  afile;
	afile.open(levelName);
	for (size_t i = 0; i < 6000; i++)
	{
		for (size_t j = 0; j < 18; j++)
		{
			afile << std::to_string(p_iPlatform[i][j]);
			if (j != 17) // end of line
				afile << " ";
		}
		if (i != 5999) // end of file
			afile << std::endl;
	}
	afile.close();
}

void Scene::loadLevel(char* levelName)
{
	std::ifstream file;
	file.open(levelName);
	std::string str;
	int i = 0, j = 0;
	while (std::getline(file, str)) {							//iterating each line in file

		char* token;							//split by space
		char* line = new char[str.length()];
		line = strdup(str.c_str());
		token = strtok(line, " ");
		while (token != NULL) {									//iterating each element in line
			p_iPlatform[i][j++] = std::stoi(token);
			token = strtok(NULL, " ");

		}
		j = 0;
		i++;
	}
	file.close();
}

int Scene::testCollisionRight(int x, int y) {




	switch (p_iPlatform[x / tiles[0]->GetHeight()][y / tiles[0]->GetHeight()])
	{
	case 1:
		return 1;
		break;
	case 2:
		return 1;
		break;
	default:
		break;
	}


	return 0;
}
int Scene::testCollisionLeft(int x, int y) {

	switch (p_iPlatform[(x / tiles[0]->GetHeight()) ][y / tiles[0]->GetHeight()])
	{
	case 1:
		return 2;
		break;
	case 2:
		return 2;
		break;
	default:
		break;
	}


	return 0;
}

std::vector<Sprite*> Scene::updateScene(int x, int charXPos, int charYPos, HDC hDC, HINSTANCE _hInstance)
{
	std::vector<Sprite*> newSprites; 
	////////////////////////////////
	// --- AI Update Functions ---//
	////////////////////////////////
	for (Demon* demon : demons) {
		demon->act(0);
	}
	//random spell for spellcasters 
	if (rand() % 10 == 0 && !spCasters.empty()) {
		for (SpellCaster* spcaster : spCasters) {
			if (!spcaster->IsStateHalt() && spcaster->deathMark != TRUE) {
				if (rand() % 100 < SPELL_RATE) {
					Spell* spell = spellF(POINT{ charXPos, charYPos }, POINT{ spcaster->GetPosition().left, spcaster->GetPosition().top });
					newSprites.push_back(spell);
				}
				spcaster->act(0);
			}
			
		}

	}

	if (rand() % 10 == 0 && !demons.empty()) {
		for (Demon* demon : demons) {
			if (!demon->IsStateHalt() && demon->deathMark != TRUE) {
				if (rand() % 100 < FIRE_RATE) {
					int dir = (demon->GetVelocity().x < 0) ? 1 : 0;
					FireBurst* fire = fireF(POINT{ charXPos, charYPos }, POINT{ demon->GetPosition().left, demon->GetPosition().top },dir);
					newSprites.push_back(fire);
				}
				demon->act(0);
			}

		}

	}

	
	

	if (x/60 > p ) {
		p = x / 60;
		for (size_t i = 0; i < 18; i++)
		{
			
			if (p_iPlatform[ x / tiles[0]->GetHeight()+31][i] == 3)
			{
				Tile* tile = new Tile(hDC, _hInstance);
				tile->type = 0;
				tile->SetVelocity(0, -10);
				tile->SetBoundsAction(BA_BOUNCE);
				tile->SetPosition((32 * PLATFORM_S)+20, i * PLATFORM_S);
				newSprites.push_back(tile);
				addSpriteTile(tile); // may not be necessary

			} // moving tile(vertical)   // moving tiles bounces back in case of collision with platform ( case 1 and 2 )
			
			else if (p_iPlatform[x / tiles[0]->GetHeight() + 31][i] == 5) {
				SpellCaster* sp = new SpellCaster(hDC, _hInstance);

				sp->SetPosition(31 * PLATFORM_S, i * PLATFORM_S-40);
				newSprites.push_back(sp);
				addSpellCaster(sp); // may not be necessary

			}
			else if (p_iPlatform[x / tiles[0]->GetHeight() + 31][i] == 6) {
				Demon* demon = new Demon(hDC, _hInstance);

				demon->SetPosition(31 * PLATFORM_S, i * PLATFORM_S-40);
				newSprites.push_back(demon);
				addDemon(demon); // may not be necessary

			}
		}
		
	}
	for (auto tile : tilesSprites) {
		 tile->SetVelocity(0, tile->GetVelocity().y);
	}

	


	


	return newSprites;
}
void Scene::createNextScreen(int x) // bir önceki batch in devamlılığı yok
{
	srand(time(NULL));
	int startIndx = (x + 1920) / PLATFORM_S;
	for (size_t i = 2; i < 18; i+=4)
	{
		int randRemainder = 1;
		for (size_t j = 0; j < 34; j++)
		{
			if (rand() % 50 < randRemainder) {
				randRemainder = 1;
				int spaceCount = (rand() % 4)+1;
				
				for (size_t k = 0; k < spaceCount; k++)
				{
					p_iPlatform[j + startIndx+k][i] = 0;
				}
				if (spaceCount > 2 && rand()%2 ==0) {
					p_iPlatform[j + startIndx][i] = 3;
				}
				j += spaceCount;
				for (size_t k = 0; k < spaceCount*2; k++)
				{
					if (rand() % 25 == 0) {
						if (rand() % 2 == 0) {
							p_iPlatform[j + startIndx + k][i-1] = 5;
						}
						else {
							p_iPlatform[j + startIndx + k][i-1] = 6;
						}
					}
					p_iPlatform[j + startIndx + k][i] = 1;
				}
				
			}
			else {
				randRemainder++;
				bool freeForTile = true;
				if (j < 3 && j + startIndx >3) {
					for (size_t k = 1; k < 4; k++)
					{
						if (p_iPlatform[j + startIndx - k][i] == 3) {
							freeForTile = false;
							break;
						}
					}
				}
				if (freeForTile) {
					if (rand() % 30 == 0) {
						if (rand() % 2 == 0) {
							p_iPlatform[j + startIndx][i - 1] = 5;
						}
						else {
							p_iPlatform[j + startIndx][i - 1] = 6;
						}
					}
					p_iPlatform[j + startIndx][i] = 1;// enemy insertion
				}
					
				else
					p_iPlatform[j + startIndx][i] = 0;

			}
		}
	}
}

Spell* Scene::spellF(POINT target, POINT pos) {
	Spell* spell = new Spell(_bSpell, target);
	spell->SetPosition(pos.x , pos.y+20);
	spell->calcNextPos(target.x, target.y);
	return spell;
}
FireBurst* Scene::fireF(POINT target, POINT pos, int dir) {
	FireBurst* fb = new FireBurst();
	fb->enemy = TRUE;
	if (dir == 1) {

		fb->SetBitmap(_bCharFireBurstL);
		fb->SetPosition(pos.x-20, pos.y + 30);

		fb->SetVelocity(-30, 0);

	}
	else {

		fb->SetBitmap(_bCharFireBurstR);
		fb->SetPosition(pos.x+70, pos.y + 30);

		fb->SetVelocity(30, 0);


	}
	fb->SetNumFrames(6);
	fb->SetFrameDelay(1);
	fb->SetBounds(RECT{ 50, 50, 1870, 1030 });
	fb->SetBoundsAction(BA_DIE);
	return fb;
}


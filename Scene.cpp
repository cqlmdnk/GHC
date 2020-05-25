#include "Scene.h"


// Bu class daha yarým içine scenede olmasý gereken herþey olucak
// Düþmanlarda dahil.
Scene::Scene(HDC hDC, HINSTANCE  _hInstance) {
	tiles[0] = new Bitmap(hDC, IDB_TILE1, _hInstance);
	tiles[1] = new Bitmap(hDC, IDB_TILE2, _hInstance);
	tiles[2] = new Bitmap(hDC, IDB_TILE2, _hInstance); // tile tipleri buradan yüklenecek
	memset(p_iPlatform, 0, sizeof(p_iPlatform));
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
	for (int i = 0; i < 36; i++)
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
	map = new int* [36];

	for (size_t i = 0; i < 36; i++)
	{
		map[i] = new int[18];
		for (size_t j = 0; j < 18; j++)
		{
			map[i][j] = p_iPlatform[i + x / tiles[0]->GetHeight()][j];


		}

	}
	return map;
}
void Scene::addTile(int xCur, int yCur, int type, int x)
{
	p_iPlatform[xCur][yCur] = type;
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




	switch (p_iPlatform[1 + x / tiles[0]->GetHeight()][y / tiles[0]->GetHeight()])
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

std::vector<Sprite*> Scene::updateScene(int x, int charYPos, HDC hDC, HINSTANCE _hInstance)
{
	std::vector<Sprite*> newSprites;

	//random for spellcasters tile move spell
	BOOL tileMagic = FALSE;
	if (rand() % 10 == 0 && !spCasters.empty()) {
		for (SpellCaster* spcaster : spCasters) {
			if (!spcaster->IsStateHalt() && spcaster->deathMark != TRUE) {
				if (rand() % 100 < 20) {
					Spell* sp = spcaster->fire(POINT{ 340, charYPos + rand() % 10 - 5 });
					sp->SetVelocity(2, 3);
					addSpell(sp);
					newSprites.push_back(sp);
				}
				spcaster->act(0);
			}
			if (!spcaster->IsStateHalt())
				tileMagic = TRUE;
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
				tile->SetPosition(31 * PLATFORM_S, i * PLATFORM_S);
				newSprites.push_back(tile);
				addSpriteTile(tile); // may not be necessary

			} // moving tile(vertical)   // moving tiles bounces back in case of collision with platform ( case 1 and 2 )
			else if (p_iPlatform[x / tiles[0]->GetHeight()+31][i] == 4) {
				Tile* tile = new Tile(hDC, _hInstance);
				tile->type = 1;
				tile->SetVelocity(-10, 0);
				tile->SetBoundsAction(BA_BOUNCE);
				tile->SetPosition(31 * PLATFORM_S, i * PLATFORM_S);
				newSprites.push_back(tile);
				addSpriteTile(tile); // may not be necessary

				 // moving tile(horizontal) // moving tiles bounces back in case of collision with platform ( case 1 and 2 )
			}
		}
		
	}
	for (auto tile : tilesSprites) {
		if (tile->type == 0) {
			tile->SetVelocity(0, tile->GetVelocity().y);
		}
		else {
			tile->SetVelocity(tile->GetVelocity().x, 0);
		}
	}

	for (Demon* demon : demons) {
		demon->act(0);
	}


	


	return newSprites;
}

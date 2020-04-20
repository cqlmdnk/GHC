#include "Scene.h"


// Bu class daha yarým içine scenede olmasý gereken herþey olucak
// Düþmanlarda dahil.
Scene::Scene(HDC hDC) {
	tiles[0] = new Bitmap(hDC, TEXT("resources/tile1.bmp"));
	tiles[1] = new Bitmap(hDC, TEXT("resources/tile2.bmp"));
	tiles[2] = new Bitmap(hDC, TEXT("resources/tile2.bmp")); // tile tipleri buradan yüklenecek
	memset(p_iPlatform, 0, sizeof(p_iPlatform));
}


void Scene::addBackground(Bitmap* img) {
	background.push_back(img); // gelen obje sona ekleniyor
}
void Scene::addGameObject(Sprite* obj) { // game objeler bir kez yüklenicek çok defa kullanýcak ram kullanýmý azalsýn diye böyle yapýldý
	objects.push_back(obj);// gelen obje sona ekleniyor
}

void Scene::drawScene(HDC hDc)
{
}

void Scene::drawBackground(HDC hDc, int x) {
	int sp = 1;// Burasý için class oluþturulabilir ama ben gerek duymadým. Background þeklinde attributeeleri bitmap ve speed olabilirdi ama onun yerine hýzý yavaþ olana göre ekleyip burda hýz 
				//oluþturup çarptým.
	for (Bitmap* i : background) {
		i->Draw(hDc, (sp * (-x) % 1920 + 960 + 1920) % 1920, 0, TRUE); 
		i->Draw(hDc, (sp * (-x) % 1920 - 960) % 1920, 0, TRUE); // en öndeki katman ile platformun hızı eşit olmalı
		sp++;
	}
	if (x / 40 != p  && x != 0) { // hız a bağlı olarak girmeye biliyor // hızdan bağısmız başka bir x e ihtiyaç var // p eklendi durum düzeldi
		
				platform = CreateOffscreenBmp(2000, 1080, x);
				p = x / 40;
	}
	
	BlitToHdc(hDc, platform, -(x% 40), 0, 2000, 1080);

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
	for (int i = 0; i < 57; i++)
	{
		for (int j = 0; j < 27; j++)
		{
			RECT rect = { i*40, j*40,( i* 40)+40, (j * 40)+40 };
			switch (p_iPlatform[i + x/40][j])
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

	return bmp;
}

void Scene::BlitToHdc(HDC hdcDst, HBITMAP hbmSrc, int x, int y, int wd, int hgt) {
	HDC hdcScreen = GetDC(NULL);
	HDC hdcSrc = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmOld = static_cast<HBITMAP>(SelectObject(hdcSrc, hbmSrc));

	//BitBlt(hdcDst, x, y, wd, hgt, hdcSrc, 0, 0, SRCCOPY);
	TransparentBlt(hdcDst, x, y, wd, hgt, hdcSrc, 0, 0,wd,hgt , RGB(255, 0, 255));
	SelectObject(hdcSrc, hbmOld);
	DeleteDC(hdcSrc);
	ReleaseDC(NULL, hdcScreen);
}




bool** Scene::getMap(int x)
{
	bool** map = 0;
	map = new bool* [48];

	for (size_t i = 0; i < 48; i++)
	{
		map[i] = new bool[27];
		for (size_t j = 0; j < 27; j++)
		{
			//map[i][j] = p_iPlatform[i + x*10][j] != 0;
			

		}
		
	}
	return map;
}

void Scene::addTile(int xCur, int yCur, int type, int x)
{
		p_iPlatform[x/40+xCur][yCur] = type;
}

void Scene::saveLevel(char* levelName)
{
	std::ofstream  afile;
	afile.open(levelName);
	for (size_t i = 0; i < 6000; i++)
	{
		for (size_t j = 0; j < 30; j++)
		{
			afile << std::to_string(p_iPlatform[i][j]);
			if(j != 29) // end of line
				afile << " ";
		}
		if(i != 5999) // end of file
			afile << std::endl;
	}
	afile.close();
}

void Scene::loadLevel(char * levelName)
{
	std::ifstream file;
	file.open(levelName);
	std::string str;
	int i = 0, j = 0;
	while (std::getline(file, str)) {							//iterating each line in file

		char *token;							//split by space
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

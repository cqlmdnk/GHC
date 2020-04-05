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
void Scene::addGameObject(GameObject* obj) { // game objeler bir kez yüklenicek çok defa kullanýcak ram kullanýmý azalsýn diye böyle yapýldý
	objects.push_back(obj);// gelen obje sona ekleniyor
}

void Scene::drawBackground(HDC hDc, int x) {
	int sp = 1;// Burasý için class oluþturulabilir ama ben gerek duymadým. Background þeklinde attributeeleri bitmap ve speed olabilirdi ama onun yerine hýzý yavaþ olana göre ekleyip burda hýz 
				//oluþturup çarptým.
	for (Bitmap* i : background) {
		i->Draw(hDc, (sp * (-x)*10 % 1920 + 960 + 1920) % 1920, 0, TRUE); 
		i->Draw(hDc, (sp * (-x)*10 % 1920 - 960) % 1920, 0, TRUE);
		sp = sp + 1;
	}
	for (size_t i = 0; i < 55; i++)
	{
		for (size_t j = 0; j < 30; j++)
		{
			switch (p_iPlatform[i+x][j])
			{
			case 1:
				tiles[0]-> Draw(hDc, i*35, j * 35, TRUE);
				break;
			case 2:
				tiles[1]->Draw(hDc, i * 35, j * 35, TRUE);
				break;
			default:
				break;
			}
			
		}
	}

}

void Scene::addTile(int x, int y, int type)
{
		p_iPlatform[x][y] = type;
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

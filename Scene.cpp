#include "Scene.h"

// Bu class daha yar�m i�ine scenede olmas� gereken her�ey olucak
// D��manlarda dahil.


void Scene::addBackground(Bitmap* img) {
	background.push_back(img); // gelen obje sona ekleniyor
}
void Scene::addGameObject(GameObject* obj) { // game objeler bir kez y�klenicek �ok defa kullan�cak ram kullan�m� azals�n diye b�yle yap�ld�
	objects.push_back(obj);// gelen obje sona ekleniyor
}

void Scene::drawBackground(HDC hDc, int x) {
	int sp = 1;// Buras� i�in class olu�turulabilir ama ben gerek duymad�m. Background �eklinde attributeeleri bitmap ve speed olabilirdi ama onun yerine h�z� yava� olana g�re ekleyip burda h�z 
				//olu�turup �arpt�m.
	for (Bitmap* i : background) {
		i->Draw(hDc, (sp * x % 1920 + 960 + 1920) % 1920, 0, TRUE);
		i->Draw(hDc, (sp * x % 1920 - 960) % 1920, 0, TRUE);
		sp = sp + 1;
	}

}
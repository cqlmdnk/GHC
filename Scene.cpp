#include "Scene.h"

// Bu class daha yarým içine scenede olmasý gereken herþey olucak
// Düþmanlarda dahil.


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
		i->Draw(hDc, (sp * x % 1920 + 960 + 1920) % 1920, 0, TRUE);
		i->Draw(hDc, (sp * x % 1920 - 960) % 1920, 0, TRUE);
		sp = sp + 1;
	}

}
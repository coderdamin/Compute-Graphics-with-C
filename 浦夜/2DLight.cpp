#include "Scene.h"
#include "SavePNG.h"
#include <math.h>

#define WIDTH 512
#define HEIGHT 512

int main() {
	SavePNG* pPng = SavePNG::CreatePNG("Reflection_1.png", WIDTH, HEIGHT, false);
	Scene* pScene = new Scene;
	//Circle* pCircle1 = new Circle(Vector(0.4f, 0.4f), 0.3f);
	//Circle* pCircle2 = new Circle(Vector(0.6f, 0.6f), 0.3f);
	//ShapeUnion* pShape = new ShapeUnion(pCircle1, pCircle2);
	//Entity* pEntity = new Entity(pShape, Color(0.0f, 0.0f, 0.0f), 0.9f);
	Circle* pCircle3 = new Circle(Vector(0.4f, 0.7f), 0.2f);
	Entity* pEntity2 = new Entity(pCircle3, Color(2.0f, 2.0f, 0.0f), 0.0f);
	//pScene->AddEntity(pEntity);
	pScene->AddEntity(pEntity2);

	Plane* pPlane1 = new Plane(Vector(0.9f, 0.5f), Vector(0.7f, 0.5f), Vector(0.8f, 0.47f));
	Plane* pPlane2 = new Plane(Vector(0.9f, 0.5f), Vector(0.8f, 0.4f), Vector(0.8f, 0.47f));
	Plane* pPlane3 = new Plane(Vector(0.8f, 0.4f), Vector(0.7f, 0.5f), Vector(0.8f, 0.47f));
	ShapeIntersect* pIntersect1 = new ShapeIntersect(pPlane1, pPlane2);
	ShapeIntersect* pIntersect2 = new ShapeIntersect(pIntersect1, pPlane3);
	Entity* pEntity3 = new Entity(pIntersect2, Color(0.2f, 0.9f, 1.1f), 0.9f);
	pScene->AddEntity(pEntity3);

	unsigned char* pcPixelBuff = pPng->GetPNGBuff();
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x, pcPixelBuff += 3) {
			Color color = pScene->Sample(Vector((float)x / WIDTH, (float)y / HEIGHT));
			pcPixelBuff[0] = (int)fminf(color.m_fr * 255.0f, 255.0f);
			pcPixelBuff[1] = (int)fminf(color.m_fg * 255.0f, 255.0f);
			pcPixelBuff[2] = (int)fminf(color.m_fb * 255.0f, 255.0f);
		}
	}
	if (pScene != nullptr) {
		delete pScene;
	}
	if (pPng != nullptr) {
		pPng->Save();
		delete pPng;
	}
	return 0;
}

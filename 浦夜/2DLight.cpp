#include "Scene.h"
#include "SavePNG.h"
#include <math.h>
#include <initializer_list>
using std::initializer_list;

#define WIDTH 512
#define HEIGHT 512

Shape* GeneratePolygon(initializer_list<Vector> pointList);
int main() {
	SavePNG* pPng = SavePNG::CreatePNG("Reflection_2.png", WIDTH, HEIGHT, false);
	//Scene* pScene = new Scene;
	////Circle* pCircle1 = new Circle(Vector(0.4f, 0.4f), 0.3f);
	////Circle* pCircle2 = new Circle(Vector(0.6f, 0.6f), 0.3f);
	////ShapeUnion* pShape = new ShapeUnion(pCircle1, pCircle2);
	////Entity* pEntity = new Entity(pShape, Color(0.0f, 0.0f, 0.0f), 0.9f);
	//Circle* pCircle3 = new Circle(Vector(0.4f, 0.7f), 0.2f);
	//Entity* pEntity2 = new Entity(pCircle3, Color(2.0f, 2.0f, 0.0f), 0.0f);
	////pScene->AddEntity(pEntity);
	//pScene->AddEntity(pEntity2);

	//Plane* pPlane1 = new Plane(Vector(0.9f, 0.5f), Vector(0.7f, 0.5f), Vector(0.8f, 0.47f));
	//Plane* pPlane2 = new Plane(Vector(0.9f, 0.5f), Vector(0.8f, 0.4f), Vector(0.8f, 0.47f));
	//Plane* pPlane3 = new Plane(Vector(0.8f, 0.4f), Vector(0.7f, 0.5f), Vector(0.8f, 0.47f));
	//ShapeIntersect* pIntersect1 = new ShapeIntersect(pPlane1, pPlane2);
	//ShapeIntersect* pIntersect2 = new ShapeIntersect(pIntersect1, pPlane3);
	//Entity* pEntity3 = new Entity(pIntersect2, Color(0.2f, 0.9f, 1.1f), 0.9f);
	//pScene->AddEntity(pEntity3);

	Circle* c1 = new Circle(Vector(0.5f, 0.6f), 0.1f);
	Circle* c2 = new Circle(Vector(0.5f, 0.7f), 0.12f);
	Shape* su1 = new ShapeUnion(c1, c2);					//两个圆的并
	Shape* triangle = GeneratePolygon({ { 0.9f,0.5f },{ 0.7f, 0.5f },{ 0.8f, 0.4f } });
	Shape* quad1 = GeneratePolygon({ { 0.3f, 0.4f },{ 0.4f, 0.4f },{ 0.4f, 0.3f },{ 0.3f, 0.3f } });
	Shape* quad2 = GeneratePolygon({ { 0.2f, 0.5f },{ 0.3f, 0.5f },{ 0.3f, 0.6f },{ 0.2f, 0.6f } });
	Entity* e1 = new Entity(su1, Color(1.8f, 0.9f, 0.7f), 0.0f);			//葫芦形的黄光
	Entity* e2 = new Entity(triangle, Color(0.2f, 0.9f, 1.1f));	//三角形的蓝光
	Entity* e3 = new Entity(quad1, Color(0.05f, 0.05f, 0.2f), 0.8f);
	Entity* e4 = new Entity(quad2, Color(0.05f, 0.05f, 0.2f), 0.8f);
	//Entity* e3 = new Entity(quad, { 0.05f, 0.05f, 0.2f }, 1.0f);		//正方形的深蓝色
	Shape* s = GeneratePolygon({ { 1.f, 2.f },{ 3.f, 4.f } });
	Scene* pScene = new Scene;
	pScene->AddEntity(e1);
	pScene->AddEntity(e2);
	pScene->AddEntity(e3);
	pScene->AddEntity(e4);

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

Shape* GeneratePolygon(initializer_list<Vector> pointList) {
	Vector pointCenter;
	for (auto point : pointList) {
		pointCenter += point;
	}
	pointCenter /= pointList.size();

	Shape* pIntersect = nullptr;
	for (auto point = pointList.begin(); point + 1 != pointList.end(); ++point) {
		Plane* pPlane = new Plane(*point, *(point + 1), pointCenter);
		if (pIntersect == nullptr) {
			pIntersect = pPlane;
		}
		else {
			pIntersect = new ShapeIntersect(pIntersect, pPlane);
		}
	}
	Plane* pPlane = new Plane(*pointList.begin(), *(pointList.end() - 1), pointCenter);
	pIntersect = new ShapeIntersect(pIntersect, pPlane);
	return pIntersect;
}

#pragma once
#include "Scene.h"
#include "Shape.h"
#include <initializer_list>
using std::initializer_list;

Shape* GeneratePolygon(initializer_list<Vector> pointList);

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


Scene* GenerateScene1() {
	Circle* c1 = new Circle(Vector(0.5f, 0.6f), 0.1f);
	Circle* c2 = new Circle(Vector(0.5f, 0.7f), 0.12f);
	Shape* su1 = new ShapeUnion(c1, c2);					//两个圆的并
	Shape* triangle = GeneratePolygon({ { 0.9f,0.5f },{ 0.7f, 0.5f },{ 0.8f, 0.4f } });
	Shape* quad1 = GeneratePolygon({ { 0.3f, 0.4f },{ 0.4f, 0.4f },{ 0.4f, 0.3f },{ 0.3f, 0.3f } });
	Shape* quad2 = GeneratePolygon({ { 0.2f, 0.5f },{ 0.3f, 0.5f },{ 0.3f, 0.6f },{ 0.2f, 0.6f } });
	Entity* e1 = new Entity(su1, Color(1.8f, 0.9f, 0.7f), 0.0f);			//葫芦形的黄光
	Entity* e2 = new Entity(triangle, Color(0.2f, 0.9f, 1.1f), 1.0f);	//三角形的蓝光
	Entity* e3 = new Entity(quad1, Color(0.05f, 0.05f, 0.2f), 0.8f, 1.0f);
	Entity* e4 = new Entity(quad2, Color(0.05f, 0.05f, 0.2f), 0.8f);
	//Entity* e3 = new Entity(quad, { 0.05f, 0.05f, 0.2f }, 1.0f);		//正方形的深蓝色
	Shape* s = GeneratePolygon({ { 1.f, 2.f },{ 3.f, 4.f } });
	Scene* pScene = new Scene;
	pScene->AddEntity(e1);
	pScene->AddEntity(e2);
	pScene->AddEntity(e3);
	pScene->AddEntity(e4);
	return pScene;
}

Scene* GenerateScene2() {
	Circle* c1 = new Circle(Vector(1.0f, -0.5f), 0.05f);
	Circle* c3 = new Circle(Vector(0.0f, -0.5f), 0.05f);
	Circle* c2 = new Circle(Vector(0.5f, 0.3f), 0.3f);
	Plane* p1 = new Plane(0.0f, 1.0f, -0.3f);
	Entity* e1 = new Entity(c1, Color(2.0f, 9.0f, 11.0f));
	Entity* e2 = new Entity(c3, Color(11.0f, 2.0f, 9.0f));
	Shape* s1 = new ShapeIntersect(p1, c2);
	Entity* e3 = new Entity(s1, Color(0.0f, 0.0f, 0.0f), 0.2f, 1.0f);
	Scene* pScene = new Scene;
	pScene->AddEntity(e1);
	pScene->AddEntity(e2);
	pScene->AddEntity(e3);
	return pScene;
}

#pragma once
#include "Entity.h"
#include <list>
#include <stdlib.h>
#include <algorithm>

#define N 64
#define MAX_STEP 8
#define BIAS 1e-4f

class Scene{
public:
	Scene() {}
	~Scene() {
		for (auto pEntity : m_ListEntity) {
			delete pEntity;
		}
		m_ListEntity.clear();
	}

public:
	void AddEntity(Entity* pEntity) {
		if (pEntity == nullptr) {
			return;
		}
		if (find(m_ListEntity.begin(), m_ListEntity.end(), pEntity) != m_ListEntity.end()) {
			return;
		}
		m_ListEntity.push_back(pEntity);
	}

	Color Sample(Vector& point) {
		Color color(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < N; ++i){
			float a = TWO_PI*(i + (float)rand() / RAND_MAX) / N;
			color += GetColor(point, Vector(cosf(a), -sinf(a)));
		}
		color /= N;
		return color;
	}

	Color GetColor(Vector& point, Vector& direct, int nDepth = 0) {
		Color color(0.0f, 0.0f, 0.0f);
		if (nDepth >= MAX_STEP) {
			return color;
		}
		Entity* pNearEntity = nullptr;
		float fSquareDistance = 10000.0f;
		Vector intersectionNear;
		float fReflectivity = 0.0f;
		for (auto pEntity : m_ListEntity) {
			if (pEntity->ContainsPoint(point)) {
				// 形状内部
				pNearEntity = pEntity;
				//fReflectivity = 0.0f;
				Vector intersection;
				pEntity->IsIntersect(point, direct, intersection);
				intersectionNear = intersection;
				break;
			}
			else if (pEntity->IsIntersect(point, direct)) {
				// 正常相交
				Vector intersection;
				pEntity->IsIntersect(point, direct, intersection);
				float fSquareLen = (intersection - point).SquareLen();
				if (fSquareLen < fSquareDistance) {
					fSquareDistance = fSquareLen;
					pNearEntity = pEntity;
					intersectionNear = intersection;
					fReflectivity = pEntity->GetReflectivity();
				}
			}
		}
		if (pNearEntity == nullptr) {
			return color;
		}
		// 正常相交
		color = pNearEntity->GetEmissiveColor();
		// 折射
		Vector normal;
		pNearEntity->GetNormal(intersectionNear, normal);
		normal.Normalize();
		color += Refract(*pNearEntity, intersectionNear, direct, normal, nDepth + 1);
		// 反射
		if (fReflectivity > 0.0f) {
			Vector reflexDir = direct - normal * (2 * direct.Cross(normal));
			color += (GetColor(intersectionNear += reflexDir * BIAS, reflexDir, nDepth + 1) * fReflectivity);
		}
		return color;
	}
private:
	Color Refract(Entity&enIns, Vector&point, Vector&direct, Vector&normal, int depth) {
		Color color(0.0f, 0.0f, 0.0f);
		float fRefractivity = enIns.GetRefractivity();
		if (fRefractivity <= 0.0f) {
			return color;
		}
		float idotn = direct.Cross(normal);
		float eta = fRefractivity / 1.0f; // 与空气间
		float a = 0.0f;
		if (idotn > 0.0f) {
			//t = (n1 / n2)·i - ((n1 / n2)(i·n) + (1 - (n1 / n2) ^ 2(1 - (i·n) ^ 2)) ^ (-2))·n
			float k = 1.f - eta * eta * (1.0f - idotn * idotn);
			if (k < 0.0f) {
				return color;
			}
			a = eta * idotn - sqrtf(k);
		}
		else {
			eta = 1.0f / eta;
			float k = 1.f - eta * eta * (1.0f - idotn * idotn);
			a = eta * idotn + sqrtf(k);
		}
		Vector refract = direct * eta - normal * a;
		return GetColor(point + refract * BIAS, refract, depth) * fRefractivity;
	}

private:
	std::list<Entity*> m_ListEntity;
};
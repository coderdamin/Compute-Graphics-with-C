#pragma once
#include "Entity.h"
#include <list>
#include <stdlib.h>
#include <algorithm>

#define N 64
#define MAX_STEP 8

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
			color += GetColor(point, Vector(cosf(a), sinf(a)));
		}
		color /= N;
		return color;
	}

	Color GetColor(Vector& point, Vector& direct, int nDepth = 0) {
		Color color(0.0f, 0.0f, 0.0f);
		if (nDepth >= MAX_STEP) {
			return color;
		}
		for (auto pEntity : m_ListEntity) {
			if (pEntity->ContainsPoint(point)) {
				// 形状内部
				color = pEntity->GetEmissiveColor();
				break;
			}
			else if (!pEntity->IsIntersect(point, direct)) {
				// 不相交
				continue;
			}
			else{
				// 正常相交
				Vector intersection;
				pEntity->IsIntersect(point, direct, intersection);
				color = pEntity->GetEmissiveColor();
				// 反射
				//color += GetColor(point, direct, nDepth + 1) * pEntity->GetReflectivity();
				break;
			}
		}
		return color;
	}

private:
	std::list<Entity*> m_ListEntity;
};
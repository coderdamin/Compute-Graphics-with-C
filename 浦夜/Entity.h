#pragma once
#include "Shape.h"

class Entity{
public:
	Entity() :m_pShape(nullptr){}
	Entity(Shape* pShape) :m_pShape(pShape){}
	Entity(Shape* pShape, Color&color, float fReflectivity=0) 
		: m_pShape(pShape)
		, m_EmissiveColor(color)
		, m_fReflectivity(fReflectivity)
	{}
	~Entity() {
		if (m_pShape != nullptr) {
			delete m_pShape;
		}
	}

public:
	void SetShape(Shape* pShape) {
		if (m_pShape != nullptr) {
			delete m_pShape;
		}
		m_pShape = pShape;
	}
	void SetEmissiveColor(Color&color) {
		m_EmissiveColor = color;
	}
	Color& GetEmissiveColor() {
		return m_EmissiveColor;
	}
	void SetReflectivity(float fReflectivity) {
		m_fReflectivity = fReflectivity;
	}
	float GetReflectivity() {
		return m_fReflectivity;
	}
	bool ContainsPoint(Vector& point) {
		if (m_pShape == nullptr) {
			return false;
		}
		return m_pShape->ContainsPoint(point);
	}
	bool IsIntersect(Vector& point, Vector&direct) {
		if (m_pShape == nullptr) {
			return false;
		}
		return m_pShape->IsIntersect(point, direct);
	}
	bool IsIntersect(Vector& point, Vector&direct, Vector&intersection) {
		intersection.m_fx = intersection.m_fy = 0;
		if (m_pShape == nullptr) {
			return false;
		}
		return m_pShape->IsIntersect(point, direct, intersection);
	}
private:
	float m_fReflectivity;
	Shape* m_pShape;
	Color m_EmissiveColor;
};

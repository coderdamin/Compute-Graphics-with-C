#pragma once
#include "Shape.h"

class Entity{
public:
	Entity()
		: m_pShape(nullptr)
		, m_fReflectivity(0)
		, m_fRefractivity(0)
	{}
	Entity(Shape* pShape)
		: m_pShape(pShape)
		, m_fReflectivity(0)
		, m_fRefractivity(0)
	{}
	Entity(Shape* pShape, Color&color, float fReflectivity=0, float fRefractivity = 0)
		: m_pShape(pShape)
		, m_EmissiveColor(color)
		, m_fReflectivity(fReflectivity)
		, m_fRefractivity(fRefractivity)
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
	void SetRefractivity(float fRefractivity) {
		m_fRefractivity = fRefractivity;
	}
	float GetRefractivity() {
		return m_fRefractivity;
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
		if (m_pShape == nullptr) {
			intersection.m_fx = intersection.m_fy = 0;
			return false;
		}
		return m_pShape->IsIntersect(point, direct, intersection);
	}
	bool GetNormal(Vector&point, Vector& normal) {
		if (m_pShape == nullptr) {
			normal.m_fx = normal.m_fy = 0;
			return false;
		}
		return m_pShape->GetNormal(point, normal);
	}
private:
	float m_fReflectivity;
	float m_fRefractivity;
	Shape* m_pShape;
	Color m_EmissiveColor;
};

#pragma once

#include "Basic.h"

class Shape {
public:
	virtual bool ContainsPoint(Vector& point) = 0;
	virtual bool IsIntersect(Vector& point, Vector&direct) = 0;
	virtual bool IsIntersect(Vector& point, Vector&direct, Vector&intersection) = 0;
};

class Circle : public Shape {
public:
	Circle() : m_fRadius(0.0f){}
	Circle(Vector&point, float fRadius) : m_Center(point),m_fRadius(fRadius) {}
	virtual ~Circle() {}
public:
	virtual bool ContainsPoint(Vector& point) {
		if (m_fRadius <= PRECISION) {
			return false;
		}
		return ((point - m_Center).SquareLen() <= (m_fRadius * m_fRadius));
	}
	virtual bool IsIntersect(Vector& point, Vector&direct) {
		if (m_fRadius <= PRECISION) {
			return false;
		}
		float proj = (m_Center - point).Cross(direct);
		if (proj <= 0) {
			return false;
		}
		Vector foot = point + direct * proj;
		return (foot.SquareLen() <= (m_fRadius * m_fRadius));
	}
	virtual bool IsIntersect(Vector& point, Vector&direct, Vector&intersection) {
		if (m_fRadius <= PRECISION) {
			return false;
		}
	}
private:
	Vector m_Center;
	float m_fRadius;
};
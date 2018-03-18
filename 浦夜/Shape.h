#pragma once

#include "Basic.h"
#include <math.h>

class Shape {
public:
	virtual bool ContainsPoint(Vector& point) = 0;
	virtual bool IsIntersect(Vector& point, Vector&direct) = 0;
	virtual bool IsIntersect(Vector& point, Vector&direct, Vector&intersection) = 0;
	virtual bool GetNormal(Vector&point, Vector& normal) = 0;
	virtual bool IsProfessedly(Vector&point) = 0;
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
		return ((foot - m_Center).SquareLen() <= (m_fRadius * m_fRadius));
	}
	virtual bool IsIntersect(Vector& point, Vector&direct, Vector&intersection) {
		intersection.m_fx = intersection.m_fy = 0;
		if (m_fRadius <= PRECISION) {
			return false;
		}
		bool bInCircle = ContainsPoint(point);
		float proj = (m_Center - point).Cross(direct);
		if (!bInCircle && proj <= 0) {
			return false;
		}
		Vector foot = point + direct * proj;
		float proj2 = sqrtf(m_fRadius * m_fRadius - (foot - m_Center).SquareLen());
		intersection = foot - direct * proj2;
		return (bInCircle) || ((foot - m_Center).SquareLen() <= (m_fRadius * m_fRadius));
	}
	virtual bool GetNormal(Vector&point, Vector& normal) {
		if (!IsProfessedly(point)) {
			return false;
		}
		normal = (point - m_Center);
		return true;
	}
	virtual bool IsProfessedly(Vector&point) {
		if (m_fRadius <= PRECISION) {
			return false;
		}
		return ((point - m_Center).SquareLen() == (m_fRadius * m_fRadius));
	}

private:
	Vector m_Center;
	float m_fRadius;
};

// a*x + b*y + c >= 0
class Plane : public Shape{
public:
	Plane(float fa, float fb, float fc)
		: m_fa(fa)
		, m_fb(fb)
		, m_fc(fc)
	{
		InitializeNormal();
	}
	Plane(const Vector& point1, const Vector& point2, Vector& pointC) {
		m_fa = point2.m_fy - point1.m_fy;
		m_fb = point1.m_fx - point2.m_fx;
		m_fc = point2.m_fx * point1.m_fy - point1.m_fx * point2.m_fy;
		if (!ContainsPoint(pointC)) {
			m_fa = -m_fa;
			m_fb = -m_fb;
			m_fc = -m_fc;
		}
		InitializeNormal();
	}
	virtual ~Plane() {}
public:
	virtual bool ContainsPoint(Vector& point) {
		return (m_fa*point.m_fx + m_fb*point.m_fy + m_fc >= 0);
	}
	virtual bool IsIntersect(Vector& point, Vector&direct) {
		float fProj = direct.Cross(m_Normal);
		return ((fProj < 0) || ContainsPoint(point));
	}
	virtual bool IsIntersect(Vector& point, Vector&direct, Vector&intersection) {
		intersection.m_fx = intersection.m_fy = 0;
		float fProj = direct.Cross(m_Normal);
		if ((fProj > 0) && (!ContainsPoint(point))) {
			return false;
		}
		if ((fProj < 0) && (ContainsPoint(point))) {
			intersection.m_fx = intersection.m_fy = -1.0f;
			return true;
		}
		//float fDistance = m_fa*point.m_fx + m_fb*point.m_fy + m_fc;
		//intersection = point + m_Normal * fDistance;
		intersection.m_fx = (m_fb*direct.m_fy*point.m_fx - m_fb * direct.m_fx*point.m_fy - m_fc * direct.m_fx) / (m_fa*direct.m_fx + m_fb * direct.m_fy);
		if (direct.m_fx) intersection.m_fy = (intersection.m_fx - point.m_fx)*direct.m_fy / direct.m_fx + point.m_fy;
		else intersection.m_fy = (-m_fc - m_fa * intersection.m_fx) / m_fb;
		return true;
	}
	virtual bool GetNormal(Vector&point, Vector& normal) {
		normal = m_Normal;
		return true;
	}
	virtual bool IsProfessedly(Vector&point) {
		return (m_fa*point.m_fx + m_fb*point.m_fy + m_fc == 0);
	}

private:
	void InitializeNormal() {
		m_Normal.m_fx = -m_fa;
		m_Normal.m_fy = -m_fb;
		m_Normal.Normalize();
	}
private:
	float m_fa, m_fb, m_fc;
	Vector m_Normal;
};

class ShapeUnion : public Shape{
public:
	ShapeUnion() : m_pShape1(nullptr), m_pShape2(nullptr){}
	ShapeUnion(Shape*pShape1, Shape*pShape2) : m_pShape1(pShape1), m_pShape2(pShape2) {}
	virtual ~ShapeUnion() {
		if (m_pShape1 != nullptr) {
			delete m_pShape1;
		}
		if (m_pShape2 != nullptr) {
			delete m_pShape2;
		}
	}
public:
	virtual bool ContainsPoint(Vector& point) {
		bool bResult = false;
		if (m_pShape1 != nullptr) {
			bResult = m_pShape1->ContainsPoint(point);
		}
		if (!bResult && m_pShape2 != nullptr) {
			bResult = m_pShape2->ContainsPoint(point);
		}
		return bResult;
	}
	virtual bool IsIntersect(Vector& point, Vector&direct) {
		bool bResult = false;
		if (m_pShape1 != nullptr) {
			bResult = m_pShape1->IsIntersect(point, direct);
		}
		if (!bResult && m_pShape2 != nullptr) {
			bResult = m_pShape2->IsIntersect(point, direct);
		}
		return bResult;
	}
	virtual bool IsIntersect(Vector& point, Vector&direct, Vector&intersection) {
		intersection.m_fx = intersection.m_fy = 0;
		bool bResult = false;
		if (m_pShape1 != nullptr) {
			bResult = m_pShape1->IsIntersect(point, direct, intersection);
		}
		if (!bResult && m_pShape2 != nullptr) {
			bResult = m_pShape2->IsIntersect(point, direct, intersection);
		}
		return bResult;
	}
	virtual bool GetNormal(Vector&point, Vector& normal) {
		bool bResult = false;
		if ((m_pShape1 != nullptr)
			&& m_pShape1->IsProfessedly(point)){
			bResult = m_pShape1->GetNormal(point, normal);
		}
		if ((!bResult && m_pShape2 != nullptr)
			&& m_pShape2->IsProfessedly(point)){
			bResult = m_pShape2->GetNormal(point, normal);
		}
		return bResult;
	}
	virtual bool IsProfessedly(Vector&point) {
		bool bResult = false;
		if (m_pShape1 != nullptr) {
			bResult = m_pShape1->IsProfessedly(point);
		}
		if (!bResult && m_pShape2 != nullptr) {
			bResult = m_pShape2->IsProfessedly(point);
		}
		return bResult;
	}

private:
	Shape* m_pShape1;
	Shape* m_pShape2;
};

class ShapeIntersect : public Shape{
public:
	ShapeIntersect() : m_pShape1(nullptr), m_pShape2(nullptr){}
	ShapeIntersect(Shape*pShape1, Shape*pShape2) : m_pShape1(pShape1), m_pShape2(pShape2) {}
	virtual ~ShapeIntersect() {
		if (m_pShape1 != nullptr) {
			delete m_pShape1;
		}
		if (m_pShape2 != nullptr) {
			delete m_pShape2;
		}
	}
public:
	virtual bool ContainsPoint(Vector& point) {
		bool bResult = false;
		if (m_pShape1 != nullptr) {
			bResult = m_pShape1->ContainsPoint(point);
		}
		if (bResult && m_pShape2 != nullptr) {
			bResult = m_pShape2->ContainsPoint(point);
		}
		return bResult;
	}
	virtual bool IsIntersect(Vector& point, Vector&direct) {
		bool bResult = false;
		Vector intersection1;
		Vector intersection2;
		if (m_pShape1 != nullptr) {
			bResult = m_pShape1->IsIntersect(point, direct, intersection1);
		}
		if (bResult && m_pShape2 != nullptr) {
			bResult = m_pShape2->IsIntersect(point, direct, intersection2);
		}
		if (bResult) {
			bResult = m_pShape2->ContainsPoint(intersection1) || m_pShape1->ContainsPoint(intersection2);
		}
		return bResult;
	}
	virtual bool IsIntersect(Vector& point, Vector&direct, Vector&intersection) {
		intersection.m_fx = intersection.m_fy = 0;
		bool bResult = false;
		if (m_pShape1 != nullptr) {
			bResult = m_pShape1->IsIntersect(point, direct, intersection);
		}
		Vector intersection2;
		if (bResult && m_pShape2 != nullptr) {
			bResult = m_pShape2->IsIntersect(point, direct, intersection2);
		}
		if (!bResult) {
			return false;
		}
		bool bContains1 = false, bContains2 = false;
		bContains1 = m_pShape2->ContainsPoint(intersection);
		bContains2 = m_pShape1->ContainsPoint(intersection2);
		if (bContains1 && bContains2) {
			if ((intersection2 - point).SquareLen() < (intersection - point).SquareLen()) {
				intersection = intersection2;
			}
		}
		else if (bContains1) {
		}
		else if (bContains2) {
			intersection = intersection2;
		}
		else {
			bResult = false;
		}
		return bResult;
	}
	virtual bool GetNormal(Vector&point, Vector& normal) {
		bool bResult = false;
		if ((m_pShape1 != nullptr)
			&& m_pShape1->IsProfessedly(point)){
			bResult = m_pShape1->GetNormal(point, normal);
		}
		if ((!bResult && m_pShape2 != nullptr)
			&& m_pShape2->IsProfessedly(point)){
			bResult = m_pShape2->GetNormal(point, normal);
		}
		return bResult;
	}
	virtual bool IsProfessedly(Vector&point) {
		bool bResult = false;
		if (m_pShape1 != nullptr) {
			bResult = m_pShape1->IsProfessedly(point);
		}
		if (!bResult && m_pShape2 != nullptr) {
			bResult = m_pShape2->IsProfessedly(point);
		}
		return bResult;
	}

private:
	Shape* m_pShape1;
	Shape* m_pShape2;
};
#pragma once

#include <math.h>

#define PI 3.1415926535898f
#define TWO_PI 6.28318530718f
#define PRECISION 1e-6

class Vector{
public:
	Vector() :m_fx(0.0f), m_fy(0.0f) {}
	Vector(float fx, float fy) :m_fx(fx), m_fy(fy) {}
	~Vector() {}

public:
	Vector operator+(Vector&point) {
		Vector result;
		result.m_fx = m_fx + point.m_fx;
		result.m_fy = m_fy + point.m_fy;
		return result;
	}
	Vector operator-(Vector&point) {
		Vector result;
		result.m_fx = m_fx - point.m_fx;
		result.m_fy = m_fy - point.m_fy;
		return result;
	}
	Vector operator*(float fScale) {
		Vector result;
		if (fScale < -PRECISION && fScale > PRECISION) {
			result.m_fx = m_fx * fScale;
			result.m_fy = m_fy * fScale;
		}
		return result;
	}
	Vector& operator=(Vector& point) {
		if (this != &point) {
			m_fx = point.m_fx;
			m_fy = point.m_fy;
		}
		return *this;
	}

	float Len() {
		return sqrtf(m_fx * m_fx + m_fy * m_fy);
	}

	float SquareLen() {
		return m_fx * m_fx + m_fy * m_fy;
	}

	float Cross(Vector&vec) {
		return m_fx * vec.m_fx + m_fy * vec.m_fy;
	}

public:
	float m_fx, m_fy;
};

class Color {
public:
	Color() :m_fr(0.0f), m_fg(0.0f), m_fb(0.0f) {}
	Color(float fr, float fg, float fb) :m_fr(fr), m_fg(fg), m_fb(fb) {}
	~Color() {}

public:
	Color operator*(Color& color) {
		Color result;
		result.m_fr = m_fr * color.m_fr;
		result.m_fg = m_fr * color.m_fg;
		result.m_fb = m_fr * color.m_fb;
		return result;
	}
	Color operator*(float fScale) {
		Color result;
		if (fScale < -PRECISION && fScale > PRECISION) {
			result.m_fr = m_fr * fScale;
			result.m_fg = m_fr * fScale;
			result.m_fb = m_fr * fScale;
		}
		return result;
	}
	Color& operator=(Color& color) {
		if (this != &color) {
			m_fr = color.m_fr;
			m_fg = color.m_fg;
			m_fb = color.m_fb;
		}
		return *this;
	}
	Color& operator+=(Color& color) {
		m_fr += color.m_fr;
		m_fg += color.m_fg;
		m_fb += color.m_fb;
		return *this;
	}
	Color& operator/=(float fScale) {
		if (fScale >= -PRECISION && fScale <= PRECISION) {
			m_fr = m_fg = m_fb = 0.0f;
		}
		else{
			m_fr /= fScale;
			m_fg /= fScale;
			m_fb /= fScale;
		}
		return *this;
	}
public:
	float m_fr, m_fg, m_fb;
};
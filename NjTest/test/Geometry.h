#pragma once
#include<cmath>
template<typename T>
struct Vector2D {
	T x;
	T y;
	Vector2D(T inx, T iny) :x(inx), y(iny) {}
	Vector2D() :x(0), y(0) {}
	Vector2D operator+(const Vector2D& rval) {
		return Vector2D(x + rval.x, y + rval.y);
	}
	Vector2D operator-(const Vector2D& rval) {
		return Vector2D(x - rval.x, y - rval.y);
	}
	void operator+=(const Vector2D& v) {
		x += v.x;
		y += v.y;
	}
	void operator-=(const Vector2D& v) {
		x -= v.x;
		y -= v.y;
	}
	void operator*=(float scale) {
		x *= scale;
		y *= scale;
	}
	float Magnitude()const {
		return hypot(x, y);
	}
	void Normalize() {
		float mag = Magnitude();
		x/=mag;
		y /= mag;
	}
	Vector2D Normalized()const {
		float mag = Magnitude();
		return {x/mag,y/mag};
	}
};



using Vector2 = Vector2D<int>;
using Vector2f = Vector2D<float>;
using Position2 = Vector2;
using Position2f = Vector2f;

float Dot(const Vector2f& va,const Vector2f& vb);
float Cross(const Vector2f& va, const Vector2f& vb);


struct Size {
	int w, h;
	Size() {}
	Size(int iw, int ih) :w(iw), h(ih) {}
};

struct Rect {
	Position2 pos;
	Size size;
	Rect() {}
	Rect(const Position2& p, const Size& s) :pos(p), size(s) {}
	Rect(int x, int y, int w, int h) :pos(x, y), size(w, h) {}
	int Left()const {
		return pos.x ;
	}
	int Right()const {
		return pos.x + size.w;
	}
	int Top()const {
		return pos.y;
	}
	int Bottom()const {
		return pos.y + size.h;
	}
	int Width()const {
		return size.w;
	}
	int Height()const {
		return size.h;
	}
};

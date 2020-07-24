#pragma once
#include<cmath>
template<typename T>
struct Vector2D {
	T x;
	T y;
	Vector2D(T inx, T iny) :x(inx), y(iny) {}
	Vector2D() :x(0), y(0) {}
	Vector2D operator+(const Vector2D& rval)const {
		return Vector2D(x + rval.x, y + rval.y);
	}
	Vector2D operator-(const Vector2D& rval)const {
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
	Vector2D operator*(const float scale)const {
		return {x*scale,y*scale};
	}
	float Magnitude()const {
		return hypot(x, y);
	}
	/// <summary>
	/// ベクトルの長さの2乗を返す
	/// </summary>
	/// <returns>ベクトルの長さの2乗</returns>
	float SQMagnitude()const {
		return x*x+y*y;
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
	size_t w=0, h=0;
	Size() {}
	Size(size_t iw, size_t ih) :w(iw), h(ih) {}
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
		return static_cast<int>(pos.x + size.w);
	}
	int Top()const {
		return pos.y;
	}
	int Bottom()const {
		return static_cast<int>(pos.y + size.h);
	}
	size_t Width()const {
		return size.w;
	}
	size_t Height()const {
		return size.h;
	}
};

/// <summary>
/// 円構造体
/// </summary>
struct Circle {
	Position2f center;///<中心
	float radius;///<半径
	Circle() :center{ 0,0 }, radius(0){}
	Circle(const Position2f& c,float r) :center(c), radius(r){}
};

/// <summary>
/// 線分
/// </summary>
struct Segment {
	Position2f start;//起点
	Vector2f vec;//起点からもう１端点までのベクトル
};
/// <summary>
/// カプセル型構造体
/// </summary>
struct Capsule {
	Segment seg;
	float radius;//カプセルの厚み
};


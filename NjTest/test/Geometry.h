#pragma once

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
};



using Vector2 = Vector2D<int>;
using Vector2f = Vector2D<float>;
using Position2 = Vector2;
using Position2f = Vector2f;

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

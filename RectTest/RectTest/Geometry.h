#pragma once

struct Vector2 {
	int x;
	int y;
	Vector2(int inx, int iny) :x(inx), y(iny) {}
	void operator+=(const Vector2& v) {
		x += v.x;
		y += v.y;
	}
	void operator-=(const Vector2& v) {
		x -= v.x;
		y -= v.y;
	}
	void Normalize() {
		x = x != 0 ? x = 1 : 0;
		y = y != 0 ? y = 1 : 0;
	}
	Vector2 Normalized() {
		return Vector2(x != 0 ? x = 1 : 0,
			y = y != 0 ? y = 1 : 0);
	}
	Vector2 operator-()const {
		return Vector2(-x, -y);
	}
};

Vector2 operator*(const Vector2& lval, const Vector2& rval);

struct Size {
	
	int w;
	int h;
	Size(int inw, int inh) :w(inw), h(inh) {}
};
typedef Vector2 Position2 ;
///ãÈå`ç\ë¢ëÃ
struct Rect {
	Position2 pos;
	Size size;
	Rect(int x, int y, int w, int h) :pos(x, y), size(w, h) {}
	int Left()const {
		return pos.x - size.w / 2;
	}
	int Right()const {
		return pos.x + size.w / 2;
	}
	int Top()const {
		return pos.y - size.h / 2;
	}
	int Bottom()const {
		return pos.y + size.h / 2;
	}
	void Draw(unsigned int color);
};

bool IsCollided(const Rect& lval, const Rect& rval,Size& overlappedSize);

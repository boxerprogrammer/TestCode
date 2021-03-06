#pragma once

struct Vector2 {
	float x;
	float y;
	Vector2():x(0),y(0){}
	Vector2(float inx, float iny):x(inx),y(iny){}
	float Magnitude()const;
	void Normalize();
	Vector2 Normalized();
	void operator+=(const Vector2& val);
	void operator-=(const Vector2& val);
	void operator*=(const float scale);
	void operator/=(const float div);

};
Vector2 operator+(const Vector2& lval, const Vector2& rval);
Vector2 operator-(const Vector2& lval, const Vector2& rval);
Vector2 operator*(const Vector2& lval, const float scale);
Vector2 operator/(const Vector2& lval, const float div);

typedef Vector2 Position2;

struct Circle {//円
	Position2 center;//中心座標
	float r;//半径
};

/// <summary>
/// 矩形(長方形)を表す
/// </summary>
struct Rect {
	int left;//左
	int top;//上
	int width;//幅
	int height;//高さ
	int Left()const {
		return left;
	}
	int Top()const {
		return top;
	}
	int Right()const {
		return left+width;
	}
	int Bottom()const {
		return top + height;
	}
	/// <summary>
	/// 中心点の座標を返す
	/// </summary>
	/// <returns>中心点座標</returns>
	Vector2 Center()const {
		return Vector2(left + width / 2, top + height / 2);
	}
};

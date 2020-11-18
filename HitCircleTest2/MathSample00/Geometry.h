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

struct Circle {//�~
	Position2 center;//���S���W
	float r;//���a
};

/// <summary>
/// ��`(�����`)��\��
/// </summary>
struct Rect {
	int left;//��
	int top;//��
	int width;//��
	int height;//����
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
	/// ���S�_�̍��W��Ԃ�
	/// </summary>
	/// <returns>���S�_���W</returns>
	Vector2 Center()const {
		return Vector2(left + width / 2, top + height / 2);
	}
};

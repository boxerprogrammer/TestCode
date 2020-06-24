#pragma once

struct Vector2 {
	int x;
	int y;
	Vector2():x(0),y(0){}
	Vector2(int inx,int iny):x(inx),y(iny){}
};
Vector2 operator+(const Vector2& lval, const Vector2& rval);
Vector2 operator-(const Vector2& lval, const Vector2& rval);

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

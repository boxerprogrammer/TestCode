#pragma once

#include<array>
#include<DxLib.h>

struct Vector2 {
	float x, y;
	/// <summary>
	/// ベクトルの長さ
	/// </summary>
	/// <returns></returns>
	float Length()const;
	/// <summary>
	/// 正規化する
	/// </summary>
	void Normalize();

	/// <summary>
	/// 正規化したベクトルを返す
	/// </summary>
	/// <returns>正規化したベクトル</returns>
	Vector2 Normalied()const;

	Vector2 operator*(float scale)const;
	Vector2 operator/(float div)const;
	void operator*=(float scale);

	Vector2 operator-()const;
	Vector2 operator+(const Vector2& val)const;
	Vector2 operator-(const Vector2& val)const;

	void operator+=(const Vector2& val);
	void operator-=(const Vector2& val);
};

using Position2 = Vector2;

struct Size {
	float w, h;
};

struct Vector3 {
	float x, y, z;
	Vector3() :x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float inx,float iny,float inz) :x(inx), y(iny), z(inz) {}
	Vector3 operator+(const Vector3& v)const;
	Vector3 operator-(const Vector3& v)const;
	VECTOR GetVECTOR()const;
};

using Position3 = Vector3;


struct Cube {
	std::array<Position3,8> positions;
	Position3 center_;
	Cube(const Position3& center=Vector3(0,0,0), float w = 1.0f);
	void Draw(MATRIX mat,uint32_t color=0xffffff);
};

float DegreeToRadian(float degree);




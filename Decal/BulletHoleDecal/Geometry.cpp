#include"Geometry.h"
#include<DxLib.h>
#include<cmath>

void
mgt_lib::Rect::Draw() {
	DxLib::DrawBox(Left() * 2, Top() * 2, Right() * 2, Bottom() * 2, 0xffffffff, false);
}

void
mgt_lib::Rect::Draw(Vector2& offset) {
	DxLib::DrawBox((Left() + offset.x) * 2, (Top() + offset.y) * 2, (Right() + offset.x) * 2, (Bottom() + offset.y) * 2, 0xffffffff, false);
}

void
mgt_lib::Vector2::operator*=(float scale) {
	x *= scale;
	y *= scale;
}

mgt_lib::Vector2
mgt_lib::Vector2::operator*(float scale) {
	return Vector2(x * scale, y * scale);
}

mgt_lib::Vector2 operator+(const mgt_lib::Vector2& va, const mgt_lib::Vector2 vb) {
	return mgt_lib::Vector2(va.x + vb.x, va.y + vb.y);
}

mgt_lib::Vector2 operator-(const mgt_lib::Vector2& va, const mgt_lib::Vector2 vb) {
	return mgt_lib::Vector2(va.x - vb.x, va.y - vb.y);
}

const float
mgt_lib::Vector2::Magnitude()const {
	return hypot(x, y);
}


void
mgt_lib::Vector2::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}


mgt_lib::Vector2
mgt_lib::Vector2::Normalized() {
	float mag = Magnitude();
	return mgt_lib::Vector2(x / mag, y / mag);
}


///内積を返す
float
mgt_lib::Dot(const mgt_lib::Vector2& va, const mgt_lib::Vector2& vb) {
	return va.x * vb.x + va.y * vb.y;
}

///外積を返す
float
mgt_lib::Cross(const mgt_lib::Vector2& va, const mgt_lib::Vector2& vb) {
	return va.x * vb.y - vb.x * va.y;
}

///内積演算子
float
operator*(const mgt_lib::Vector2& va, const mgt_lib::Vector2& vb) {
	return mgt_lib::Dot(va, vb);
}

///外積演算子
float
operator%(const mgt_lib::Vector2& va, const mgt_lib::Vector2& vb) {
	return mgt_lib::Cross(va, vb);
}


void
mgt_lib::Vector2::operator+=(const mgt_lib::Vector2& v) {
	x += v.x;
	y += v.y;
}
void
mgt_lib::Vector2::operator-=(const mgt_lib::Vector2& v) {
	x -= v.x;
	y -= v.y;
}


//-------ここから3Dのターン------
void
mgt_lib::Vector3::operator*=(float scale) {
	x *= scale;
	y *= scale;
	z *= scale;
}

mgt_lib::Vector3
mgt_lib::Vector3::operator*(float scale)const {
	return mgt_lib::Vector3(x * scale, y * scale, z * scale);
}

mgt_lib::Vector3 
mgt_lib::operator+(const mgt_lib::Vector3& va, const mgt_lib::Vector3 vb) {
	return mgt_lib::Vector3(va.x + vb.x, va.y + vb.y, va.z + vb.z);
}

mgt_lib::Vector3 
mgt_lib::operator-(const mgt_lib::Vector3& va, const mgt_lib::Vector3 vb) {
	return mgt_lib::Vector3(va.x - vb.x, va.y - vb.y, va.z - vb.z);
}

float
mgt_lib::Vector3::Magnitude()const {
	return sqrt(SQMagnitude());
}

float
mgt_lib::Vector3::SQMagnitude()const {
	return x * x + y * y + z * z;
}

void
mgt_lib::Vector3::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}


mgt_lib::Vector3
mgt_lib::Vector3::Normalized()const {
	float mag = Magnitude();
	return mgt_lib::Vector3(x / mag, y / mag, z / mag);
}


///内積を返す
float
mgt_lib::Dot(const mgt_lib::Vector3& va, const mgt_lib::Vector3& vb) {
	return va.x * vb.x + va.y * vb.y + va.z * vb.z;
}

///外積を返す
mgt_lib::Vector3
mgt_lib::Cross(const mgt_lib::Vector3& va, const mgt_lib::Vector3& vb) {
	return mgt_lib::Vector3(va.z * vb.y - va.y * vb.z, va.z * vb.x - va.x * vb.z, va.x * vb.y - vb.x * va.y);
}

///内積演算子
float
mgt_lib::operator*(const mgt_lib::Vector3& va, const mgt_lib::Vector3& vb) {
	return mgt_lib::Dot(va, vb);
}

///外積演算子
mgt_lib::Vector3
mgt_lib::operator%(const mgt_lib::Vector3& va, const mgt_lib::Vector3& vb) {
	return mgt_lib::Cross(va, vb);
}

const DxLib::VECTOR& mgt_lib::VGetFromVector3(const Vector3& vec) {
	return VGet(vec.x, vec.y, vec.z);
}
const mgt_lib::Vector3& mgt_lib::GetVector3(const VECTOR& vec) {
	return {vec.x,vec.y,vec.z};
}


void
mgt_lib::Vector3::operator+=(const mgt_lib::Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}
void
mgt_lib::Vector3::operator-=(const mgt_lib::Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}


///当たり判定
bool
mgt_lib::Plane::CheckHit(const mgt_lib::RayLine& ray, mgt_lib::Position3& hitpos, mgt_lib::Vector3& norm) const {
	auto rv = ray.vector;
	rv.Normalize();
	if (rv.Normalized() * normal < 0) {
		float t = (offset - ray.start * normal) / (rv * normal);
		hitpos = ray.start + (rv * t);
		norm = normal.Normalized();
		return true;
	}
	return false;
}

///マテリアルを返します
const mgt_lib::Material&
mgt_lib::Plane::GetMaterial()const {
	return material;
}

///指定座標からの最短距離(球だったら中心との距離-半径。平面だったら垂線の長さ)
float
mgt_lib::Plane::DistanceFromStart(const mgt_lib::Position3& pos) {
	return (normal * pos) - offset;
}
///当たり判定
bool
mgt_lib::Sphere::CheckHit(const mgt_lib::RayLine& ray, mgt_lib::Position3& hitpos, mgt_lib::Vector3& normal)const {
	//レイが正規化済みである前提で…
	//
	//視点から球体中心へのベクトル(視線)を作ります
	mgt_lib::Vector3 tocenter = pos - ray.start;
	//
	//中心から視線への内積をとります⇒ベクトル長
	float dot = mgt_lib::Dot(tocenter, ray.vector);

	if (dot < 0.0f)return false;

	//
	//視線ベクトルとベクトル長をかけて、中心からの垂線下した点を求めます
	Vector3 raydash = ray.vector * dot;

	Vector3 vertical = tocenter - raydash;//垂線ベクトル
	//その垂線の長さが半径以内かどうかでレイが球にあたっているかを
	//判定します
	float sqmag = vertical.SQMagnitude();
	if (sqmag < radius * radius) {
		float w = sqrt(radius * radius - sqmag);
		hitpos = ray.start + ray.vector * (dot - w);//交点
		normal = ray.vector * (dot - w) - tocenter;//中心から交点へのベクトル
		normal.Normalize();
		return true;
	}
	else {
		return false;
	}
}

///マテリアルを返します
const mgt_lib::Material&
mgt_lib::Sphere::GetMaterial()const {
	return material;
}

///指定座標からの最短距離(球だったら中心との距離-半径。平面だったら垂線の長さ)
float
mgt_lib::Sphere::DistanceFromStart(const mgt_lib::Position3& p) {
	return (pos - p).Magnitude() - radius;
}
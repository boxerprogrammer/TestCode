#include"Geometry.h"
#include<DxLib.h>
#include<cmath>
#include<vector>

using namespace std;
namespace {
	VECTOR V2V(const Vector2& v) {
		return VGet(v.x, v.y, 0);
	}
}



float GetAngle2Vector(const Vector2& v1, const Vector2& v2) {
	auto tmpv1 = v1.Normalized();
	auto tmpv2 = v2.Normalized();
	float dot = Dot(tmpv1, tmpv2);
	float cross = Cross(tmpv1, tmpv2);
	auto angle = atan2(cross, dot);
	if (angle < 0) {
		angle += DX_PI_F * 2.0f;
	}
	return angle;
}

Fan::Fan(const Position2& p, const Vector2& inv1, const Vector2& inv2) :
center(p),v1(inv1),v2(inv2){

}
Fan::Fan(const Position2& p, float r, float angle):center(p) {
	v1 = Vector2(r, 0);
	v2 = Vector2(r * cos(angle), r * sin(angle));
}
Fan::Fan(const Position2& p, const Vector2& inv, float angle):
center(p),v1(inv){
	v2 = RotateMat(angle)*v1;
}

float 
Fan::GetAngle()const {
	return GetAngle2Vector(v1, v2);
}

void 
Fan::AddAngle(float angle) {
	float tmpAngle = GetAngle2Vector(v1, v2);

	v2 = RotateMat(tmpAngle+angle) * v1;
}


float 
Fan::Radius()const {
	return v1.Magnitude();
}

void 
Fan::Draw(uint32_t color ) {
	// ２Ｄ描画に使用する頂点データ型
	//struct VERTEX2D
	//{
	//	VECTOR pos;
	//	float rhw;
	//	COLOR_U8 dif;
	//	float u, v;
	//};
	constexpr float min_angle = DX_PI_F / 36.0f;//だいたい5度くらい
	float angle = GetAngle2Vector(v1, v2);

	int triangles_num=(int)ceil(angle / min_angle);
	vector<VERTEX2D> v(3*triangles_num);

	for (auto& e : v) {
		e.rhw = 1.0f;
		e.dif = GetColorU8(color>>16&0xff, color>>8&0xff, color&0xff,255);
		e.u = 0;
		e.v = 0;
	}
	auto vstart = v1;
	auto vend =RotateMat(min_angle)*v1;
	for (size_t i = 0; i < triangles_num; ++i) {
		if (i < triangles_num - 1) {
			v[i * 3 + 0].pos = V2V(center);
			v[i * 3 + 1].pos = V2V(center + vstart);
			v[i * 3 + 2].pos = V2V(center + vend);
		}
		else {
			v[i * 3 + 0].pos = V2V(center);
			v[i * 3 + 1].pos = V2V(center + vstart);
			v[i * 3 + 2].pos = V2V(center + v2);
		}
		vstart = vend;
		vend = RotateMat(min_angle) * vstart;
	}
	// ２Ｄポリゴンを描画する
	//DrawPrimitive2D(v.data(), v.size(), DX_PRIMTYPE_TRIANGLEFAN, DX_NONE_GRAPH,true);
	DrawPolygon2D(v.data(), v.size()/3,DX_NONE_GRAPH, true);

	Position2 p = center + v1;
	DrawLine(center.x, center.y, p.x, p.y, 0xff8888);
	p = center + v2;
	DrawLine(center.x, center.y, p.x, p.y, 0xff8888);
}


void
Rect::Draw() {
	DxLib::DrawBox(Left()*2, Top()*2, Right()*2, Bottom()*2, 0xffffffff, false);
}

void
Rect::Draw(const Vector2& offset) {
	DxLib::DrawBox((Left()+offset.x)*2, (Top()+offset.y)*2, (Right()+offset.x)*2, (Bottom()+offset.y)*2, 0xffffffff, false);
}

void
Vector2::operator*=(float scale) {
	x *= scale;
	y *= scale;
}

Vector2
Vector2::operator*(float scale)const {
	return Vector2(x*scale, y*scale);
}

Vector2 operator+(const Vector2& va, const Vector2 vb) {
	return Vector2(va.x + vb.x, va.y + vb.y);
}

Vector2 operator-(const Vector2& va, const Vector2 vb){
	return Vector2(va.x - vb.x, va.y - vb.y);
}

float
Vector2::Magnitude()const {
	return hypot(x, y);
}


void 
Vector2::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}


Vector2
Vector2::Normalized()const {
	float mag = Magnitude();
	return Vector2(x / mag,	y /mag);
}

float 
Vector2::Square()const {
	return x * x + y * y;
}

///内積を返す
float
Dot(const Vector2& va, const Vector2& vb) {
	return va.x*vb.x + va.y*vb.y;
}

///外積を返す
float
Cross(const Vector2& va, const Vector2& vb) {
	return va.x*vb.y - vb.x*va.y;
}

///内積演算子
float 
operator*(const Vector2& va, const Vector2& vb) {
	return Dot(va, vb);
}

///外積演算子
float 
operator%(const Vector2& va, const Vector2& vb) {
	return Cross(va, vb);
}


void 
Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
}
void 
Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
}
bool
Vector2::operator==(const Vector2& v)const {
	return x == v.x && y == v.y;
}

bool
Vector2::operator!=(const Vector2& v)const {
	return x != v.x || y != v.y;
}
const Vector2 Vector2::Zero = { 0.0f,0.0f };

///２つの行列の乗算を返す
///@param lmat 左辺値(行列)
///@param rmat 右辺値(行列)
///@attention 乗算の順序に注意してください
Matrix
MultipleMat(const Matrix& lmat, const Matrix& rmat) {
	Matrix ret = {};

	for (int k = 0; k <= 2; ++k) {
		for (int j = 0; j <= 2; ++j) {
			for (int i = 0; i <= 2; ++i) {
				ret.m[k][j] += lmat.m[k][i] * rmat.m[i][j];
			}
		}
	}

	return ret;

}

///ベクトルに対して行列乗算を適用し、結果のベクトルを返す
///@param mat 行列
///@param vec ベクトル
Vector2
MultipleVec(const Matrix& mat, const Vector2& vec) {
	Vector2 ret = {};
	ret.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2];
	ret.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2];
	return ret;
}


///単位行列を返す
Matrix IdentityMat() {
	Matrix ret = {};
	ret.m[0][0] = ret.m[1][1] = ret.m[2][2] = 1;
	return ret;
}

///平行移動行列を返す
///@param x X方向平行移動量
///@param y Y方向平行移動量
Matrix TranslateMat(float x, float y) {
	Matrix ret = {};
	ret = IdentityMat();
	ret.m[0][2] = x;
	ret.m[1][2] = y;
	return ret;
}

///回転行列を返す
///@param angle 回転角度
Matrix RotateMat(float angle) {
	Matrix ret = {};
	ret = IdentityMat();
	ret.m[0][0] = cos(angle);
	ret.m[0][1] = -sin(angle);
	ret.m[1][0] = sin(angle);
	ret.m[1][1] = cos(angle);
	return ret;
}

Matrix operator*(const Matrix& lmat, const Matrix& rmat) {
	return MultipleMat(lmat, rmat);
}
Vector2 operator*(const Matrix& mat, const Vector2& vec) {
	return MultipleVec(mat, vec);
}

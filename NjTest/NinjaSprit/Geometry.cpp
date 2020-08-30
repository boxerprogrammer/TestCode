#include"Geometry.h"
#include<algorithm>
#include<DxLib.h>

const Vector2D<float> Vector2D<float>::UP(0.0f, -1.0f);
const Vector2D<float> Vector2D<float>::DOWN(0.0f, 1.0f);
const Vector2D<float> Vector2D<float>::LEFT(-1.0f, 0.0f);
const Vector2D<float> Vector2D<float>::RIGHT(1.0f, 0.0f);
const Vector2D<float> Vector2D<float>::ZERO(0.0f,0.0f);
const Vector2D<float> Vector2D<float>::NIL(NAN, NAN);
const Segment Segment::ZERO(Vector2f::ZERO, Vector2f::ZERO);
const Segment Segment::NIL(Vector2f::NIL, Vector2f::NIL);

using namespace std;

namespace DxLib {
	const tagVECTOR& V2V(const Vector2f& v) {
		return VGet(v.x, v.y, 0);
	}
}

float GetAngle2Vector(const Vector2f& v1, const Vector2f& v2) {
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


Vector2f Vector2ToVector2F(const Vector2& v) {
	return { static_cast<float>(v.x),static_cast<float>(v.y) };
}
Vector2 Vector2FToVector2(const Vector2f& v) {
	return { static_cast<int>(v.x),static_cast<int>(v.y) };
}


float Dot(const Vector2f& va, const Vector2f& vb) {
	return va.x * vb.x + va.y * vb.y;
}
float Cross(const Vector2f& va, const Vector2f& vb) {
	return va.x * vb.y - va.y * vb.x;
}

Position2f 
Segment::End() {
	return start + vec;
}

bool 
Segment::IsNil() {
	return vec.IsNil();
}

Vector2f
Clamp(const Vector2f& val,const Vector2f& minVal, const Vector2f& maxVal) {
	return { std::clamp(val.x,minVal.x,maxVal.x),std::clamp(val.y,minVal.y,maxVal.y) };
}





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
Vector2f
MultipleVec(const Matrix& mat, const Vector2f& vec) {
	Vector2f ret = {};
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
Vector2f operator*(const Matrix& mat, const Vector2f& vec) {
	return MultipleVec(mat, vec);
}
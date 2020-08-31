#include"Geometry.h"
#include<algorithm>
#include<DxLib.h>
#include<vector>
#include"Arithmetic.h"

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

/// <summary>
/// v1とv2の角度を測る。必ずv1→v2は時計回りであること
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns></returns>
float GetAngle2Vector(const Vector2f& v1, const Vector2f& v2) {
	auto tmpv1 = v1.Normalized();
	auto tmpv2 = v2.Normalized();
	float dot = Dot(tmpv1, tmpv2);//内積(cosθ)
	float cross = Cross(tmpv1, tmpv2);//外積(sinθ)
	auto angle = atan2(cross, dot);
	if (angle < 0.0f) {
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
	Matrix ret = IdentityMat();
	ret.m[0][2] = x;
	ret.m[1][2] = y;
	return ret;
}

///回転行列を返す
///@param angle 回転角度
Matrix RotateMat(float angle) {
	Matrix ret = IdentityMat();
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


FanShape::FanShape(const Position2f& incenter, const Vector2f& inV1, const Vector2f& inV2):
center(incenter),
v1(inV1),
v2(inV2){

}
FanShape::FanShape(const Position2f& incenter, const Vector2f& inV1, float angle):
	center(incenter),
	v1(inV1)
{
	SetAngle2(angle);
}

void 
FanShape::SetAngle1(float angle) {

}
void 
FanShape::SetAngle2(float angle) {
	v2 = RotateMat(angle) * v1;
}

void 
FanShape::AddAngle1(float angle) {

}
void 
FanShape::AddAngle2(float angle) {

}

/// <summary>
/// 扇形を描画する
/// </summary>
void 
FanShape::Draw(int graphH,float amp) {
	constexpr float step_angle = (DX_PI_F / 18.0f);
	float angle_range = GetAngle2Vector(v1, v2);
	
	size_t triangle_num = static_cast<size_t>(angle_range / step_angle);

	float rem=fmod(angle_range, step_angle);
	std::vector<VERTEX2D> vertices(3*(rem > 0.0f ? triangle_num + 1 : triangle_num));

	for (auto& v : vertices) {
		v.dif.r = 0x88;
		v.dif.g = 0xaa;
		v.dif.b = 0xff;
		v.dif.a = 0xff;
		v.u = 0.0f;
		v.v = 0.0f;
		v.rhw = 1.0;
	}
	Vector2f tmpV1 = v1;
	Vector2f tmpV2 = RotateMat(step_angle) * tmpV1;
	float ampX = amp / 800.0f;
	float ampY = amp / 600.0f;
	for (size_t i = 0; i < triangle_num; ++i) {
		vertices[i * 3 + 0].pos = V2V(center);
		vertices[i * 3 + 0].u = vertices[i * 3 + 0].pos.x / 800.0f;
		vertices[i * 3 + 0].v = vertices[i * 3 + 0].pos.y / 600.0f;
		vertices[i * 3 + 1].pos = V2V(center+tmpV1);
		vertices[i * 3 + 1].u = sin(float(i*i) * step_angle) * ampX + vertices[i * 3 + 1].pos.x / 800.0f;
		vertices[i * 3 + 1].v = cos(float(i*i) * step_angle) * ampY + vertices[i * 3 + 1].pos.y / 600.0f;
		vertices[i * 3 + 2].pos = V2V(center+tmpV2);
		vertices[i * 3 + 2].u = sin(float(i + 1) * float(i + 1)*step_angle) * ampX + vertices[i * 3 + 2].pos.x / 800.0f;
		vertices[i * 3 + 2].v = cos(float(i + 1) * float(i + 1) * step_angle) * ampY + vertices[i * 3 + 2].pos.y / 600.0f;
		
		tmpV1 = tmpV2;
		tmpV2 = RotateMat(step_angle) * tmpV1;
	}
	tmpV2 = v2;
	if (rem > 0.0f) {
		auto idx = triangle_num;
		vertices[idx * 3 + 0].pos = V2V(center);
		vertices[idx * 3 + 0].u = vertices[idx * 3 + 0].pos.x / 800.0f;
		vertices[idx * 3 + 0].v = vertices[idx * 3 + 0].pos.y / 600.0f;
		vertices[idx * 3 + 1].pos = V2V(center + tmpV1);
		vertices[idx * 3 + 1].u = sin(float(idx)* float(idx) * step_angle) * ampX +vertices[idx * 3 + 1].pos.x / 800.0f;
		vertices[idx * 3 + 1].v = cos(float(idx)* float(idx) * step_angle) * ampY+ vertices[idx * 3 + 1].pos.y / 600.0f;
		vertices[idx * 3 + 2].pos = V2V(center + tmpV2);
		vertices[idx * 3 + 2].u = sin(float((idx+1)*(idx+1)) * step_angle) * ampX+ vertices[idx * 3 + 2].pos.x / 800.0f;
		vertices[idx * 3 + 2].v = cos(float((idx + 1) * (idx + 1)) * step_angle) * ampY+ vertices[idx * 3 + 2].pos.y / 600.0f;
		
		tmpV1 = tmpV2;
		tmpV2 = RotateMat(step_angle) * tmpV1;
	}

	if (graphH == -1) {
		graphH = DX_NONE_GRAPH;
	}
	DxLib::DrawPrimitive2D(vertices.data(), vertices.size(),
		DX_PRIMTYPE_TRIANGLELIST, graphH, false);

	DrawLineAA(center.x, center.y,
		(center + v1).x, (center + v1).y, 0xff0000, 3.0f);
	DrawLineAA(center.x, center.y,
		(center + v2).x, (center + v2).y, 0xff0000, 3.0f);
}
/// <summary>
/// 半径を返す
/// </summary>
/// <returns>半径</returns>
float 
FanShape::Radius() {
	return 0.0f;
}
/// <summary>
/// V1→V2の角度を返す
/// </summary>
/// <returns>V1→V2の角度</returns>
float 
FanShape::GetAngle() {
	return 0.0f;
}


SlashShape::SlashShape(const Position2f& incenter, const Vector2f& inV1, const Vector2f& inV2) :
	center(incenter),
	v1(inV1),
	v2(inV2) {

}
SlashShape::SlashShape(const Position2f& incenter, const Vector2f& inV1, float angle) :
	center(incenter),
	v1(inV1)
{
	SetAngle2(angle);
}

void
SlashShape::SetAngle1(float angle) {

}
void
SlashShape::SetAngle2(float angle) {
	v2 = RotateMat(angle) * v1;
}

void
SlashShape::AddAngle1(float angle) {

}
void
SlashShape::AddAngle2(float angle) {

}

/// <summary>
/// 斬撃を描画する
/// </summary>
void
SlashShape::Draw(int graphH, float amp) {
	constexpr float step_angle = (DX_PI_F / 18.0f);
	float angle_range = GetAngle2Vector(v1, v2);

	size_t triangles_num = (size_t)ceil(angle_range / step_angle);
	triangles_num++;
	float rem = fmod(angle_range, step_angle);
	std::vector<VERTEX2D> vertices(2 * triangles_num);

	for (auto& v : vertices) {
		v.dif.r = 0x88;
		v.dif.g = 0xaa;
		v.dif.b = 0xff;
		v.dif.a = 0xff;
		v.u = 0.0f;
		v.v = 0.0f;
		v.rhw = 1.0;
	}
	
	Vector2f tmpV = v1;
	float r = tmpV.Magnitude();
	float inr = r;
	float step_r=r/(float)(triangles_num);
	float ampX = amp / 800.0f;
	float ampY = amp / 600.0f;
	float curAngle = 0.0f;
	for (size_t i = 0; i < triangles_num-1; ++i) {
		auto ratio = curAngle / angle_range;
		inr = r * (1 - ratio);
		auto nv = tmpV.Normalized();
		vertices[i * 2 + 0].pos = V2V(center + nv*inr);
		vertices[i * 2 + 0].u = sin(float(i * i) * step_angle) * ampX + vertices[i * 2 + 0].pos.x / 800.0f;
		vertices[i * 2 + 0].v = cos(float(i * i) * step_angle) * ampY + vertices[i * 2 + 0].pos.y / 600.0f;
		vertices[i * 2 + 1].pos = V2V(center + nv*r);
		vertices[i * 2 + 1].u = sin(float(i + 1) * float(i + 1) * step_angle) * ampX + vertices[i * 2 + 1].pos.x / 800.0f;
		vertices[i * 2 + 1].v = cos(float(i + 1) * float(i + 1) * step_angle) * ampY + vertices[i * 2 + 1].pos.y / 600.0f;
		
		
		curAngle += step_angle;
		tmpV =  RotateMat(step_angle) * tmpV;
		//inr = SaturateSubtract(inr,step_r,0.0f);
	}
	tmpV = v2;
	if (rem > 0.0f) {
		auto idx = (triangles_num-1)*2;
		auto nv = tmpV.Normalized();
		vertices[idx].pos = V2V(center);
		vertices[idx].u = sin(float(idx) * float(idx) * step_angle) * ampX + vertices[idx].pos.x / 800.0f;
		vertices[idx].v = cos(float(idx) * float(idx) * step_angle) * ampY + vertices[idx].pos.y / 600.0f;
		vertices[idx+ 1].pos = V2V(center + nv * r);
		vertices[idx+ 1].u = sin(float((idx + 1) * (idx + 1)) * step_angle) * ampX + vertices[idx  + 1].pos.x / 800.0f;
		vertices[idx+ 1].v = cos(float((idx + 1) * (idx + 1)) * step_angle) * ampY + vertices[idx + 1].pos.y / 600.0f;
	}

	if (graphH == -1) {
		graphH = DX_NONE_GRAPH;
	}
	DxLib::DrawPrimitive2D(vertices.data(), vertices.size(),
		DX_PRIMTYPE_TRIANGLESTRIP, graphH, false);

	DrawLineAA(center.x, center.y,
		(center + v1).x, (center + v1).y, 0xff0000, 3.0f);
	DrawLineAA(center.x, center.y,
		(center + v2).x, (center + v2).y, 0xff0000, 3.0f);
}
/// <summary>
/// 半径を返す
/// </summary>
/// <returns>半径</returns>
float
SlashShape::Radius() {
	return 0.0f;
}
/// <summary>
/// V1→V2の角度を返す
/// </summary>
/// <returns>V1→V2の角度</returns>
float
SlashShape::GetAngle() {
	return 0.0f;
}
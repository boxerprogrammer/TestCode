#pragma once
#include<cmath>
#include<algorithm>
#include<cassert>



template<typename T>
struct Vector2D {
	T x;//x座標
	T y;//y座標
	Vector2D(T inx, T iny) :x(inx), y(iny) {}
	Vector2D() :x(0), y(0) {}
	/// <summary>
	/// 加算
	/// </summary>
	/// <param name="rval">右辺値</param>
	/// <returns>加算結果</returns>
	Vector2D operator+(const Vector2D& rval)const {
		return Vector2D(x + rval.x, y + rval.y);
	}
	/// <summary>
	/// 減算
	/// </summary>
	/// <param name="rval">右辺値</param>
	/// <returns>減算結果</returns>
	Vector2D operator-(const Vector2D& rval)const {
		return Vector2D(x - rval.x, y - rval.y);
	}
	Vector2D operator-()const {
		return Vector2D(-x, -y);
	}
	/// <summary>
	/// 自分に指定値だけ加算する
	/// </summary>
	/// <param name="v">加算値</param>
	void operator+=(const Vector2D& v) {
		x += v.x;
		y += v.y;
	}
	/// <summary>
	/// 自分にして位置だけ減算する
	/// </summary>
	/// <param name="v">減算値</param>
	void operator-=(const Vector2D& v) {
		x -= v.x;
		y -= v.y;
	}
	/// <summary>
	/// スカラー倍する
	/// </summary>
	/// <param name="scale">倍率</param>
	void operator*=(float scale) {
		x *= scale;
		y *= scale;
	}
	/// <summary>
	/// スカラー倍したものを返す
	/// </summary>
	/// <param name="scale">倍率</param>
	/// <returns>スケーリングした結果の値</returns>
	Vector2D operator*(const float scale)const {
		return {x*scale,y*scale};
	}
	/// <summary>
	/// ベクトルに対するスカラー値除算
	/// </summary>
	/// <param name="div">除算値</param>
	/// <returns>除算された結果の値</returns>
	Vector2D operator/(const float div)const {
		return { x / div,y / div };
	}
	/// <summary>
	/// ベクトルの大きさを返す
	/// </summary>
	/// <returns>ベクトルの大きさ</returns>
	float Magnitude()const {
		return hypot(x, y);
	}
	/// <summary>
	/// ベクトルが無効かどうか
	/// </summary>
	/// <returns>true:無効である false:有効である</returns>
	bool IsNil()const {
		return isnan(x) || isnan(y);
	}
	/// <summary>
	/// 等しいかどうか
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>true:等しい false:等しくない</returns>
	bool operator==(const Vector2D<T>& v)const {
		assert(!IsNil());
		return x == v.x && y == v.y;
	}
	/// <summary>
	/// 不等かどうか
	/// </summary>
	/// <param name="v">右辺値</param>
	/// <returns>true:等しくない false:等しい</returns>
	bool operator!=(const Vector2D<T>& v) {
		assert(!IsNil());
		return !(x == v.x && y == v.y);
	}
	/// <summary>
	/// ベクトルの長さの2乗を返す
	/// </summary>
	/// <returns>ベクトルの長さの2乗</returns>
	float SQMagnitude()const {
		return x*x+y*y;
	}
	/// <summary>
	/// ベクトルを正規化する(大きさを１にする)
	/// </summary>
	void Normalize() {
		float mag = Magnitude();
		assert(mag != 0.0f);
		x/=mag;
		y /= mag;
	}
	/// <summary>
	/// 正規化済みのベクトルを返す
	/// </summary>
	/// <returns>正規化ベクトル</returns>
	Vector2D Normalized()const {
		float mag = Magnitude();
		assert(mag != 0.0f);
		return {x/mag,y/mag};
	}
	/// <summary>
	/// X軸からの角度を返す
	/// </summary>
	/// <returns>X軸を基準とした角度</returns>
	float AngleFromX()const {
		return atan2(y, x);
	}
	static const Vector2D<float> ZERO;
	static const Vector2D<float> UP;
	static const Vector2D<float> DOWN;
	static const Vector2D<float> LEFT;
	static const Vector2D<float> RIGHT;
	static const Vector2D<float> NIL;
	
};


using Vector2 = Vector2D<int>;
using Vector2f = Vector2D<float>;
using Position2 = Vector2;
using Position2f = Vector2f;

/// <summary>
/// 整数型のベクタから浮動小数点型のベクタに変換
/// </summary>
/// <param name="vn">整数型ベクタ</param>
/// <returns>浮動小数型ベクタ</returns>
Vector2f Vector2ToVector2F(const Vector2& vn);
/// <summary>
/// 浮動小数型のベクタを整数型のベクタに変換
/// </summary>
/// <param name="vf">浮動小数型ベクタ</param>
/// <returns>整数型ベクタ</returns>
Vector2 Vector2FToVector2(const Vector2f& vf);

/// <summary>
/// 汎用クランプ関数(値をminからmaxの間に収める)
/// </summary>
/// <typeparam name="T">組み込み型</typeparam>
/// <param name="value">値</param>
/// <param name="minValue">最小値</param>
/// <param name="maxValue">最大値</param>
/// <returns>クランプ結果値</returns>
template<typename T>
inline T Clamp(const T& value, const T& minValue, const T& maxValue) {
	return std::min(std::max(value, minValue), maxValue);
}

/// <summary>
/// ベクトルクランプ関数(要素値をminからmaxの間に収める)
/// </summary>
/// <typeparam name="T">組み込み型</typeparam>
/// <param name="value">値</param>
/// <param name="minValue">最小値ベクトル</param>
/// <param name="maxValue">最大値ベクトル</param>
/// <returns>クランプ結果値</returns>
template<>
inline Vector2f Clamp(const Vector2f& val, const Vector2f& minVal, const Vector2f& maxVal) {
	return { Clamp(val.x,minVal.x,maxVal.x),Clamp(val.y,minVal.y,maxVal.y) };
}

/// <summary>
/// 内積を返す
/// </summary>
/// <param name="va">左項</param>
/// <param name="vb">右項</param>
/// <returns>内積値</returns>
float Dot(const Vector2f& va,const Vector2f& vb);

/// <summary>
/// 外積を返す
/// </summary>
/// <param name="va">左項</param>
/// <param name="vb">右項</param>
/// <returns>外積値</returns>
float Cross(const Vector2f& va, const Vector2f& vb);

/// <summary>
/// サイズを定義
/// 中身はsize_tが２つ(変換に気を付けよう)
/// </summary>
struct Size {
	size_t w=0, h=0;
	Size() {}
	Size(size_t iw, size_t ih) :w(iw), h(ih) {}
	/// <summary>
	/// Size(size_t型２つ)からVector2(int型２つ)に変換
	/// </summary>
	/// <returns>整数ベクタ</returns>
	Vector2 ToIntVector() {
		return { static_cast<int>(w),static_cast<int>(h) };
	}
};

/// <summary>
/// 矩形(整数型)を表す
/// </summary>
struct Rect {
	Position2 pos;//左上座標
	Size size;//幅、高さ
	Rect() {}
	/// <summary>
	/// 左上座標とサイズで初期化
	/// </summary>
	/// <param name="p">左上座標</param>
	/// <param name="s">サイズ</param>
	Rect(const Position2& p, const Size& s) :pos(p), size(s) {}
	/// <summary>
	/// 左、上、幅、高さで初期化
	/// </summary>
	/// <param name="x">左</param>
	/// <param name="y">上</param>
	/// <param name="w">幅</param>
	/// <param name="h">高</param>
	Rect(int x, int y, size_t w, size_t h) :pos(x, y), size(w, h) {}
	/// <summary>
	/// 左座標を返す(整数型)
	/// </summary>
	/// <returns>左座標</returns>
	int Left()const {
		return pos.x ;
	}
	/// <summary>
	/// 右座標を返す(整数型)
	/// </summary>
	/// <returns>右座標</returns>
	int Right()const {
		return static_cast<int>(pos.x + size.w);
	}
	/// <summary>
	/// 上座標を返す(整数型)
	/// </summary>
	/// <returns>上座標</returns>
	int Top()const {
		return pos.y;
	}
	/// <summary>
	/// 下座標を返す(整数型)
	/// </summary>
	/// <returns>下座標</returns>
	int Bottom()const {
		return static_cast<int>(pos.y + size.h);
	}
	/// <summary>
	/// 左座標を返す(浮動少数型)
	/// </summary>
	/// <returns>左座標</returns>
	float LeftF()const {
		return static_cast<float>(pos.x);
	}
	/// <summary>
	/// 右座標を返す(浮動少数型)
	/// </summary>
	/// <returns>左座標</returns>
	float RightF()const {
		return static_cast<float>(pos.x + size.w);
	}
	/// <summary>
	/// 上座標を返す(浮動少数型)
	/// </summary>
	/// <returns>上座標</returns>
	float TopF()const {
		return static_cast<float>(pos.y);
	}
	/// <summary>
	/// 下座標を返す(浮動少数型)
	/// </summary>
	/// <returns>下座標</returns>
	float BottomF()const {
		return static_cast<float>(pos.y+ size.h);
	}
	/// <summary>
	/// 幅を返す
	/// </summary>
	/// <returns>幅</returns>
	size_t Width()const {
		return size.w;
	}
	/// <summary>
	/// 高さを返す
	/// </summary>
	/// <returns>高さ</returns>
	size_t Height()const {
		return size.h;
	}
	/// <summary>
	/// 中心点を返す(整数型)
	/// </summary>
	/// <returns>中心点</returns>
	Vector2 Center()const {
		return { pos.x + static_cast<int>(size.w / 2),
			static_cast<int>(pos.y + size.h / 2) };
	}
	/// <summary>
	/// 中心点を返す(浮動小数型)
	/// </summary>
	/// <returns>中心点</returns>
	Vector2f CenterF()const {
		return { static_cast<float>(pos.x + size.w / 2),
			static_cast<float>(pos.y + size.h / 2) };
	}
};

/// <summary>
/// 円構造体
/// </summary>
struct Circle {
	Position2f center;///<中心
	float radius;///<半径
	Circle() :center{ 0,0 }, radius(0){}
	Circle(const Position2f& c,float r) :center(c), radius(r){}
};

/// <summary>
/// 線分
/// </summary>
struct Segment {
	Position2f start;//起点
	Vector2f vec;//起点からもう１端点までのベクトル
	Segment() {}
	Segment(const Position2f& s, const Position2f& v) :start(s), vec(v) {}
	Segment(float x, float y, float vx, float vy) :start(x, y), vec(vx, vy) {}
	/// <summary>
	/// 終点座標を返す
	/// </summary>
	/// <returns>終点座標</returns>
	Position2f End();
	static const Segment ZERO;///<ゼロ線分
	static const Segment NIL;///<無効線分
	/// <summary>
	/// 無効な線分かどうか
	/// </summary>
	/// <returns>true:無効 false:有効</returns>
	bool IsNil();
};
/// <summary>
/// カプセル型構造体
/// </summary>
struct Capsule {
	Segment seg;//線分
	float radius;//カプセルの厚み
};

namespace DxLib {
	struct tagVECTOR;
	const tagVECTOR& V2V(const Vector2f& v);
}
float GetAngle2Vector(const Vector2f& v1, const Vector2f& v2);

/// <summary>
/// 行列
/// </summary>
struct Matrix {
	float m[3][3];
};

///単位行列を返す
Matrix IdentityMat();

///平行移動行列を返す
///@param x X方向平行移動量
///@param y Y方向平行移動量
Matrix TranslateMat(float x, float y);

///回転行列を返す
///@param angle 回転角度
Matrix RotateMat(float angle);

///２つの行列の乗算を返す
///@param lmat 左辺値(行列)
///@param rmat 右辺値(行列)
///@attention 乗算の順序に注意してください
Matrix MultipleMat(const Matrix& lmat, const Matrix& rmat);

///ベクトルに対して行列乗算を適用し、結果のベクトルを返す
///@param mat 行列
///@param vec ベクトル
Vector2f MultipleVec(const Matrix& mat, const Vector2f& vec);

Matrix operator*(const Matrix& lmat, const Matrix& rmat);
Vector2f operator*(const Matrix& mat, const Vector2f& vec);

/// <summary>
/// 扇形構造体
/// </summary>
///<attention>
///不変条件
///v1→v2は必ず時計回りであること
///v1とv2の大きさは必ず同じであること
///</attention>
struct FanShape {
	Position2f center;///<中心座標
	Vector2f v1;///<扇形の端点①までのベクトル
	Vector2f v2;///<扇形の端点②までのベクトル
	FanShape(const Position2f& incenter, const Vector2f& inV1, const Vector2f& inV2);
	FanShape(const Position2f& incenter, const Vector2f& inV1, float angle);
	void AddAngle1(float angle);
	void AddAngle2(float angle);
	void SetAngle1(float angle);
	void SetAngle2(float angle);

	/// <summary>
	/// 扇形を描画する
	/// </summary>
	void Draw(int graphH=-1, float amp=0.0f);
	/// <summary>
	/// 半径を返す
	/// </summary>
	/// <returns>半径</returns>
	float Radius();
	/// <summary>
	/// V1→V2の角度を返す
	/// </summary>
	/// <returns>V1→V2の角度</returns>
	float GetAngle();
	
};

/// <summary>
/// 斬撃軌跡構造体
/// </summary>
///<attention>
///不変条件
///v1→v2は必ず時計回りであること
///v1とv2の大きさは必ず同じであること
///</attention>
struct SlashShape {
	Position2f center;///<中心座標
	Vector2f v1;///<端点①までのベクトル
	Vector2f v2;///<端点②までのベクトル
	SlashShape(const Position2f& incenter, const Vector2f& inV1, const Vector2f& inV2);
	SlashShape(const Position2f& incenter, const Vector2f& inV1, float angle);
	void AddAngle1(float angle);
	void AddAngle2(float angle);
	void SetAngle1(float angle);
	void SetAngle2(float angle);

	/// <summary>
	/// スラッシュエフェクトを描画する
	/// </summary>
	void Draw(int graphH = -1, float amp = 0.0f,int psH=-1,int normalH=-1);
	/// <summary>
	/// 半径を返す
	/// </summary>
	/// <returns>半径</returns>
	float Radius();
	/// <summary>
	/// V1→V2の角度を返す
	/// </summary>
	/// <returns>V1→V2の角度</returns>
	float GetAngle();

};

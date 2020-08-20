#pragma once


//サイズを表す構造体
struct Size {
	float w;//幅
	float h;//高さ
};

//座標を表す構造体
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///ベクトルの大きさを返します
	float Magnitude()const;
	///ベクトルの大きさの2乗を返します
	float Square()const;
	
	///正規化(大きさを１に)します
	void Normalize();

	///正規化ベクトルを返します
	Vector2 Normalized()const;

	void operator+=(const Vector2& v);
	void operator-=(const Vector2& v);
	void operator*=(float scale);
	Vector2 operator*(float scale)const;
	Vector2 operator-() {
		return Vector2(-x, -y);
	}
};

Vector2 operator+(const Vector2& va, const Vector2 vb);
Vector2 operator-(const Vector2& va, const Vector2 vb);

///内積を返す
float Dot(const Vector2& va, const Vector2& vb);

///外積を返す
float Cross(const Vector2& va, const Vector2& vb);

///内積演算子
float operator*(const Vector2& va, const Vector2& vb);

///外積演算子
float operator%(const Vector2& va, const Vector2& vb);

//とりあえず「座標」って意味だとベクタより
//Positionのほうがよくね？って理由でこの名前
typedef Vector2 Position2;

struct Circle {
	Position2 center; //中心座標
	float radius;//半径
	Circle() :radius(0), center(0, 0) {}
	Circle(float r,const Position2& p) :radius(r), center(p) {}
	Circle(const Position2& p,float r) :radius(r), center(p) {}
};

/// <summary>
/// 扇形構造体
/// 不変条件：v1→v2は時計回りである事
///          :v1の大きさとv2の大きさは同じである事
/// </summary>
struct Fan {
	Position2 center; //中心座標
	Vector2 v1; //扇形の端ベクトル①
	Vector2 v2; //扇形の端ベクトル②
	Fan() :center(0, 0),v1(0,0),v2(0,0) {}//無効です
	Fan(const Position2& p, const Vector2& inv1, const Vector2& inv2);
	Fan(const Position2& p, float r, float angle);
	Fan(const Position2& p, const Vector2& inv, float angle);
	void Draw(unsigned int color=0xffffff);
	float Radius()const;
	void AddAngle(float angle);
	float GetAngle()const;
};

float GetAngle2Vector(const Vector2& v1, const Vector2& v2);

struct Rect {
	Position2 pos; //中心座標
	int w, h;//幅,高さ
	Rect() : pos(0, 0), w(0), h(0) {}
	Rect(float x, float y, int inw, int inh) :
		pos(x, y), w(inw), h(inh) {}
	Rect(const Position2& inpos, int inw, int inh) :
		pos(inpos), w(inw), h(inh)
	{}
	void SetCenter(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	void SetCenter(const Position2& inpos) {
		pos.x = inpos.x;
		pos.y = inpos.y;
	}
	const Vector2& Center() const{
		return pos;
	}
	float Left()const { return pos.x - w / 2; }
	float Top()const { return pos.y - h / 2; }
	float Right()const { return pos.x + w / 2; }
	float Bottom()const { return pos.y + h / 2; }
	float Width()const { return w; }
	float Height()const { return h; }
	void Draw();//自分の矩形を描画する
	void Draw(const Vector2& offset);//自分の矩形を描画する(オフセット付き)
};

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
Vector2 MultipleVec(const Matrix& mat, const Vector2& vec);

Matrix operator*(const Matrix& lmat, const Matrix& rmat);
Vector2 operator*(const Matrix& mat, const Vector2& vec);

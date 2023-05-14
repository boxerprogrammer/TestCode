#pragma once

/// <summary>
/// ベクトル構造体
/// プレイヤーの座標や速度を表すためのもの
/// </summary>
struct Vector2 {
	float x;
	float y;

	Vector2() :x(0), y(0) {}
	Vector2(float inx, float iny) :x(inx), y(iny) {};

	//加算、減算
	Vector2 operator+(const Vector2& rval)const {
		return { x+rval.x , y+rval.y};
	}
	Vector2 operator-(const Vector2& rval)const {
		return { x - rval.x , y - rval.y };
	}
	void operator+=(const Vector2& rval);
	void operator-=(const Vector2& rval);

	//スカラー倍,スカラー割り
	Vector2 operator*(float scale)const;
	Vector2 operator/(float div)const;

	void operator*=(float scale);
	void operator/=(float div);

	//ベクター逆転(-)
	Vector2 operator-()const;

	/// ベクトルの大きさを返す
	float Length()const;

	/// ベクトルの大きさの2乗を返す
	float SQLength()const;

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	void Normalize();

	/// <summary>
	/// 正規化したベクトルを返す
	/// </summary>
	Vector2 GetNormalized()const;

};

//座標を表す別名
using Position2 = Vector2;

//サイズをまとめる構造体
struct Size {
	int w;///幅
	int h;///高さ
};

/// <summary>
/// 矩形構造体
/// </summary>
struct Rect {
	Position2 center;//矩形の中心
	Size size;//矩形の幅、高さ

	Rect();
	Rect(const Position2& c, const Size& sz);

	/// <summary>
	/// 左上右下を指定することで矩形を構築
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="top">上</param>
	/// <param name="right">右</param>
	/// <param name="bottom">下</param>
	void SetLTRB(int left,int top ,int right,int bottom);

	//左
	int Left()const;
	//上
	int Top()const;
	//右
	int Right()const;
	//下
	int Bottom()const;

	const Position2& GetCenter()const;
	const Size& GetSize()const;

	/// <summary>
	/// デバッグ用矩形表示
	/// </summary>
	void Draw(unsigned int Color = 0xfffff)const;
	
	/// <summary>
	/// もう一つの矩形と当たったかどうか
	/// </summary>
	/// <param name="rc">矩形</param>
	/// <returns>true 当たった false はずれた</returns>
	bool IsHit(const Rect& rc)const;
};





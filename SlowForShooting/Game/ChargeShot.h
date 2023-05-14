#pragma once
#include"../Geometry.h"
//自機の弾を管理するクラス
class ChargeShot
{
private:
	Rect rect_ = {};
	Vector2 vel_ = {};
	int handle_ = 0;
	int animFrame_ = 0;
	//弾有効フラグ
	bool isEnabled_ = false;
public:
	ChargeShot(int handle);
	const Rect& GetRect()const;

	/// <summary>
	/// 弾を発射する
	/// </summary>
	/// <param name="pos">発射場所</param>
	void Fire(const Position2& pos);
	//弾の有効無効フラグを得る
	bool IsEnabled()const;

	void Update();
	void Draw();
};


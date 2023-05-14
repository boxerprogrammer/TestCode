#pragma once

#include"../Geometry.h"

enum class BulletType {
	Zako
};

//弾基底クラス
class Bullet
{
protected:
	Rect rect_ = {};//当たり矩形(中心座標も持っている)
	Vector2 vel_ = {};//速度
	//今弾は生きているフラグ(true:生きてる false:死んでる)
	bool isEnabled_ = false;
public:
	Bullet();
	/// <summary>
	/// 矩形を返す
	/// </summary>
	/// <returns>矩形</returns>
	const Rect& GetRect()const;

	/// <summary>
	/// 役目を終えて死ぬ
	/// </summary>
	void OnDamaged(int damage);

	/// <summary>
	/// 弾を発射する
	/// </summary>
	/// <param name="pos">発射場所</param>
	void Fire(const Position2& pos,const Vector2& vel);

	//弾の有効無効フラグを得る
	bool IsEnabled()const;

	virtual void Update()=0;
	virtual void Draw()=0;
};



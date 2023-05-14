#pragma once
#include"../Geometry.h"
#include<memory>
class EnemyFactory;
//自機の弾を管理するクラス
class Missle
{
private:
	Rect rect_ = {};
	Vector2 vel_ = {};
	int handle_ = 0;
	int imgIdx_ = 0;
	//今弾は生きているフラグ(true:生きてる false:死んでる)
	bool isEnabled_ = false;
	std::shared_ptr<EnemyFactory> ef_;
public:
	Missle(int handle,std::shared_ptr<EnemyFactory> ef);
	void SetVelocity(const Vector2& vec);
	const Rect& GetRect()const;
	/// <summary>
	/// 役目を終えて死ぬ
	/// </summary>
	void Kill();
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


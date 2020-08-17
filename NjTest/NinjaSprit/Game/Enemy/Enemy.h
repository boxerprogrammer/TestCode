#pragma once
#include "../Character.h"
#include<vector>
class Player;
class Spawner;
class Enemy : public Character
{
	friend Spawner;
protected:
	const std::shared_ptr<Player>& player_;//プレイヤーの情報を知るため
	int life_ = 0;//敵の体力(0以下は死を意味する)
	bool isDeletable_ = false;///リストから消していい(破棄していい)フラグ
	Vector2f velocity_;///<敵の現在速度

	///自分のクローンを返す…が、これは子クラスでの実装
	///を呼び出すだけ(純粋仮想関数)
	virtual Enemy* MakeClone() = 0;
	///プレイヤーを狙う
	virtual void AimPlayer();

public:
	Enemy(const std::shared_ptr<Player>& p , std::shared_ptr<Camera> c);
	virtual ~Enemy() = default;

	virtual void OnHit(CollisionInfo&) = 0;

	/// <summary>
	/// 攻撃を受けた
	/// </summary>
	/// <param name="damage">ダメージ値</param>
	virtual void OnDamage(int damage) = 0;

	/// <summary>
	/// 死イベント
	/// </summary>
	virtual void OnDead() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;


	///速度アクセス
	virtual const Vector2f& GetVelocity()const;
	virtual void SetVelocity(const Vector2f& vel);

	/// <summary>
	/// 死んでるかー？
	/// </summary>
	/// <returns>死んでる:true。生きてる:false</returns>
	virtual bool IsDead()const;

	/// <summary>
	/// 削除してOK？
	/// </summary>
	/// <returns>OK:true,NG:false</returns>
	virtual bool IsDeletable()const;

	virtual const std::vector<Circle>& GetCircles()const = 0;
};


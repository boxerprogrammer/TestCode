#pragma once
#include "Enemy.h"

/// <summary>
/// 雑魚敵その①
/// </summary>
class Patapata : public Enemy
{
private:
	int handle_;
	int burstH_;//爆発時の画像ハンドル
	int idx_ = 0;

	///弾発射コントロール
	const int fire_interval = 300;//弾が発射されるまでの間隔
	const int fire_rand_rate = 30;//発射間隔にばらつきを与えるフレーム
	int fireFrame_ = fire_interval;//実際にこれがゼロになったら発射する

	//通常アニメーション
	void NormalUpdate();
	void NormalDraw();

	//爆発アニメーション
	void BurstUpdate();
	void BurstDraw();

	//メンバ関数ポインタ
	void (Patapata::*updateFunc_)(void );
	void (Patapata::*drawFunc_)(void);
	
public:
	Patapata(std::shared_ptr<Player> player, 
		const Position2& pos,
		int handle,
		int burstH, 
		std::shared_ptr<BulletFactory> bFactory);

	/// <summary>
	/// ダメージが入った
	/// </summary>
	/// <param name="damage"></param>
	void OnDamage(int damage);

	/// <summary>
	/// 衝突可能か？
	/// </summary>
	/// <returns></returns>
	bool IsCollidable()const;
	
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw()override;

	virtual std::shared_ptr<Enemy> Clone()override;

};


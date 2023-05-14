#pragma once
#include "Enemy.h"
#include"ItemType.h"
class ItemFactory;
/// <summary>
/// アイテムをくれるやつ
/// </summary>
class Bringer : public Enemy
{
private:
	int handle_;
	ItemType itemType_;//持ってるアイテムの種別
	int burstH_;//爆発時の画像ハンドル
	int idx_ = 0;
	float angle_ = 0.0f;
	Vector2 vel_ = { -2.0f,0.0f };
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
	void (Bringer::* updateFunc_)(void);

	std::shared_ptr<ItemFactory> itemFactory_;

public:
	Bringer(std::shared_ptr<Player> player,
		const Position2& pos,
		int handle,
		int burstH,
		ItemType itemType,//持ってるアイテムの種別
		std::shared_ptr<BulletFactory> bulletFactory,
		std::shared_ptr<ItemFactory> itemFactory);

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

	virtual std::shared_ptr<Enemy> Clone()override; void (Bringer::* drawFunc_)(void);



};


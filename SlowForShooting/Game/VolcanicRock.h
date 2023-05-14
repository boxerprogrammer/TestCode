#pragma once
#include "Enemy.h"
#pragma once
#include "Enemy.h"
/// <summary>
/// 火山岩(10発当てないと壊せない)
/// </summary>
class VolcanicRock :
	public Enemy
{
private:
	int life_ = 5;//耐久力(10)
	int handle_;
	int burstH_;//爆発時の画像ハンドル
	int idx_ = 0;
	float scale_ = 2.0f;
	float angle_ = 0.0f;
	Vector2 vel_ = { 0.0f,-20.0f };
	float gravity_ = 0.1f;//重力

	//通常アニメーション
	void NormalUpdate();
	void NormalDraw();

	//爆発アニメーション
	void BurstUpdate();
	void BurstDraw();

	//メンバ関数ポインタ
	void (VolcanicRock::* updateFunc_)(void);
	void (VolcanicRock::* drawFunc_)(void);

public:
	VolcanicRock(std::shared_ptr<Player> player,
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


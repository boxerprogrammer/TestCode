#pragma once
#include "Enemy.h"
class EffectManager;
class CollisionManager;
class Stage;
class GameplayingScene;
/// <summary>
/// 侍クラス(敵)
/// </summary>
class Samurai : public Enemy
{
private:
	int life_ = 10;
	int damageTimer_ = 0;
	void HSlash();
	void VSlash();
	void Jump();

	using Update_t = void (Samurai::*)();
	void IdleUpdate();
	void JumpUpdate();
	void FallUpdate();
	void DamageUpdate();
	void RunUpdate();

	
	
	void HSlashUpdate();
	void VSlashUpdate();
	Update_t updater_;
	Update_t lastUpdater_;

	using Draw_t = void (Samurai::*)();
	void IdleDraw();
	void JumpDraw();
	void FallDraw();
	void DamageDraw();
	void RunDraw();
	void HSlashDraw();
	void VSlashDraw();
	Draw_t drawer_;

	void Dead();

	int runH_ = -1;
	int HslashH_ = -1;
	int VslashH_ = -1;
	int damageH_ = -1;
	std::vector<Circle> circles_;
	std::shared_ptr<EffectManager> effectManager_;
	std::shared_ptr<CollisionManager> collisionManager_;
	std::shared_ptr<Stage> stage_;
	std::weak_ptr<Collider> slashCol_;

	std::weak_ptr<Enemy> weakThis_;
	
	bool wasSlashed_ = false;
	int frame_ = 0;
	int animFrame_ = 0;
	Enemy* MakeClone() override;

	Samurai(const std::shared_ptr<Player>& p, std::shared_ptr<Camera> camera, std::shared_ptr<Stage> stg);
public:
	void SetWeakRef(std::weak_ptr<Enemy> w)override;
	Samurai(GameplayingScene* gs);

	/// <summary>
	/// 衝突判定イベント
	/// </summary>
	/// <param name="me">衝突情報自分</param>
	/// <param name="another">衝突情報相手</param>
	void OnHit(CollisionInfo& me, CollisionInfo& another)override;

	/// <summary>
	/// 攻撃を受けた
	/// </summary>
	/// <param name="damage">ダメージ値</param>
	void OnDamage(int damage)override;

	/// <summary>
	/// 死イベント
	/// </summary>
	void OnDead() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	const std::vector<Circle>& GetCircles()const override;
};


#pragma once
#include "Enemy.h"

class EffectManager;
class CollisionManager;
class Camera;
class Stage;
class Slasher :
    public Enemy
{
private:
	int runH = -1;
	int slashH = -1;
	std::shared_ptr<EffectManager> effectManager_;
	std::shared_ptr<CollisionManager> collisionManager_;
	std::shared_ptr<Stage> stage_;
	bool wasSlashed = false;
	int frame_ = 0;
	int animFrame_ = 0;
	Enemy* MakeClone() override;
	using Func_t = void (Slasher::*)();
	
	void RunUpdate();
	void JumpUpdate();
	void FallUpdate();
	void SlashUpdate();
	Func_t updater_;

	void RunDraw();
	void SlashDraw();
	Func_t drawer_;

public:
	Slasher(const std::shared_ptr<Player>& p,std::shared_ptr<Camera> camera,std::shared_ptr<Stage> stg);
	Slasher(const std::shared_ptr<Player>& p,std::shared_ptr<EffectManager> efktMng,  std::shared_ptr<Camera> camera, std::shared_ptr<Stage> stg);
	
	/// <summary>
	/// 何かが当たったイベント(呼ばれる)
	/// </summary>
	/// <param name="colInfo">何がどう当たったか情報</param>
	void OnHit(CollisionInfo& colInfo)override;

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


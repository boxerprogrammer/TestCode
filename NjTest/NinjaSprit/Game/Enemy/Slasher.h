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
	/// ���������������C�x���g(�Ă΂��)
	/// </summary>
	/// <param name="colInfo">�����ǂ��������������</param>
	void OnHit(CollisionInfo& colInfo)override;

	/// <summary>
	/// �U�����󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W�l</param>
	void OnDamage(int damage)override;

	/// <summary>
	/// ���C�x���g
	/// </summary>
	void OnDead() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	const std::vector<Circle>& GetCircles()const override;
};


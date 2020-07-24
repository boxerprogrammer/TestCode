#pragma once
#include "Enemy.h"

class EffectManager;
class CollisionManager;
class Camera;
class Slasher :
    public Enemy
{
private:
	std::shared_ptr<EffectManager> effectManager_;
	std::shared_ptr<CollisionManager> collisionManager_;
	bool wasSlashed = false;
	int frame_ = 0;
	int animFrame_ = 0;
	Enemy* MakeClone() override;
	using Func_t = void (Slasher::*)();
	
	void RunUpdate();
	void SlashUpdate();
	Func_t updater_;

	void RunDraw();
	void SlashDraw();
	Func_t drawer_;

public:
	Slasher(const std::shared_ptr<Player>& p,std::shared_ptr<Camera> camera);
	Slasher(const std::shared_ptr<Player>& p,std::shared_ptr<EffectManager> efktMng,  std::shared_ptr<Camera> camera);
	
	/// <summary>
	/// ���������������C�x���g
	/// </summary>
	/// <param name="">�����ǂ��������������</param>
	void OnHit(CollisionInfo& col)override;

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
};


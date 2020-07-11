#pragma once
#include "Enemy.h"

class EffectManager;
class Slasher :
    public Enemy
{
private:
	std::shared_ptr<EffectManager> effectManager_;
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
	Slasher(const std::shared_ptr<Player>& p);
	Slasher(const std::shared_ptr<Player>& p,std::shared_ptr<EffectManager> efktMng);
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


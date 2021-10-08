#pragma once
#include "Enemy.h"
class EffectManager;
class CollisionManager;
class Stage;
class GameplayingScene;
/// <summary>
/// ���N���X(�G)
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
	/// �Փ˔���C�x���g
	/// </summary>
	/// <param name="me">�Փˏ�񎩕�</param>
	/// <param name="another">�Փˏ�񑊎�</param>
	void OnHit(CollisionInfo& me, CollisionInfo& another)override;

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


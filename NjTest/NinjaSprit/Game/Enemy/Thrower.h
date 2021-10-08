#pragma once
#include "Enemy.h"
class EffectManager;
class CollisionManager;
class ProjectileManager;
class GameplayingScene;
class Camera;
class Stage;
/// <summary>
/// �N�i�C����
/// </summary>
class Thrower :
    public Enemy
{
private:
	int jumpH_ = -1;
	int throwH_ = -1;
	int runH_ = -1;
	int idleH_ = -1;
	int frame_ = 0;
	int animFrame_ = 0;
	int specialAttackTimer_ = 0;//����U���^�C�}�[
	float lockonAngle_=0.0f;
	float addAngle_ = 0.0f;
	std::vector<Circle> circles_;
	std::shared_ptr<EffectManager> effectManager_;
	std::shared_ptr<CollisionManager> collisionManager_;
	ProjectileManager& projectileManager_;
	std::shared_ptr<Stage> stage_;
	void SpecialAttack();
	void SpecialAttackUpdate();

	void RunUpdate();
	void RunawayUpdate();
	void AdjustGround();
	void IdleUpdate();
	void RunDraw();
	void IdleDraw();
	void ThrowUpdate();
	void ThrowDraw();

	void JumpUpdate();
	void FallUpdate();
	void JumpDraw();

	using Update_t = void (Thrower::*)();
	using Draw_t = void (Thrower::*)();

	Update_t updater_;
	Draw_t drawer_;
	Enemy* MakeClone() override;
public:
	Thrower(GameplayingScene* gs);

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


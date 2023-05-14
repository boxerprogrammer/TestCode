#pragma once
#include "Enemy.h"
/// <summary>
/// ��(10�����ĂȂ��Ɖ󂹂Ȃ�)
/// </summary>
class Rock :
    public Enemy
{
private:
	int life_=10;//�ϋv��(10)
	int handle_;
	int burstH_;//�������̉摜�n���h��
	int idx_ = 0;
	float scale_ = 2.0f;
	float angle_ = 0.0f;
	Vector2 vel_ = { -2.0f,0.0f };
	
	//�ʏ�A�j���[�V����
	void NormalUpdate();
	void NormalDraw();

	//�����A�j���[�V����
	void BurstUpdate();
	void BurstDraw();

	//�����o�֐��|�C���^
	void (Rock::* updateFunc_)(void);
	void (Rock::* drawFunc_)(void);

public:
	Rock(std::shared_ptr<Player> player,
		const Position2& pos,
		int handle,
		int burstH,
		std::shared_ptr<BulletFactory> bFactory);

	/// <summary>
	/// �_���[�W��������
	/// </summary>
	/// <param name="damage"></param>
	void OnDamage(int damage);

	/// <summary>
	/// �Փˉ\���H
	/// </summary>
	/// <returns></returns>
	bool IsCollidable()const;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update()override;
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw()override;

	virtual std::shared_ptr<Enemy> Clone()override;

};


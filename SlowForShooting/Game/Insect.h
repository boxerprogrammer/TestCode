#pragma once
#include "Enemy.h"

/// <summary>
/// ���N���X(�G�̈��)
/// </summary>
class Insect :
    public Enemy
{
private:
	int handle_;
	int burstH_;//�������̉摜�n���h��
	int idx_ = 0;
	float angle_ = 0.0f;
	Vector2 vel_ = { -2.0f,0.0f };
	///�e���˃R���g���[��
	const int fire_interval = 300;//�e�����˂����܂ł̊Ԋu
	const int fire_rand_rate = 30;//���ˊԊu�ɂ΂����^����t���[��
	int fireFrame_ = fire_interval;//���ۂɂ��ꂪ�[���ɂȂ����甭�˂���

	//�ʏ�A�j���[�V����
	void NormalUpdate();
	void NormalDraw();

	//�����A�j���[�V����
	void BurstUpdate();
	void BurstDraw();

	//�����o�֐��|�C���^
	void (Insect::* updateFunc_)(void);
	void (Insect::* drawFunc_)(void);

public:
	Insect(std::shared_ptr<Player> player,
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


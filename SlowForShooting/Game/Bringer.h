#pragma once
#include "Enemy.h"
#include"ItemType.h"
class ItemFactory;
/// <summary>
/// �A�C�e�����������
/// </summary>
class Bringer : public Enemy
{
private:
	int handle_;
	ItemType itemType_;//�����Ă�A�C�e���̎��
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
	void (Bringer::* updateFunc_)(void);

	std::shared_ptr<ItemFactory> itemFactory_;

public:
	Bringer(std::shared_ptr<Player> player,
		const Position2& pos,
		int handle,
		int burstH,
		ItemType itemType,//�����Ă�A�C�e���̎��
		std::shared_ptr<BulletFactory> bulletFactory,
		std::shared_ptr<ItemFactory> itemFactory);

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

	virtual std::shared_ptr<Enemy> Clone()override; void (Bringer::* drawFunc_)(void);



};


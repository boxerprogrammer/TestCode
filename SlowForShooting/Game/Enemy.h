#pragma once
#include<memory>
#include"../Geometry.h"
#include"EnemyType.h"



class Player;
class BulletFactory;
/// <summary>
/// �G�̊��N���X
/// </summary>
class Enemy
{
protected://�q�N���X�ɂ͌����������
	std::shared_ptr<Player> player_ = nullptr;
	Rect rect_;//�G�̋�`
	bool isEnabled_ = true;
	//���˂���Ƃ��Ɏg����e�H��ւ̎Q��
	std::shared_ptr<BulletFactory> bulletFactory_;
public:
	Enemy(std::shared_ptr<Player> player,const Position2& pos, std::shared_ptr<BulletFactory> bFactory);
	const Rect& GetRect()const;

	void SetPos(const Position2& pos);

	/// <summary>
	/// �_���[�W���󂯂�
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	virtual void OnDamage(int damage)=0;

	/// <summary>
	/// ���̓G�͐����Ă邩�H
	/// </summary>
	/// <returns>true �����Ă� / false ����ł�</returns>
	bool IsEnabled()const;

	/// <summary>
	/// �Փˉ\���H
	/// </summary>
	/// <returns>true �Փˉ\  /  false �Փ˕s��</returns>
	virtual bool IsCollidable()const = 0;

	/// <summary>
	/// �G�I�u�W�F�N�g�̍X�V
	/// </summary>
	virtual void Update()=0;

	/// <summary>
	/// �G�I�u�W�F�N�g�̕`��
	/// </summary>
	virtual void Draw()=0;

	/// <summary>
	/// �������g�̃N���[����Ԃ�
	/// </summary>
	/// <returns>�G�I�u�W�F�N�g�̃N���[��</returns>
	virtual std::shared_ptr<Enemy> Clone() = 0;

};


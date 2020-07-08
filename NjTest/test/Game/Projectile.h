#pragma once

#include"Character.h"

/// <summary>
/// ���x�������Ĕ��ł����I�u�W�F�N�g
/// </summary>
class Projectile : public Character
{
protected:
	Vector2f vel_;
	bool isActive_=false;
public:
	/// <summary>
	/// ���񂾂��ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsDead()const;
	Projectile();
	virtual ~Projectile() = default;
	/// <summary>
	/// ���ݍ��W�ɑ��x�����Z����
	/// </summary>
	///<remarks>
	/// ���������^���ȊO�̂��Ƃ���肽�����Update��
	/// �I�[�o�[���C�h���Ă�������
	///</remarks>
	virtual void Update()override;

	/// <summary>
	/// Draw�͍Ō�̔h���N���X�Ŏ�������
	/// </summary>
	virtual void Draw() = 0;
};


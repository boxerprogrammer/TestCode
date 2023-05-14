#pragma once

#include"../Geometry.h"

enum class BulletType {
	Zako
};

//�e���N���X
class Bullet
{
protected:
	Rect rect_ = {};//�������`(���S���W�������Ă���)
	Vector2 vel_ = {};//���x
	//���e�͐����Ă���t���O(true:�����Ă� false:����ł�)
	bool isEnabled_ = false;
public:
	Bullet();
	/// <summary>
	/// ��`��Ԃ�
	/// </summary>
	/// <returns>��`</returns>
	const Rect& GetRect()const;

	/// <summary>
	/// ��ڂ��I���Ď���
	/// </summary>
	void OnDamaged(int damage);

	/// <summary>
	/// �e�𔭎˂���
	/// </summary>
	/// <param name="pos">���ˏꏊ</param>
	void Fire(const Position2& pos,const Vector2& vel);

	//�e�̗L�������t���O�𓾂�
	bool IsEnabled()const;

	virtual void Update()=0;
	virtual void Draw()=0;
};



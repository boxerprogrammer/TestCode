#pragma once

#include"../Character.h"
class Camera;
/// <summary>
/// ���x�������Ĕ��ł����I�u�W�F�N�g���N���X
/// </summary>
class Projectile : public Character
{
protected:
	Vector2f vel_;
//	bool isActive_=false;//�L���t���O
public:
	/// <summary>
	/// ���񂾂��ǂ���
	/// </summary>
	/// <returns>true:����ł� false:�����Ă�</returns>
	bool IsDead()const;
	/// <param name="camera">�J�����ւ̎Q��</param>
	Projectile(std::shared_ptr<Camera> camera);
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


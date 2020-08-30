#pragma once
#include<memory>
#include"../Geometry.h"


class Camera;
class Collider;
class Input;
struct CollisionInfo;
///�L�����N�^�[���N���X
class Character
{
	friend Collider;
protected:
	Position2f pos_;//�L�������S���W
	std::shared_ptr<Camera> camera_;
	//�����Ă邩�ǂ���
	//�����ǁA�ꎞ�I�ɋx����Ԃ̂Ƃ���
	//false�ɂȂ����肷��
	bool isActive_ = true;
public:
	/// <summary>
	/// ���̃L�����������Ă邩�ǂ���
	/// </summary>
	/// <returns>true:�����Ă�/false:����ł�</returns>
	bool IsActive()const { return isActive_; }

	/// <summary>
	/// �L�����N�^���N���X�R���X�g���N�^
	/// </summary>
	/// <param name="camera">�J�����ւ̎Q��(�\���Ɏg�p)</param>
	Character(std::shared_ptr<Camera> camera);

	///���W�A�N�Z�X
	virtual const Vector2f& GetPosition()const;
	virtual void SetPosition(const Vector2f& vel);

	virtual ~Character() = default;
	///<summary>�L�����N�^�[�̍X�V���s��</summary>
	virtual void Update() = 0;
	///<summary>�L�����N�^�[�̕\�����s��</summary>
	virtual void Draw() = 0;
	/// <summary>
	/// �Փ˔���C�x���g
	/// </summary>
	/// <param name="me">�Փˏ�񎩕�</param>
	/// <param name="another">�Փˏ�񑊎�</param>
	virtual void OnHit(CollisionInfo& me,CollisionInfo& another) = 0;

};
/// <summary>
/// �L�����N�^�[���������Ă��邩�ǂ���
/// </summary>
/// <param name="velocity">���x�x�N�g��</param>
/// <returns>true ������</returns>
inline bool IsFalling(const Vector2f& velocity) {
	return velocity.y >= 0;
}
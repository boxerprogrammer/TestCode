#pragma once
#include "Equipment.h"
#include"../../Geometry.h"
class Player;
class CapsuleCollider;
class Camera;
class ShadowClone;
/// <summary>
/// ��������
/// �L�яk�݂���B�L�тĂ���Œ��ɐ�s���͂���ƐU��񂷂��Ƃ��ł���
/// </summary>
class ChainEquip : public Equipment
{
private:
	int throwSE_ = -1;
	int chainH_ = -1;
	ShadowClone* shadow_=nullptr;
	CapsuleCollider* capsuleCollider_ = nullptr;
	int frame_;
	std::shared_ptr<Player>& player_;
	float swingTargetAngle_=0.0f;
	float swingPerAngle_ = 0.0f;
	float currentAngle_ = 0.0f;
	int swingFrame_ = 0;
	Vector2f direction_;
	float GetCurrentChainLength()const;
	bool CanBeAdditionalInput()const;
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="collisionMgr">�����蔻��}�l�[�W��</param>
	/// <param name="camera">�J����</param>
	ChainEquip(std::shared_ptr<Player>& player, 
		std::shared_ptr<CollisionManager> collisionMgr, 
		std::shared_ptr<Camera> camera, ShadowClone* shadow=nullptr);
	/// <summary>
	/// �����U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	void Attack(const Player& player, const Input& input,Vector2f offset=Vector2f::ZERO)override;
	/// <summary>
	/// �ǉ�����(�U��񂵂̂��߂����̃��\�b�h)
	/// </summary>
	/// <param name="input">���͏��</param>
	void AdditionalInput(const Input& input);
	/// <summary>
	/// ���t���[���X�V�p
	/// </summary>
	void Update()override;
	/// <summary>
	/// �`��p
	/// </summary>
	void Draw()override;
};


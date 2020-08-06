#pragma once
#include "Equipment.h"
#include"../../Geometry.h"
class Player;
class CapsuleCollider;
class Camera;
/// <summary>
/// ��������
/// �L�яk�݂���B�L�тĂ���Œ��ɐ�s���͂���ƐU��񂷂��Ƃ��ł���
/// </summary>
class ChainEquip : public Equipment
{
private:
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
	ChainEquip(std::shared_ptr<Player>& player, std::shared_ptr<CollisionManager> cm, std::shared_ptr<Camera> camera);
	/// <summary>
	/// �����U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
		void Attack(const Player& player, const Input& input)override;
		void AdditionalInput(const Input& input);
		void Update()override;
		void Draw()override;
};


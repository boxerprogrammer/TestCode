#pragma once
#include "Equipment.h"
#include"../Geometry.h"
class Player;
class ChainEquip :
    public Equipment
{
private:
	int frame_;
	const Player& player_;
	float swingTargetAngle_=0.0f;
	float swingPerAngle_ = 0.0f;
	float currentAngle_ = 0.0f;
	int swingFrame_ = 0;
	Vector2f direction_;
public:
	ChainEquip(const Player& p);
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


#pragma once
#include "Equipment.h"
class Player;
class ChainEquip :
    public Equipment
{
private:
	int frame_;
	const Player& player_;
	enum class Direction {
		left,
		up,
		right,
		down,
	};
	Direction direction_;
public:
	ChainEquip(const Player& p);
	/// <summary>
	/// �����U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
		void Attack(const Player& player, const Input& input)override;
		void Update()override;
		void Draw()override;
};


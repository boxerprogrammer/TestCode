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
	/// 鎖鎌攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
		void Attack(const Player& player, const Input& input)override;
		void Update()override;
		void Draw()override;
};


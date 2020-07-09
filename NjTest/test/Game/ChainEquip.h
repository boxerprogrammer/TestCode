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
	
	Vector2f direction_;
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


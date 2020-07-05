#pragma once
class Player;
class Input;
class Equipment
{
public:
	/// <summary>
	/// 今の武器で攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	virtual void Attack(const Player& p, const Input& i) = 0;
};


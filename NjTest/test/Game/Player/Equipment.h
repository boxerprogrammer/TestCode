#pragma once
class Player;
class Input;
class Equipment
{
public:
	virtual ~Equipment() = default;
	/// <summary>
	/// 今の武器で攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	virtual void Attack(const Player& p, const Input& i) = 0;
	/// <summary>
	/// 追加入力(ほぼ鎖鎌用)
	/// </summary>
	/// <param name="i">現在のinput</param>
	virtual void AdditionalInput(const Input& i) {};
	virtual void Update() {};
	virtual void Draw() {};
};


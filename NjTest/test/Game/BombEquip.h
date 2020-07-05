#pragma once
#include"Equipment.h"

class ProjectileManager;
class BombEquip : public Equipment
{
private:
	ProjectileManager& pm_;
public:
	BombEquip(ProjectileManager& pm);
	/// <summary>
	/// 今の武器で爆弾攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	void Attack(const Player& player, const Input& input)override;
};



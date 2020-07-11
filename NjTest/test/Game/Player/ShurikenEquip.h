#pragma once
#include "Equipment.h"
class ProjectileManager;
class ShurikenEquip :
    public Equipment
{
private:
	ProjectileManager& pm_;
public:
	ShurikenEquip(ProjectileManager& pm);
	/// <summary>
	/// 手裏剣攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	void Attack(const Player& p, const Input& i) override;
};


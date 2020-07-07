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
	/// �藠���U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	void Attack(const Player& p, const Input& i) override;
};


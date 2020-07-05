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
	/// ���̕���Ŕ��e�U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	void Attack(const Player& player, const Input& input)override;
};



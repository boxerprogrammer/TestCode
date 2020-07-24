#pragma once
#include"Equipment.h"
#include<memory>
class ProjectileManager;
class CollisionManager;
class Camera;
class BombEquip : public Equipment
{
private:
	ProjectileManager& pm_;
	
public:
	BombEquip(ProjectileManager& pm,std::shared_ptr<CollisionManager> col,std::shared_ptr<Camera> c);
	/// <summary>
	/// ���e�U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	void Attack(const Player& player, const Input& input)override;
};



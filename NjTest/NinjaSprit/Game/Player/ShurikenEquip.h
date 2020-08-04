#pragma once
#include "Equipment.h"
class ProjectileManager;
class Camera;
class ShurikenEquip :
    public Equipment
{
private:
	ProjectileManager& pm_;
public:
	ShurikenEquip(ProjectileManager& pm,std::shared_ptr<CollisionManager> cm,std::shared_ptr<Camera> camera);
	/// <summary>
	/// �藠���U���������܂�
	/// </summary>
	/// <param name="player">�v���C���[�ւ̎Q��</param>
	/// <param name="input">���͎Q��</param>
	void Attack(const Player& p, const Input& i) override;
};


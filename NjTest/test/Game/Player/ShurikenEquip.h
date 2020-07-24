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
	/// 手裏剣攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	void Attack(const Player& p, const Input& i) override;
};


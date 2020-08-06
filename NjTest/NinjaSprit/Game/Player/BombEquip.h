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
	/// 爆弾攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	void Attack(const Player& player, const Input& input)override;
};



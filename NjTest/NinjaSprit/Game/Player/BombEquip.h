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
	int throwH = -1;
public:
	/// <summary>
	/// 爆弾装備
	/// </summary>
	/// <param name="projectileManager">飛び道具マネージャへの参照</param>
	/// <param name="collisionManager">当たり判定マネージャへの参照</param>
	/// <param name="camera">カメラオブジェクトへの参照</param>
	BombEquip(ProjectileManager& projectileManager,
		std::shared_ptr<CollisionManager> collisionManager,
		std::shared_ptr<Camera> camera);
	/// <summary>
	/// 爆弾攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	void Attack(const Player& player, const Input& input)override;
};



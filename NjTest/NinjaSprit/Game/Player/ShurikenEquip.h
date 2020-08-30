#pragma once
#include "Equipment.h"
class ProjectileManager;
class Camera;
/// <summary>
/// 手裏剣装備
/// </summary>
class ShurikenEquip :
    public Equipment
{
private:
	int throwH_ = -1;
	ProjectileManager& projectileMgr_;
public:
	/// <summary>
	/// 手裏剣装備コンストラクタ
	/// </summary>
	/// <param name="projectileMgr">飛び道具マネージャ</param>
	/// <param name="collisionMgr">当たり判定マネージャ</param>
	/// <param name="camera">カメラ</param>
	ShurikenEquip(ProjectileManager& projectileMgr,
		std::shared_ptr<CollisionManager> collisionMgr,
		std::shared_ptr<Camera> camera);
	/// <summary>
	/// 手裏剣攻撃を加えます
	/// </summary>
	/// <param name="player">プレイヤーへの参照</param>
	/// <param name="input">入力参照</param>
	void Attack(const Player& p, const Input& i, Vector2f offset = Vector2f::ZERO) override;
};


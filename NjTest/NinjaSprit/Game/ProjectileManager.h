#pragma once

#include<memory>
#include<list>

class Projectile;
using ProjectileList_t = std::list<std::shared_ptr<Projectile>>;
/// <summary>
/// 飛び道具管理クラス
/// </summary>
class ProjectileManager
{
private:
	ProjectileList_t projectiles_;
public:
	/// <summary>
	/// 現在の飛び道具リストへの参照を返す
	/// </summary>
	/// <returns>飛び道具リスト</returns>
	ProjectileList_t& GetProjectiles();
	void AddProjectile(Projectile*);
	/// <summary>
	/// 飛び道具すべてをUpdate
	/// </summary>
	void Update();
	/// <summary>
	/// 飛び道具すべてを描画
	/// </summary>
	void Draw();
};


#pragma once
#include "Spawner.h"
class CollisionManager;
class SideSpawner :  public Spawner
{
private:
	int frame_=0;
	std::shared_ptr<CollisionManager> collisionManager_;
public:
	SideSpawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager>& em,std::shared_ptr<CollisionManager> cm);

	/// <summary>
	/// 時間ごとの発生などが必要なため毎フレーム更新を行う
	/// </summary>
	void Update() override;
};


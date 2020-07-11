#include "SideSpawner.h"
#include"Enemy.h"
#include"../../Geometry.h"
#include"EnemyManager.h"

SideSpawner::SideSpawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager>& em):
Spawner(pos,prototype,em){

}

/// <summary>
/// 時間ごとの発生などが必要なため毎フレーム更新を行う
/// </summary>
void 
SideSpawner::Update() {
	if (++frame_ % 60 == 0) {
		auto clone=CreateClone();
		if (clone == nullptr)return;
		clone->SetPosition({ -36,480 });
		enemyManager_->AddEnemy(clone);
	}
}

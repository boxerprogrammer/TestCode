#include "SideSpawner.h"
#include"Enemy.h"
#include"../../Geometry.h"
#include"EnemyManager.h"

SideSpawner::SideSpawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager>& em):
Spawner(pos,prototype,em){

}

/// <summary>
/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
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

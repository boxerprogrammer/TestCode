#include "SideSpawner.h"
#include"Enemy.h"
#include"../../Geometry.h"
#include"EnemyManager.h"
#include"../CollisionManager.h"
#include"../CircleCollider.h"
#include<random>

using namespace std;

//Circle(Position2f(0,0),50),

namespace {
	mt19937 mt;

}

SideSpawner::SideSpawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager>& em, std::shared_ptr<CollisionManager> cm):
collisionManager_(cm)
,Spawner(pos,prototype,em){

}

/// <summary>
/// 時間ごとの発生などが必要なため毎フレーム更新を行う
/// </summary>
void 
SideSpawner::Update() {
	static bool fromRight = false;
	if (++frame_ % (60+rand()%40-20) == 0) {
		auto clone=CreateClone();
		if (clone == nullptr)return;
		if (fromRight) {
			clone->SetPosition({ -36,480 });
		}
		else {
			clone->SetPosition({ 836,480 });
		}
		
		fromRight = !fromRight;
		enemyManager_->AddEnemy(clone);
		collisionManager_->AddCollider(
			new CircleCollider(enemyManager_->Enemies().back(),
			Circle(Position2f(0, 0), 50),
			tag_enemy_damage));
	}
}

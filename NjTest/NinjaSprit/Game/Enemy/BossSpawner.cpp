#include "BossSpawner.h"
#include"../Collision/CollisionManager.h"
#include"EnemyManager.h"
#include"Enemy.h"
#include"../Camera.h"
#include"../Collision/CircleCollider.h"
#include"../Collision/CollisionManager.h"

using namespace std;
BossSpawner::BossSpawner(const Position2f& pos, 
	Enemy* prototype, 
	std::shared_ptr<EnemyManager> em, 
	std::shared_ptr<CollisionManager> cm, 
	std::shared_ptr<Camera> c) :
	collisionManager_(cm)
	, Spawner(pos, prototype, em, c) {
	updater_ = &BossSpawner::CreateUpdate;
}


void 
BossSpawner::CreateUpdate() {
	auto clone = CreateClone();
	enemyManager_->AddEnemy(clone);
	const auto& circles=prototype_->GetCircles();
	const auto& c=circles[0];
	collisionManager_->AddCollider(
		new CircleCollider(enemyManager_->Enemies().back(),
			c,
			tag_enemy_damage));
	updater_ = &BossSpawner::SleepUpdate;
}
void 
BossSpawner::SleepUpdate() {
	
}


void
BossSpawner::Update() {
	(this->*updater_)();
}
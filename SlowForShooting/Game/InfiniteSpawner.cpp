#include "InfiniteSpawner.h"
#include"Enemy.h"
#include"EnemyFactory.h"
#include<DxLib.h>

constexpr int spawn_interval = 60;

InfiniteSpawner::InfiniteSpawner(const Position2& pos, 
	EnemyType type, 
	std::shared_ptr<EnemyFactory> factory)
:Spawner(pos,factory->Make(type), factory) , initPos_(pos){
}

std::shared_ptr<Enemy> InfiniteSpawner::Spawn()
{
	std::shared_ptr<Enemy> clone = protoType_->Clone();
	clone->SetPos({ 640.0f, static_cast<float>(GetRand(480)) });
	factory_->AddEnemy(clone);
	if (pos_.x < 0) {
		isEnabled_ = false;
	}
	return clone;
}

void InfiniteSpawner::Update(int scroll)
{
	pos_.x = initPos_.x - scroll;
	++timer_;
	if (timer_ % spawn_interval == 0) {
		Spawn();
		Spawn();
	}
}

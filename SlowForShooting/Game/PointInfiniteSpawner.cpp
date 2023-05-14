#include "PointInfiniteSpawner.h"
#include"Enemy.h"
#include"EnemyFactory.h"
#include<DxLib.h>

constexpr int spawn_interval = 60;

PointInfiniteSpawner::PointInfiniteSpawner(const Position2& pos,
	EnemyType type,
	std::shared_ptr<EnemyFactory> factory)
	:Spawner(pos, factory->Make(type), factory), initPos_(pos) {
}

std::shared_ptr<Enemy> PointInfiniteSpawner::Spawn()
{
	std::shared_ptr<Enemy> clone = protoType_->Clone();
	clone->SetPos({ pos_.x, pos_.y });
	factory_->AddEnemy(clone);
	if (pos_.x < 0) {
		isEnabled_ = false;
	}
	return clone;
}

void PointInfiniteSpawner::Update(int scroll)
{
	pos_.x = initPos_.x - scroll;
	++timer_;
	if (timer_ % spawn_interval == 0) {
		Spawn();
		Spawn();
	}
}

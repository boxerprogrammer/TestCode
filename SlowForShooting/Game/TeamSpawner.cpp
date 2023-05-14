#include "TeamSpawner.h"
#include"Enemy.h"
#include"EnemyFactory.h"

constexpr int spawn_interval = 15;
constexpr int spawn_limit = 5;

TeamSpawner::TeamSpawner(const Position2& pos, 
   EnemyType type, 
    std::shared_ptr<EnemyFactory> factory):
    Spawner(pos,factory->Make(type), factory), timer_(0), count_(0)
{
}

std::shared_ptr<Enemy> TeamSpawner::Spawn()
{

    std::shared_ptr<Enemy> clone = protoType_->Clone();
    clone->SetPos(pos_);
    factory_->AddEnemy(clone);

    ++count_;
    if (count_ >= spawn_limit) {
        isEnabled_ = false;
    }
    return clone;
}

void TeamSpawner::Update(int scroll)
{
    if ((timer_++ % spawn_interval) == 0) {
        Spawn();
    }

}

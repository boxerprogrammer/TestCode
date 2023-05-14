#include "Spawner.h"

Spawner::Spawner(const Position2& pos,std::shared_ptr<Enemy> protoType,
	std::shared_ptr<EnemyFactory> factory):
	pos_(pos),
	protoType_(protoType),
	factory_(factory)
{
}

bool Spawner::IsEnabled() const
{
	return isEnabled_;
}

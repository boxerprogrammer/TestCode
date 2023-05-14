#include "Enemy.h"
#include"Player.h"



Enemy::Enemy(std::shared_ptr<Player> player,
				const Position2& pos,
				std::shared_ptr<BulletFactory> bFactory):
player_(player),
rect_(pos, {}), 
bulletFactory_(bFactory)
{
}

const Rect& Enemy::GetRect() const
{
	return rect_;
}

void Enemy::SetPos(const Position2& pos)
{
	rect_.center = pos;
}

bool Enemy::IsEnabled() const
{
	return isEnabled_;
}

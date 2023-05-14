#include "Bullet.h"

Bullet::Bullet()
{
}

const Rect& Bullet::GetRect() const
{
	return rect_;
}

void Bullet::OnDamaged(int damage)
{
	isEnabled_ = false;
}

void Bullet::Fire(const Position2& pos, const Vector2& vel)
{
	rect_.center = pos;
	vel_ = vel;
	isEnabled_ = true;
}

bool Bullet::IsEnabled() const
{
	return isEnabled_;
}

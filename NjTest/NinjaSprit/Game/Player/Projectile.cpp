#include "Projectile.h"

Projectile::Projectile(std::shared_ptr<Camera> c):Character(c) {
	isActive_ = true;
}
void 
Projectile::Update() {
	pos_.x += vel_.x;
	pos_.y += vel_.y;
}
bool
Projectile::IsDead()const{ 
	return !isActive_; 
}
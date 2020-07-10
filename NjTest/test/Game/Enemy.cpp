#include "Enemy.h"
#include"Player.h"

Enemy::Enemy(std::shared_ptr<Player>& p) :player_(p) {

}


void 
Enemy::AimPlayer() {

}


	///座標アクセス
const Vector2f& 
Enemy::GetPosition()const {
	return pos_;
}
void 
Enemy::SetPosition(const Vector2f& pos) {
	pos_ = pos;
}

///速度アクセス
const Vector2f& 
Enemy::GetVelocity()const {
	return velocity_;
}
void 
Enemy::SetVelocity(const Vector2f& vel) {

}

bool 
Enemy::IsDead()const {
	return life_ <= 0;
}

bool 
Enemy::IsDeletable()const {
	return isDeletable_;
}
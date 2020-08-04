#include "CircleCollider.h"
#include"../Character.h"
#include<DxLib.h>
#include<cassert>

using namespace std;

CircleCollider::CircleCollider(std::shared_ptr<Character> owner,const char* tag) :Collider(owner,tag), circle_() {
}

CircleCollider::CircleCollider(std::shared_ptr<Character> owner,const Circle& c, const char* tag) :Collider(owner,tag), circle_(c) {
}

CircleCollider::CircleCollider(std::shared_ptr<Character> owner,const Position2f& p, float r, const char* tag) : Collider(owner,tag), circle_(p, r) {

}

const Circle& 
CircleCollider::GetCircle()const {
	return circle_;
}

bool 
CircleCollider::IsHit(shared_ptr<Collider> col) {
	assert(col != nullptr);
	auto ccol = dynamic_pointer_cast<CircleCollider>(col);
	if (ccol != nullptr) {
		Circle a = { AcutualPosition() ,circle_.radius };
		Circle b = { ccol->AcutualPosition() ,ccol->circle_.radius };
		auto sqDiff=(a.center - b.center).SQMagnitude();
		auto totalRadius = a.radius + b.radius;
		return sqDiff <= totalRadius * totalRadius;
	}
	return false;
}

void
CircleCollider::Draw() {
#ifdef _DEBUG
	if (OwnerIsDead())return;
	auto& pos = AcutualPosition();

	uint32_t col = 0xffffff;
	if (GetTag() == tag_player_attack) {
		col = 0xffaaaa;
	}
	auto offset = GetCameraOffset();
	DrawCircle(pos.x+ offset.x,pos.y,circle_.radius,col,0,2);
#endif
}

const Vector2f 
CircleCollider::AcutualPosition()
{
	return circle_.center + GetOwner()->GetPosition();
}

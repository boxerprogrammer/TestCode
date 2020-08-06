#include "Character.h"
#include"../Geometry.h"
#include"Camera.h"
using namespace std;

Character::Character(std::shared_ptr<Camera> camera):camera_(camera) {
	
}

///座標アクセス
const Vector2f&
Character::GetPosition()const {
	return pos_;
}
void
Character::SetPosition(const Vector2f& pos) {
	pos_ = pos;
}
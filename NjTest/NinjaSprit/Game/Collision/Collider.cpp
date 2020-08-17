#include "Collider.h"
#include"../Camera.h"
#include"../Character.h"

using namespace std;


// 一時的に機能しなくする
void 
Collider::Sleep() {
	isActive_ = true;
}
// 機能するようにする
void 
Collider::Awake() {
	isActive_ = true;
}
// 当たり判定は機能しているか？
bool 
Collider::IsActive()const {
	return isActive_;
}


Vector2f 
Collider::GetCameraOffset(){
	if (owner_.expired()) {
		return { 0,0 };
	}
	else {
		return owner_.lock()->camera_->ViewOffset();
	}
}

bool 
Collider::IsDeletable()const {
	return isDeletable_;
}

void 
Collider::Suicide() {
	if (isImmortal_)return;
	isDeletable_ = true;
}

const string& 
Collider::GetTag()const {
	return tag_;
}

Collider::Collider(std::shared_ptr<Character> owner , const char* tag,bool isImmortal) : owner_(owner),
tag_(tag), isDeletable_(false),
isImmortal_(isImmortal){

}

bool
Collider::OwnerIsDead()const {
	return owner_.expired();
}

std::shared_ptr<Character> 
Collider::GetOwner() {
	if (owner_.expired()) {
		return nullptr;
	}
	else {
		return owner_.lock();
	}
}
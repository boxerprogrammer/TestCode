#include "Collider.h"
#include"../Camera.h"
#include"../Character.h"

using namespace std;

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
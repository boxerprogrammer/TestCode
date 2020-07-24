#include "ChainEquip.h"
#include"Player.h"
#include"../../Input.h"
#include"../../Geometry.h"
#include"../CollisionManager.h"
#include"../CapsuleCollider.h"
#include"../Camera.h"
#include<DxLib.h>

using namespace std;

namespace {
	int throwH = -1;
	int chainH = -1;
	constexpr float swing_per_angle=DX_PI_F/18.0f;
}

ChainEquip::ChainEquip(std::shared_ptr<Player>& p ,std::shared_ptr<CollisionManager> cm,std::shared_ptr<Camera> camera):player_(p),
Equipment(cm,camera){
	if (throwH == -1) {
		throwH = LoadSoundMem(L"Resource/Sound/Game/chainatk.mp3");
	}
	if (chainH == -1) {
		chainH = LoadGraph(L"Resource/Image/Player/chainsickle.png");
	}
	frame_ = -1;

}

void
ChainEquip::Attack(const Player& player, const Input& input) {
	if (frame_ >= 0)return;
	direction_ = {0.0f,0.0f};
	if (input.IsPressed("left")) {
		direction_ += {-1.0f,0.0f};
	}
	if (input.IsPressed("right")) {
		direction_ += {1.0f, 0.0f};
	}
	if (input.IsPressed("up")) {
		direction_ += {0.0f, -1.0f};
	}
	if (input.IsPressed("down")) {
		direction_ += {0.0f, 1.0f };
	}
	if (direction_.x == 0.0f && direction_.y == 0.0f) {
		if (player.GetDirection() == Player::Direction::right) {
			direction_.x = 1.0f;
		}
		if (player.GetDirection() == Player::Direction::left) {
			direction_.x = -1.0f;
		}
	}
	direction_.Normalize();
	PlaySoundMem(throwH, DX_PLAYTYPE_BACK);
	frame_ = 0;
	currentAngle_ = atan2f(direction_.y, direction_.x);
	if (currentAngle_ < 0.0f) {
		currentAngle_ += 2 * DX_PI_F;
	}
	swingTargetAngle_ = currentAngle_;

	if (capsuleCollider_ == nullptr) {
		Capsule cap = { {{0,0},{0,0}},30 };
		capsuleCollider_ = new CapsuleCollider(player_, cap, tag_player_attack, true);
		collisionManager_->AddCollider(capsuleCollider_);
	}
}

void 
ChainEquip::AdditionalInput(const Input& input) {
	if (frame_ > 10)return;
	Vector2f newVec = { 0.0f,0.0f };
	if (input.IsPressed("left")) {
		newVec += {-1.0f, 0.0f};
	}
	if (input.IsPressed("right")) {
		newVec += {1.0f, 0.0f};
	}
	if (input.IsPressed("up")) {
		newVec += {0.0f, -1.0f};
	}
	if (input.IsPressed("down")) {
		newVec += {0.0f, 1.0f };
	}
	if (Cross(newVec, direction_) == 0.0f)return;
	swingTargetAngle_ = atan2f(newVec.y, newVec.x);
	if (swingTargetAngle_ < 0.0f){
		swingTargetAngle_ += 2 * DX_PI_F;
	}
	float sign = std::signbit(swingTargetAngle_ - currentAngle_)?-1.0f:1.0f;
	if (fabsf(swingTargetAngle_ - currentAngle_) >= DX_PI_F) {
		sign *= -1.0f;
	}
	swingPerAngle_=swing_per_angle*sign;
}

void 
ChainEquip::Update() {
	
	if (frame_ < 0){
		if (capsuleCollider_ != nullptr) {
			capsuleCollider_->GetCapsule().seg.vec = { 0,0 };
		}
		return;
	}
	if (frame_<10 || currentAngle_ == swingTargetAngle_) {
		++frame_;
	}
	else if (currentAngle_ != swingTargetAngle_) {
		currentAngle_ += swingPerAngle_;
		constexpr float PI2F = DX_PI_F * 2.0f;
		if (fabsf(std::fmod(swingTargetAngle_ + PI2F, PI2F) -
			std::fmod(currentAngle_ + PI2F, PI2F)) < swing_per_angle) {
			currentAngle_ = swingTargetAngle_;
			direction_ = { cosf(currentAngle_),sin(currentAngle_) };
		}
	}
	else {
		swingPerAngle_ = 0.0f;
	}
	if (frame_ > 20) {
		frame_ = -1;
	}
	auto& vec = capsuleCollider_->GetCapsule().seg.vec;
	vec = direction_ * GetChainLength();
}

float 
ChainEquip::GetChainLength()const {
	int f = abs((frame_ + 10) % 20 - 10);
	return f * 400 / 10;
}

void
ChainEquip::Draw() {
	auto pos=player_->Position();
	if (frame_ >=0) {
		int w = GetChainLength();
		const int xoffset = camera_->ViewOffset().x;
		DrawRectRotaGraph2(static_cast<int>(pos.x+xoffset), static_cast<int>(pos.y),
			400 - w, 0, 
			w, 48,
			0,24,
			1.0f,
			currentAngle_,
			chainH, true);
	}
}

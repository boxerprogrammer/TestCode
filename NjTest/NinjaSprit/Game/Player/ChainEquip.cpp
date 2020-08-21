#include "ChainEquip.h"
#include"Player.h"
#include"../../Input/Input.h"
#include"../../Geometry.h"
#include"../Collision/CollisionManager.h"
#include"../Collision/CapsuleCollider.h"
#include"../Camera.h"
#include"../../Arithmetic.h"
#include"../../System/File.h"
#include"../../System/FileManager.h"
#include<DxLib.h>

using namespace std;

namespace {
	
	constexpr float swing_per_angle=DX_PI_F/18.0f;
	constexpr float equip_offset_y = -48.0f;
	constexpr int expansion_frames = 10;
	constexpr float capsule_radius = 30.0f;
	constexpr int reciprocal_frames = expansion_frames*2;
}

bool 
ChainEquip::CanBeAdditionalInput()const {
	return frame_ <= expansion_frames;
}

ChainEquip::ChainEquip(std::shared_ptr<Player>& p ,std::shared_ptr<CollisionManager> cm,std::shared_ptr<Camera> camera):player_(p),
Equipment(cm,camera){
	auto& fileMgr=FileManager::Instance();
	if (throwSE_ == -1) {
		throwSE_ = fileMgr.Load(L"Resource/Sound/Game/chainatk.mp3")->Handle();
	}
	if (chainH_ == -1) {
		chainH_ = fileMgr.Load(L"Resource/Image/Player/chainsickle.png")->Handle();
	}
	frame_ = -1;

}

void
ChainEquip::Attack(const Player& player, const Input& input) {
	if (frame_ >= 0)return;
	direction_ = {0.0f,0.0f};
	if (input.IsPressed("left")) {
		direction_ += Vector2f::LEFT;
	}
	if (input.IsPressed("right")) {
		direction_ += Vector2f::RIGHT;
	}
	if (input.IsPressed("up")) {
		direction_ += Vector2f::UP;
	}
	if (input.IsPressed("down")) {
		direction_ += Vector2f::DOWN;
	}
	if (direction_==Vector2f::ZERO) {
		if (player.GetDirection() == Player::Direction::right) {
			direction_.x = 1.0f;
		}
		if (player.GetDirection() == Player::Direction::left) {
			direction_.x = -1.0f;
		}
	}
	direction_.Normalize();
	PlaySoundMem(throwSE_, DX_PLAYTYPE_BACK);
	frame_ = 0;
	currentAngle_ = atan2f(direction_.y, direction_.x);
	if (currentAngle_ < 0.0f) {
		currentAngle_ += 2 * DX_PI_F;
	}
	swingTargetAngle_ = currentAngle_;

	if (capsuleCollider_ == nullptr) {
		Capsule cap = { {{0,equip_offset_y},Vector2f::ZERO},capsule_radius };
		capsuleCollider_ = new CapsuleCollider(player_, cap, tag_player_attack, true);
		collisionManager_->AddCollider(capsuleCollider_);
	}
}

void 
ChainEquip::AdditionalInput(const Input& input) {
	if (!CanBeAdditionalInput())return;
	Vector2f newVec = Vector2f::ZERO;
	if (input.IsPressed("left")) {
		newVec += Vector2f::LEFT;
	}
	if (input.IsPressed("right")) {
		newVec += Vector2f::RIGHT;
	}
	if (input.IsPressed("up")) {
		newVec += Vector2f::UP;;
	}
	if (input.IsPressed("down")) {
		newVec += Vector2f::DOWN;
	}
	if (Cross(newVec, direction_) == 0.0f)return;
	swingTargetAngle_ = newVec.AngleFromX();
	if (swingTargetAngle_ < 0.0f){
		swingTargetAngle_ += 2 * DX_PI_F;
	}
	float sign = Sign(swingTargetAngle_ - currentAngle_);
	if (fabsf(swingTargetAngle_ - currentAngle_) >= DX_PI_F) {
		sign *= -1.0f;
	}
	swingPerAngle_=swing_per_angle*sign;
}

void 
ChainEquip::Update() {
	
	if (frame_ < 0){
		if (capsuleCollider_ != nullptr) {
			capsuleCollider_->GetCapsule().seg.vec = Vector2f::ZERO;
		}
		return;
	}
	if (CanBeAdditionalInput() || currentAngle_ == swingTargetAngle_) {
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
	//çƒê∂ÇµÇ¬Ç≠ÇµÇΩÇÁ-1Ç…ñﬂÇ∑
	if (frame_ > reciprocal_frames) {
		frame_ = -1;
	}
	auto& vec = capsuleCollider_->GetCapsule().seg.vec;
	vec = direction_ * GetCurrentChainLength();
}

float 
ChainEquip::GetCurrentChainLength()const {
	//çsÇ¡ÇƒñﬂÇÈí∑Ç≥Çï\åªÇµÇƒÇ¢ÇÈ
	int f = abs((frame_ + expansion_frames) % reciprocal_frames - expansion_frames);
	return static_cast<float>(f) * 400.0f / (float)expansion_frames;
}

void
ChainEquip::Draw() {
	auto pos=player_->Position();
	if (frame_ >=0) {
		int w = static_cast<int>(GetCurrentChainLength());
		const auto xoffset = camera_->ViewOffset().x;
		DrawRectRotaGraph2(static_cast<int>(pos.x+xoffset), static_cast<int>(pos.y+ equip_offset_y),
			400 - w, 0, 
			w, 48,
			0,24,
			1.0f,
			currentAngle_,
			chainH_, true);
	}
}

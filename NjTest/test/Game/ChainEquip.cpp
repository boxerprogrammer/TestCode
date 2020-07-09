#include "ChainEquip.h"
#include"Player.h"
#include"../Input.h"
#include"../Geometry.h"
#include<DxLib.h>

namespace {
	int throwH = -1;
	int chainH = -1;
}

ChainEquip::ChainEquip(const Player& p):player_(p){
	if (throwH == -1) {
		throwH = LoadSoundMem(L"Resource/Sound/Game/shurikenthrow.wav");
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
}

void 
ChainEquip::Update() {
	if (frame_ >= 0) {
		++frame_;
		if (frame_ > 20) {
			frame_ = -1;
		}
	}
}

void
ChainEquip::Draw() {
	auto pos=player_.Position();
	if (frame_ >=0) {
		auto angle = atan2f(direction_.y,direction_.x);
		int f = abs((frame_ + 10) % 20 - 10);
		int w = f*400/10;
		DrawRectRotaGraph2(pos.x, pos.y,
			400 - w, 0, 
			w, 48,
			0,24,
			1.0f,
			angle,
			chainH, true);
		
	}
}

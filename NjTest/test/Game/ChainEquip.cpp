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
	Vector2f vel(5, 0);
	if (player.GetDirection() == Player::Direction::right) {
		direction_ = Direction::right;
	}
	if (player.GetDirection() == Player::Direction::left) {
		direction_ = Direction::left;
	}
	if (input.IsPressed("up")) {
		direction_ = Direction::up;
	}
	if (input.IsPressed("down")) {
		direction_ = Direction::down;
	}
	PlaySoundMem(throwH, DX_PLAYTYPE_BACK);
	frame_ = 0;
}

void 
ChainEquip::Update() {
	if (frame_ >= 0) {
		++frame_;
		if (frame_ > 40) {
			frame_ = -1;
		}
	}
}

void
ChainEquip::Draw() {
	auto pos=player_.Position();
	if (frame_ >=0) {
		int f = abs((frame_ + 20) % 40 - 20);
		int w=f*400.0f / 20.0f;
		if (direction_ == Direction::right) {
			DrawRectRotaGraph2(pos.x, pos.y, 400 - w, 0, w, 48, 0, 24, 1.0f, 0.0f, chainH, true, false);
		}
		else if (direction_ == Direction::left) {
			DrawRectRotaGraph2(pos.x-w, pos.y, 400 - w, 0, w, 48, 0, 24, 1.0f, 0.0f, chainH, true, true);
		}
	}
}

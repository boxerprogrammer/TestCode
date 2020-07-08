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
	directionVector_ = { 0, 0 };
	if (input.IsPressed("right")) {
		directionVector_.x = 1.0f;
	}
	if (input.IsPressed("left")) {
		directionVector_.x = -1.0f;
	}
	if (input.IsPressed("up")) {
		directionVector_.y = -1.0f;
	}
	if (input.IsPressed("down")) {
		directionVector_.y = 1.0f;
	}
	if (directionVector_.x == 0.0f && directionVector_.y == 0.0f) {
		if (player.GetDirection() == Player::Direction::right) {
			directionVector_.x = 1.0f;
		}
		if (player.GetDirection() == Player::Direction::left) {
			directionVector_.x = -1.0f;
		}
	}
	directionVector_.Normalize();
	PlaySoundMem(throwH, DX_PLAYTYPE_BACK);
	frame_ = 0;
}

void 
ChainEquip::Update() {
	if (frame_ >= 0) {
		++frame_;
		if (frame_ > 30) {
			frame_ = -1;
		}
	}
}

void
ChainEquip::Draw() {
	auto pos=player_.Position();
	if (frame_ >= 0) {
		int f = abs((frame_ + 15) % 30 - 15);
		float w = f * 400.0f / 15.0f;
		Vector2f v90 = { -directionVector_.y, directionVector_.x };
		//if (direction_ == Direction::right) {
		
		{
			Position2f p1 = pos + v90 * 24.0f;
			Position2f p2 = pos + directionVector_ * w + v90 * 24.0f ;
			Position2f p3 = pos + directionVector_ * w - v90 *24.0f;
			Position2f p4 = pos - v90 * 24.0f;

			DrawRectModiGraph(
				p1.x,p1.y,
				p2.x,p2.y,
				p3.x,p3.y,
				p4.x,p4.y,
				400 - w, 0,
				w, 48, chainH, true);
			DrawQuadrangle(
				p1.x, p1.y,
				p2.x, p2.y,
				p3.x, p3.y,
				p4.x, p4.y, 0xffffff, false);
		}
	}
	//	else if (direction_ == Direction::left) {
	//		DrawRectModiGraph(pos.x, pos.y,
	//			pos.x - w, pos.y,
	//			pos.x - w, pos.y + 48,
	//			pos.x, pos.y + 48, 
	//			400 - w, 0, 
	//			w, 48, chainH, true);
	//	}else if (direction_ == Direction::up) {
	//		DrawRectModiGraph(pos.x-24, pos.y,
	//			pos.x-24, pos.y-w,
	//			pos.x+24, pos.y -w,
	//			pos.x+24, pos.y + 48, 
	//			400 - w, 0,
	//			w, 48, chainH, true);
	//	}
	//	else if (direction_ == Direction::left) {
	//		DrawRectModiGraph(pos.x, pos.y,
	//			pos.x - w, pos.y,
	//			pos.x - w, pos.y + 48,
	//			pos.x, pos.y + 48, 
	//			400 - w, 0, 
	//			w, 48, chainH, true);
	//	}
	//}
}

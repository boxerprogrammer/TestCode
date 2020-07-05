#include "BombShot.h"
#include<DxLib.h>

namespace {
	int bombH = -1;
}

BombShot::BombShot(const Position2& pos, const Vector2f& vel) {
	pos_ = pos;
	vel_ = vel;
	if (bombH == -1) {
		bombH = LoadGraph(L"Resource/Image/Player/bombshot.png");
	}
	angle_ = 0.0f;
}
//Update‚ÍProjectile‚Ì‚à‚Ì‚ðŽg—p
//void Update()override;
void 
BombShot::Draw() {
	DxLib::DrawRotaGraph(pos_.x,pos_.y,0.75f,angle_,bombH,true);
	angle_ += 0.25f;
}
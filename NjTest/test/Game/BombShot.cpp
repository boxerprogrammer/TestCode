#include "BombShot.h"
#include<DxLib.h>

int bombH = -1;

BombShot::BombShot(const Position2& pos, const Vector2f& vel) {
	pos_ = pos;
	vel_ = vel;
	bombH = LoadGraph(L"Resource/Image/Player/bombshot.png");
	angle_ = 0.0f;
}
//Update‚ÍProjectile‚Ì‚à‚Ì‚ðŽg—p
//void Update()override;
void 
BombShot::Draw() {
	DxLib::DrawRotaGraph(pos_.x,pos_.y,1.0f,angle_,bombH,true);
	angle_ += 0.1f;
}
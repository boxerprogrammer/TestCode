#include "BombShot.h"
#include<DxLib.h>

namespace {
	int bombH = -1;
	int explodeSH = -1;
	int explodeH = -1;
}

BombShot::~BombShot() {
	OutputDebugStringA("\nBomb shot is deleted\n");
}

BombShot::BombShot(const Position2& pos, const Vector2f& vel) {
	pos_ = pos;
	vel_ = vel;
	if (bombH == -1) {
		bombH = LoadGraph(L"Resource/Image/Player/bombshot.png");
	}
	if (explodeH == -1) {
		explodeH = LoadGraph(L"Resource/Image/Effect/bomb_prj_exp.png");
	}
	if (explodeSH == -1) {
		explodeSH = LoadSoundMem(L"Resource/Sound/Game/bombexp.wav");
	}
	angle_ = 0.0f;
	isActive_ = true;
	updater_ = &BombShot::NormalUpdate;
	drawer_ = &BombShot::NormalDraw;
}
//Update‚ÍProjectile‚Ì‚à‚Ì‚ðŽg—p
void 
BombShot::Update() {
	(this->*updater_)();
}
void 
BombShot::Draw() {
	(this->*drawer_)();
}


void 
BombShot::NormalUpdate() {
	angle_ += 0.25f;
	Projectile::Update();//java‚Å‚¢‚¤base.Update();‚¾‚ÆŽv‚Á‚Ä‚­‚¾‚³‚¢
	if (pos_.x >= 800 || pos_.x <= 0 || pos_.y >= 600 || pos_.y <= 0) {
		updater_ = &BombShot::ExplodeUpdate;
		drawer_ = &BombShot::ExplodeDraw;
		frame_=0;
		PlaySoundMem(explodeSH, DX_PLAYTYPE_BACK);
	}
}
void 
BombShot::ExplodeUpdate() {
	++frame_;
	if (frame_ >= 5 * 8) {
		updater_ = &BombShot::KillUpdate;
		drawer_ = &BombShot::NoDraw;
	}
}
void 
BombShot::KillUpdate() {
	isActive_ = false;
}

void 
BombShot::NormalDraw() {
	DxLib::DrawRotaGraph(pos_.x, pos_.y, 0.75f, angle_, bombH, true);

}
void 
BombShot::ExplodeDraw() {
	DrawRectRotaGraph(pos_.x, pos_.y, 48 * (frame_ / 4), 0, 48, 48, 3.0f, 0.0f, explodeH, true);
}

void 
BombShot::NoDraw() {

}
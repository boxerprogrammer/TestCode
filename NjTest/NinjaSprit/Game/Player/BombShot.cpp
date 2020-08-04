#include "BombShot.h"
#include"../Collision/Collider.h"
#include"../Camera.h"
#include<DxLib.h>

namespace {
	int bombH = -1;
	int explodeSH = -1;
	int explodeH = -1;
}

BombShot::~BombShot() {
#ifdef _DEBUG
	OutputDebugStringA("\nBomb shot is deleted\n");
#endif
}

BombShot::BombShot(const Position2f& pos, const Vector2f& vel,std::shared_ptr<Camera> c) :Projectile(c),frame_(0){
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
//Update��Projectile�̂��̂��g�p
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
	Projectile::Update();//java�ł���base.Update();���Ǝv���Ă�������
	const auto& vrect = camera_->GetViewRange();
	if (pos_.x <= vrect.Left() ||  vrect.Right()<= pos_.x ||
		pos_.y <= vrect.Top()|| vrect.Bottom()<= pos_.y) {
		Explode();
	}
}
void BombShot::Explode()
{
	updater_ = &BombShot::ExplodeUpdate;
	drawer_ = &BombShot::ExplodeDraw;
	frame_ = 0;
	PlaySoundMem(explodeSH, DX_PLAYTYPE_BACK);
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
	const auto xoffset = camera_->ViewOffset().x;
	DxLib::DrawRotaGraph(
		static_cast<int>(pos_.x+xoffset), static_cast<int>(pos_.y), 0.75f, angle_, bombH, true);

}
void 
BombShot::ExplodeDraw() {
	const auto xoffset = camera_->ViewOffset().x;
	DrawRectRotaGraph(
		static_cast<int>(pos_.x+xoffset), static_cast<int>(pos_.y), 48 * (frame_ / 4), 0, 48, 48, 3.0f, 0.0f, explodeH, true);
}

void 
BombShot::NoDraw() {

}

void
BombShot::OnHit(CollisionInfo& info) {
	if (info.collider->GetTag() != tag_player_attack) {
		Explode();
	}
}
#include "BombShot.h"
#include"../Collision/Collider.h"
#include"../Camera.h"
#include"../../System/File.h"
#include"../../System/FileManager.h"
#include<DxLib.h>

namespace {

}

BombShot::~BombShot() {
}

BombShot::BombShot(const Position2f& pos, const Vector2f& vel,std::shared_ptr<Camera> c) :Projectile(c),frame_(0){
	pos_ = pos;
	vel_ = vel;
	auto& fileMgr=FileManager::Instance();
	if (bombH == -1) {
		bombH = fileMgr.Load(L"Resource/Image/Player/bombshot.png")->Handle();
	}
	if (explodeH == -1) {
		explodeH = fileMgr.Load(L"Resource/Image/Effect/bomb_prj_exp.png")->Handle();
	}
	if (explodeSH == -1) {
		explodeSH = fileMgr.Load(L"Resource/Sound/Game/bombexp.wav")->Handle();
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
#include "ShurikenShot.h"
#include"../Collision/Collider.h"
#include"../Camera.h"
#include"../../System/FileManager.h"
#include"../../System/File.h"
#include<DxLib.h>
namespace {
	
}
ShurikenShot::ShurikenShot(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> c):Projectile(c) {
	auto& fileMgr = FileManager::Instance();
	pos_ = pos;
	vel_ = vel;
	if (shurikenH_ == -1) {
		shurikenH_ = fileMgr.Load(L"Resource/Image/Player/shuriken.png")->Handle();
	}
	if (shurikenSE_ == -1) {
		shurikenSE_ = fileMgr.Load(L"Resource/Sound/Game/shurikenatk.wav")->Handle();
	}
}
ShurikenShot::~ShurikenShot() {
}

void
ShurikenShot::Update() {
	angle_ += 0.5f;
	Projectile::Update();
	const auto& vrect = camera_->GetViewRange();
	if (pos_.x >= vrect.Right() || pos_.x <= vrect.Left() || pos_.y >= vrect.Bottom() || pos_.y <= vrect.Top()) {
		PlaySoundMem(shurikenSE_, DX_PLAYTYPE_BACK);
		isActive_ = false;
	}
}

void 
ShurikenShot::Draw(){
	const auto xoffset = camera_->ViewOffset().x;
	DxLib::DrawRotaGraph(
		static_cast<int>(pos_.x+xoffset), 
		static_cast<int>(pos_.y), 1.0f, angle_, shurikenH_, true);
}

void 
ShurikenShot::OnHit(CollisionInfo& me, CollisionInfo& other) {
	if (other.collider->GetTag() == tag_enemy_damage) {
		isActive_ = false;
	}
}
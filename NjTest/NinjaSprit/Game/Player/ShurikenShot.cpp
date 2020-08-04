#include "ShurikenShot.h"
#include"../Collision/Collider.h"
#include"../Camera.h"
#include<DxLib.h>
namespace {
	int shurikenH = -1;
	int shurikenSE = -1;
}
ShurikenShot::ShurikenShot(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> c):Projectile(c) {
	pos_ = pos;
	vel_ = vel;
	if (shurikenH == -1) {
		shurikenH = LoadGraph(L"Resource/Image/Player/shuriken.png");
	}
	if (shurikenSE == -1) {
		shurikenSE = LoadSoundMem(L"Resource/Sound/Game/shurikenatk.wav");
	}
}
ShurikenShot::~ShurikenShot() {
#ifdef _DEBUG
	OutputDebugStringA("Shuriken is deleted");
#endif
}

void
ShurikenShot::Update() {
	angle_ += 0.5f;
	Projectile::Update();
	const auto& vrect = camera_->GetViewRange();
	if (pos_.x >= vrect.Right() || pos_.x <= vrect.Left() || pos_.y >= vrect.Bottom() || pos_.y <= vrect.Top()) {
		PlaySoundMem(shurikenSE, DX_PLAYTYPE_BACK);
		isActive_ = false;
	}
}

void 
ShurikenShot::Draw(){
	const auto xoffset = camera_->ViewOffset().x;
	DxLib::DrawRotaGraph(
		static_cast<int>(pos_.x+xoffset), 
		static_cast<int>(pos_.y), 1.0f, angle_, shurikenH, true);
}

void 
ShurikenShot::OnHit(CollisionInfo& info) {
	if (info.collider->GetTag() == tag_enemy_damage) {
		isActive_ = false;
	}
}
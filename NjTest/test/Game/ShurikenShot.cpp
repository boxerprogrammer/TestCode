#include "ShurikenShot.h"
#include<DxLib.h>
namespace {
	int shurikenH = -1;
	int shurikenSE = -1;
}
ShurikenShot::ShurikenShot(const Position2& pos, const Vector2f& vel) {
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
	if (pos_.x >= 800 || pos_.x <= 0 || pos_.y >= 600 || pos_.y <= 0) {
		PlaySoundMem(shurikenSE, DX_PLAYTYPE_BACK);
		isActive_ = false;
	}
}

void 
ShurikenShot::Draw(){
	DxLib::DrawRotaGraph(pos_.x, pos_.y, 1.0f, angle_, shurikenH, true);
}
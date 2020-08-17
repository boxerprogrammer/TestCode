#include "Asura.h"
#include"../../Arithmetic.h"
#include"../Camera.h"
#include"../../System/File.h"
#include"../../System/FileManager.h"
#include"../Collision/Collider.h"
#include"../../Scene/GameplayingScene.h"
#include"../Effect.h"
#include<DxLib.h>

namespace {
	float ground_line = 0.0f;
	float entering_speed = 3.0f;
	constexpr float draw_scale = 1.2f;
	constexpr float chichu_y = 1100.f;
}

Enemy*
Asura::MakeClone() {
	return new Asura(*this);
}

Asura::Asura(GameplayingScene* gs):Boss(gs) {
	updater_ = &Asura::EnteringUpdate;
	drawer_ = &Asura::NormalDraw;

	auto& fileMgr = FileManager::Instance();
	ashuraH_ = fileMgr.Load(L"Resource/Image/Enemy/ashura.png")->Handle();

	

	auto rc = camera_->GetViewRange();
	//左右中央に配置
	pos_.x = static_cast<float>(rc.Left() + rc.Right()) / 2.0f;
	pos_.y = chichu_y;//地中深くに
	ground_line = rc.Height() - 16;
	circles_.emplace_back(Position2f(0, -400), 50);

	life_ = 100;
	effectManager_ = gs->GetEffectManager();
}

//登場演出状態
void 
Asura::EnteringUpdate() {
	if (pos_.y <= ground_line) {
		updater_ = &Asura::NormalUpdate;
		pos_.y = ground_line;
	}
	else {
		pos_.y = SaturateSubtract(pos_.y, entering_speed, ground_line);
	}
}

const std::vector<Circle>&
Asura::GetCircles()const {
	return circles_;
}

//通常状態
void 
Asura::NormalUpdate() {

}

void
Asura::DamageUpdate() {
	if (--frame_ >= 0) {
		if (frame_ % 2 == 0) {
			drawer_ = &Asura::NormalDraw;
		}
		else {
			drawer_ = &Asura::DamageDraw;
		}
	}
	else {
		updater_ = &Asura::NormalUpdate;
	}
}

//退場演出状態
void 
Asura::ExitingUpdate() {
	pos_.y = SaturateAdd(pos_.y, entering_speed, chichu_y);
	if (pos_.y == chichu_y) {
		updater_ = &Asura::DeadUpdate;
		drawer_ = &Asura::DeadDraw;
		gameScene_->OnBossDie();
		isDeletable_ = true;
		isActive_ = false;
	}
	
	if (frame_ % 5 == 0) {
		int w, h;
		GetGraphSize(ashuraH_, &w, &h);
		float x = (rand() % w) - w/2;
		effectManager_->EmitGroundExplosion(Position2f(pos_.x+x, ground_line));
	}
	++frame_;
}

void 
Asura::DeadUpdate() {

}

void
Asura::DeadDraw() {

}

//通常描画
void 
Asura::NormalDraw() {
	int w, h;
	GetGraphSize(ashuraH_, &w, &h);
	const auto xoffset = camera_->ViewOffset().x;
	DrawRotaGraph2(
		pos_.x+xoffset, pos_.y,
		w/2, 400, 
		draw_scale, 0.0f,
		ashuraH_, true);
}
void 
Asura::DamageDraw() {
	NormalDraw();
	SetDrawBlendMode(DX_BLENDMODE_ADD, 256);
	NormalDraw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void
Asura::ExitingDraw() {
	NormalDraw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
	NormalDraw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void 
Asura::OnHit(CollisionInfo& other) {
	if (other.collider->GetTag() == tag_player_attack) {
		if (updater_ == &Asura::NormalUpdate) {
			OnDamage(1);
		}
	}
}

// 攻撃を受けた
void 
Asura::OnDamage(int damage) {
	updater_ = &Asura::DamageUpdate;
	drawer_ = &Asura::DamageDraw;
	frame_ = 8;
	life_ -= damage;
	if (life_ <= 0) {
		OnDead();
	}
}




// 死イベント
void 
Asura::OnDead() {
	updater_ = &Asura::ExitingUpdate;
	drawer_ = &Asura::ExitingDraw;
	frame_ = 0;
}

// 更新
void 
Asura::Update() {
	(this->*updater_)();
}

// 描画
void 
Asura::Draw() {
	(this->*drawer_)();
}
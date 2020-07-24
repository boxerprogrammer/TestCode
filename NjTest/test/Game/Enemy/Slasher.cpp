#include "Slasher.h"
#include "../Player/Player.h"
#include"../Effect.h"
#include<DxLib.h>
#include"../Collider.h"
#include"../Camera.h"
namespace {
	int runH = -1;
	int slashH = -1;
}

void Slasher::OnHit(CollisionInfo& col){
	if (col.collider->GetTag() == tag_player_attack) {
		float sign = velocity_.x < 0 ? -1.0f : 1.0f;
		effectManager_->EmitBlow3(pos_ + Vector2f(sign * 50, 50), sign < 0.0f);
		isDeletable_ = true;
	}
}

Slasher::Slasher(const std::shared_ptr<Player>& p, std::shared_ptr<Camera> camera) :
Enemy(p,camera)
{
	if (runH == -1) {
		runH = LoadGraph(L"Resource/Image/Enemy/Slasher/run.png");
	}
	if (slashH == -1) {
		slashH = LoadGraph(L"Resource/Image/Enemy/Slasher/slash.png");
	}
	updater_ = &Slasher::RunUpdate;
	drawer_ = &Slasher::RunDraw;
}

Slasher::Slasher(const std::shared_ptr<Player>& p, std::shared_ptr<EffectManager> efktMng, std::shared_ptr<Camera> camera):
	Slasher(p,camera) {
	effectManager_ = efktMng;
}

Enemy* 
Slasher::MakeClone() {
	return new Slasher(*this);
}
void 
Slasher::OnDamage(int damage) {

}
void 
Slasher::OnDead() {
}

void
Slasher::RunUpdate() {
	if (frame_ % (150+rand()%60-30) == 0) {
		AimPlayer();
		wasSlashed = false;
		velocity_ *= 5.0f;
	}
	pos_ += velocity_;
	++frame_;
	animFrame_ = (animFrame_ + 1) % 15;
	if (!wasSlashed&&fabsf(pos_.x - player_->Position().x) < 50) {
		updater_ = &Slasher::SlashUpdate;
		drawer_ = &Slasher::SlashDraw;
		animFrame_ = 0;
		frame_ = 0;
		if (effectManager_) {
			float sign = velocity_.x < 0 ? -1.0f : 1.0f;
			effectManager_->EmitBlood(pos_ + Vector2f(sign * 50, 50), sign < 0.0f);
			//effectManager_->EmitBlow3(pos_ + Vector2f(sign * 50, 50), sign < 0.0f);
		}
	}
}

void
Slasher::SlashUpdate() {
	animFrame_ = (animFrame_ + 1) % 20;
	if (animFrame_ == 0) {
		wasSlashed = true;
		frame_ = 1;
		updater_ = &Slasher::RunUpdate;
		drawer_ = &Slasher::RunDraw;
		
	}
}

void 
Slasher::Update() {
	(this->*updater_)();
}


void
Slasher::RunDraw() {
	const auto xoffset = camera_->ViewOffset().x;
	DrawRectRotaGraph(
		static_cast<int>(pos_.x+xoffset), static_cast<int>(pos_.y),
		(animFrame_ / 5) * 36, 0, 36, 26,
		4.0f, 0.0f, runH, true,
		velocity_.x < 0);
}

void
Slasher::SlashDraw() {
	const auto xoffset=camera_->ViewOffset().x;
	DrawRectRotaGraph(static_cast<int>(pos_.x+xoffset), static_cast<int>(pos_.y),
		(animFrame_ / 5) * 42, 0, 42, 26,
		4.0f, 0.0f, slashH, true,
		velocity_.x < 0);
}
void
Slasher::Draw() {
	(this->*drawer_)();
}
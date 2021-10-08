#include "Samurai.h"
#include "../Player/Player.h"
#include"../Effect.h"
#include<DxLib.h>
#include"../Camera.h"
#include"../Stage.h"
#include"../../Arithmetic.h"
#include"../../System/FileManager.h"
#include"../../System/File.h"
#include"../../Scene/GameplayingScene.h"
#include"../Collision/CapsuleCollider.h"
#include"../Collision/CollisionManager.h"
#include<random>
#include<cassert>
namespace {
	constexpr float naraku_y = FLT_MAX;//奈落の座標
	constexpr float effect_offset_scale = 50.0f;//エフェクトのオフセットスケール
	constexpr float move_speed = 5.0f;//動くスピード
	constexpr int one_picture_frames = 5;//1枚当たりの消費フレーム
	constexpr int damage_picture_frames = 8;//ダメージフレーム
	constexpr int damage_anim_num = 3;//ダメージアニメの枚数
	constexpr int run_anim_num = 8;//走りアニメの枚数
	constexpr int slash_anim_num = 4;//斬りアニメの枚数
	constexpr int run_frames = one_picture_frames * run_anim_num;//走りアニメフレーム数
	constexpr int slash_frames = one_picture_frames * slash_anim_num;//斬りアニメフレーム数
	constexpr int slash_distance = 150;//斬ると判断する自機との距離
	constexpr float jump_power = 20.0f;//ジャンプ力
	constexpr float gravity = 0.75f;//重力
	constexpr int pict_width = 200;
	constexpr int foot_level = 127;//足元の元画像の座標
	constexpr int run_pict_width = 200;
	constexpr int slash_pict_width = 200;

	std::mt19937 mt_;//乱数用
	std::uniform_int_distribution<int> turn_interval_range(120, 180);//反転タイミングの幅
	//描画時の元の絵からの倍率を返す
	inline float draw_scale() {
		return 2.5f;
	}
}

void
Samurai::Dead() {

}

void 
Samurai::OnHit(CollisionInfo& me, CollisionInfo& other) {
	if (other.collider->GetTag() == tag_player_attack) {
		if (updater_ == &Samurai::DamageUpdate) {
			return;
		}
		float sign = Sign(velocity_.x);
		effectManager_->EmitBlow3(pos_ + Vector2f(sign, 0) * effect_offset_scale,
			sign < 0.0f);
		OnDamage(1);
		
	}
}

Samurai::Samurai(const std::shared_ptr<Player>& p, std::shared_ptr<Camera> camera, std::shared_ptr<Stage> stg) :
	Enemy(p, camera),
	stage_(stg)
{
	life_ = 10;
	auto& fileMgr = FileManager::Instance();
	if (runH_ == -1) {
		runH_ = fileMgr.Load(L"Resource/Image/Enemy/Samurai/run.png")->Handle();
	}
	if (HslashH_ == -1) {
		HslashH_ = fileMgr.Load(L"Resource/Image/Enemy/Samurai/HAttack.png")->Handle();
	}
	if (VslashH_ == -1) {
		VslashH_ = fileMgr.Load(L"Resource/Image/Enemy/Samurai/VAttack.png")->Handle();
	}
	if (damageH_ == -1) {
		damageH_ = fileMgr.Load(L"Resource/Image/Enemy/Samurai/damage.png")->Handle();
	}
	lastUpdater_ = &Samurai::IdleUpdate;
	updater_ = &Samurai::RunUpdate;
	drawer_ = &Samurai::RunDraw;
	

}

void 
Samurai::IdleUpdate() {

}

void
Samurai::SetWeakRef(std::weak_ptr<Enemy> ref) {
	weakThis_ = ref;
}

Samurai::Samurai(GameplayingScene* gs) :
	Samurai(gs->GetPlayer(), gs->GetCamera(), gs->GetStage()) {
	effectManager_ = gs->GetEffectManager();
	collisionManager_ = gs->GetCollisionManager();
}

const std::vector<Circle>&
Samurai::GetCircles()const {
	return  circles_;
}

Enemy*
Samurai::MakeClone() {
	return new Samurai(*this);
}
void
Samurai::OnDamage(int damage) {
	life_ -= damage;
	if (life_ <= 0) {
		OnDead();
	}
	else {
		damageTimer_ = 40;
		animFrame_ = 0;
		AimPlayer();
		velocity_ *= move_speed;
		lastUpdater_ = updater_;
		updater_ = &Samurai::DamageUpdate;
		drawer_ = &Samurai::DamageDraw;
	}
}
void
Samurai::OnDead() {
	effectManager_->EmitGroundExplosion(pos_);
	isDeletable_ = true;
}

void
Samurai::RunUpdate() {
	auto rc = camera_->GetViewRange();
	if (IsDivisible(frame_, turn_interval_range(mt_)) || !InRange(pos_.x, rc.LeftF(), rc.RightF())) {
		AimPlayer();
		wasSlashed_ = false;
		velocity_ *= move_speed;
	}
	pos_ += velocity_;
	++frame_;
	animFrame_ = ModuloIncrement(animFrame_, run_frames);
	if (!wasSlashed_ && fabsf(pos_.x - player_->Position().x) < slash_distance) {
		HSlash();
	}


	auto seg3 = stage_->GetThreeSegment(pos_);

	if (seg3[1].IsNil()) {
		pos_.y = naraku_y;
	}
	else {
		auto seg = seg3[1];
		assert(seg.vec.x > 0);
		float a = seg.vec.y / seg.vec.x;
		pos_.y = seg.start.y + a * (pos_.x - seg.start.x);

		//穴の手前に来たら…
		if (velocity_.x > 0) {
			if (seg3[2].IsNil() || seg3[2].vec.x == 0.0f) {
				auto diff = seg.End().x - pos_.x;

				if (0 < diff && diff <= fabsf(velocity_.x) * 2) {
					Jump();
				}
			}
		}
		else {
			if (seg3[0].IsNil() || seg3[0].vec.x == 0.0f) {
				auto diff = pos_.x - seg.start.x;

				if (0 < diff && diff <= fabsf(velocity_.x) * 2) {
					Jump();
				}
			}
		}
	}
}

void Samurai::Jump()
{
	velocity_.y = -jump_power;
	updater_ = &Samurai::JumpUpdate;
}

void 
Samurai::HSlash()
{
	updater_ = &Samurai::HSlashUpdate;
	drawer_ = &Samurai::HSlashDraw;
	animFrame_ = 0;
	frame_ = 0;
}


void 
Samurai::VSlash()
{
	updater_ = &Samurai::VSlashUpdate;
	drawer_ = &Samurai::VSlashDraw;
	animFrame_ = 0;
	frame_ = 0;
}

void
Samurai::VSlashUpdate() {
	velocity_.y += gravity;
	pos_ += velocity_;
	animFrame_ = SaturateAdd(animFrame_, 1, slash_frames-one_picture_frames*2);
	auto groundy = stage_->ComputeGroundY(pos_);
	if (groundy < pos_.y) {
		pos_.y = groundy;
		updater_ = &Samurai::RunUpdate;
		frame_ = 0;
	}
	auto rc = camera_->GetViewRange();

	if (!InRange(pos_.x, rc.LeftF(), rc.RightF())) {
		AimPlayer();
	}
}
void
Samurai::VSlashDraw() {
	const auto xoffset = camera_->ViewOffset().x;
	DrawRectRotaGraph2(static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
		(animFrame_ / 5) * slash_pict_width, 0,
		slash_pict_width, 200,
		slash_pict_width / 2, foot_level,//中心点
		draw_scale(), 0.0f, VslashH_, true,
		velocity_.x < 0);
}


void
Samurai::DamageUpdate() {
	animFrame_ = SaturateAdd(animFrame_, 1, (damage_anim_num * one_picture_frames - 1));
	if (--damageTimer_ == 0) {
		updater_ = lastUpdater_;
		drawer_ = &Samurai::RunDraw;
		if (updater_!= &Samurai::JumpUpdate&&updater_ != &Samurai::FallUpdate){
			if (!wasSlashed_ && fabsf(pos_.x - player_->Position().x) < slash_distance) {
				if (rand() % 5 == 0) {
					HSlash();
				}
				else {
					Jump();
				}
			}
			else {
				Jump();
			}
		}
	}
}

void 
Samurai::DamageDraw() {
	auto rc = camera_->GetViewRange();
	auto xpos = pos_.x;
	int xmargin = run_pict_width * 4;//ちょっとここでは適当に
	if (xpos < rc.Left() - xmargin || rc.Right() + xmargin < xpos) {
		return;
	}

	const auto xoffset = camera_->ViewOffset().x;
	DrawRectRotaGraph2(
		static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
		(animFrame_ / 5) * pict_width, 0,
		pict_width, 200,
		pict_width / 2, foot_level,//中心点
		draw_scale(), 0.0f, damageH_, true,
		velocity_.x < 0);
}

void
Samurai::JumpUpdate() {
	velocity_.y += gravity;
	pos_ += velocity_;
	if (IsFalling(velocity_)) {
		updater_ = &Samurai::FallUpdate;
		frame_ = 0;
	}
}

void
Samurai::FallUpdate() {
	velocity_.y += gravity;
	pos_ += velocity_;
	auto groundy = stage_->ComputeGroundY(pos_);
	if (groundy < pos_.y) {
		pos_.y = groundy;
		updater_ = &Samurai::RunUpdate;
		frame_ = 0;
	}
	auto rc = camera_->GetViewRange();

	if (!InRange(pos_.x, rc.LeftF(), rc.RightF())) {
		AimPlayer();
	}
	if (!wasSlashed_ && fabsf(pos_.x - player_->Position().x) < slash_distance) {
		VSlash();
	}
}


void
Samurai::HSlashUpdate() {
	animFrame_ = ModuloIncrement(animFrame_, slash_frames);
	if (animFrame_ == 0) {
		wasSlashed_ = true;
		frame_ = 1;
		updater_ = &Samurai::RunUpdate;
		drawer_ = &Samurai::RunDraw;
		if (!slashCol_.expired()) {
			slashCol_.lock()->Suicide();
		}
	}
	if (animFrame_ == 2 * one_picture_frames) {
		if ( slashCol_.expired() && !weakThis_.expired()) {
			auto sgn=Sign(velocity_.x);
			slashCol_=collisionManager_->AddCollider(new CapsuleCollider(weakThis_.lock(),
				Capsule({ {Vector2f(0,-50.0f),Vector2f(130.0f,0)*sgn},20.f }), tag_enemy_attack));
		}
	}
	
}

void
Samurai::Update() {
	(this->*updater_)();
}


void
Samurai::RunDraw() {
	auto rc = camera_->GetViewRange();
	auto xpos = pos_.x;
	int xmargin = run_pict_width * 4;//ちょっとここでは適当に
	if (xpos < rc.Left() - xmargin || rc.Right() + xmargin < xpos) {
		return;
	}

	const auto xoffset = camera_->ViewOffset().x;
	DrawRectRotaGraph2(
		static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
		(animFrame_ / 5) * run_pict_width, 0,
		run_pict_width, 200,
		run_pict_width / 2, 127,//中心点
		draw_scale(), 0.0f, runH_, true,
		velocity_.x < 0);
}

void
Samurai::HSlashDraw() {
	const auto xoffset = camera_->ViewOffset().x;
	DrawRectRotaGraph2(static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
		(animFrame_ / 5) * slash_pict_width, 0,
		slash_pict_width, 200,
		slash_pict_width / 2, 127,//中心点
		draw_scale(), 0.0f, HslashH_, true,
		velocity_.x < 0);
}
void
Samurai::Draw() {
	(this->*drawer_)();
}
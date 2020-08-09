#include "Slasher.h"
#include "../Player/Player.h"
#include"../Effect.h"
#include<DxLib.h>
#include"../Collision/Collider.h"
#include"../Camera.h"
#include"../Stage.h"
#include"../../Arithmetic.h"
#include"../../System/FileManager.h"
#include"../../System/File.h"
#include<random>
#include<cassert>
namespace {
	constexpr float naraku_y = FLT_MAX;//�ޗ��̍��W
	constexpr float effect_offset_scale = 50.0f;//�G�t�F�N�g�̃I�t�Z�b�g�X�P�[��
	constexpr float move_speed = 5.0f;//�����X�s�[�h
	constexpr int one_picture_frames = 5;//1��������̏���t���[��
	constexpr int run_anim_num = 3;//����A�j���̖���
	constexpr int slash_anim_num = 4;//�a��A�j���̖���
	constexpr int run_frames = one_picture_frames * run_anim_num;//����A�j���t���[����
	constexpr int slash_frames = one_picture_frames * slash_anim_num;//�a��A�j���t���[����
	constexpr int slash_distance = 50;//�a��Ɣ��f���鎩�@�Ƃ̋���
	constexpr float jump_power = 20.0f;//�W�����v��
	constexpr float gravity=0.75f;//�d��
	constexpr int run_pict_width = 36;
	constexpr int slash_pict_width = 42;
	
	std::mt19937 mt_;//�����p
	std::uniform_int_distribution<int> turn_interval_range(120,180);//���]�^�C�~���O�̕�
	//�`�掞�̌��̊G����̔{����Ԃ�
	inline float draw_scale() {
		return 4.0f;
	}
}

void Slasher::OnHit(CollisionInfo& col){
	if (col.collider->GetTag() == tag_player_attack) {
		float sign = Sign(velocity_.x);
		effectManager_->EmitBlow3(pos_ + Vector2f(sign,0)*effect_offset_scale,
			sign < 0.0f);
		isDeletable_ = true;
	}
}

Slasher::Slasher(const std::shared_ptr<Player>& p, std::shared_ptr<Camera> camera,std::shared_ptr<Stage> stg) :
Enemy(p,camera),
stage_(stg)
{
	auto& fileMgr = FileManager::Instance();
	if (runH == -1) {
		runH = fileMgr.Load(L"Resource/Image/Enemy/Slasher/run.png")->Handle();
	}
	if (slashH == -1) {
		slashH = fileMgr.Load(L"Resource/Image/Enemy/Slasher/slash.png")->Handle();
	}
	updater_ = &Slasher::RunUpdate;
	drawer_ = &Slasher::RunDraw;
	
}

Slasher::Slasher(const std::shared_ptr<Player>& p, std::shared_ptr<EffectManager> efktMng, std::shared_ptr<Camera> camera, std::shared_ptr<Stage> stg):
	Slasher(p,camera,stg) {
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
	auto rc=camera_->GetViewRange();
	if (IsDivisible(frame_ , turn_interval_range(mt_))||!InRange(pos_.x,rc.LeftF(),rc.RightF()) ){
		AimPlayer();
		wasSlashed = false;
		velocity_ *= move_speed;
	}
	pos_ += velocity_;
	++frame_;
	animFrame_ = ModuloIncrement(animFrame_, run_frames);
	if (!wasSlashed&&fabsf(pos_.x - player_->Position().x) < slash_distance) {
		updater_ = &Slasher::SlashUpdate;
		drawer_ = &Slasher::SlashDraw;
		animFrame_ = 0;
		frame_ = 0;
		if (effectManager_) {
			float sign = Sign(velocity_.x);
			effectManager_->EmitBlood(pos_ + Vector2f(sign * 50, 25), sign < 0.0f);
		}
	}


	auto seg3=stage_->GetThreeSegment(pos_);

	if (seg3[1].IsNil()) {
		pos_.y = naraku_y;
	}
	else {
		auto seg = seg3[1];
		assert(seg.vec.x > 0);
		float a = seg.vec.y / seg.vec.x;
		pos_.y=seg.start.y + a * (pos_.x - seg.start.x);

		//���̎�O�ɗ�����c
		if (velocity_.x > 0) {
			if (seg3[2].IsNil() || seg3[2].vec.x==0.0f) {
				auto diff = seg.End().x - pos_.x;
				
				if (0 < diff && diff <= fabsf(velocity_.x)*2) {
					velocity_.y = -jump_power;
					updater_ = &Slasher::JumpUpdate;
				}
			}
		}
		else {
			if (seg3[0].IsNil() || seg3[0].vec.x==0.0f) {
				auto diff = pos_.x - seg.start.x;
				
				if (0 < diff && diff <= fabsf(velocity_.x)*2) {
					velocity_.y = -jump_power;
					updater_ = &Slasher::JumpUpdate;
				}
			}
		}
	}
}

void 
Slasher::JumpUpdate() {
	velocity_.y += gravity;
	pos_ += velocity_;
	if (IsFalling(velocity_)) {
		updater_ = &Slasher::FallUpdate;
		frame_ = 0;
	}
}

void 
Slasher::FallUpdate() {
	velocity_.y += gravity;
	pos_ += velocity_;
	auto groundy = stage_->ComputeGroundY(pos_);
	if (groundy < pos_.y) {
		pos_.y = groundy;
		updater_ = &Slasher::RunUpdate;
		frame_ = 0;
	}
	auto rc = camera_->GetViewRange();

	if (!InRange(pos_.x, rc.LeftF(), rc.RightF())) {
		AimPlayer();
	}
}

void
Slasher::SlashUpdate() {
	animFrame_ = ModuloIncrement(animFrame_,slash_frames);
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
	auto rc = camera_->GetViewRange();
	auto xpos=pos_.x;
	int xmargin = run_pict_width *4;//������Ƃ����ł͓K����
	if (xpos < rc.Left() - xmargin || rc.Right() + xmargin < xpos) {
		return;
	}

	const auto xoffset = camera_->ViewOffset().x;
	DrawRectRotaGraph2(
		static_cast<int>(pos_.x+xoffset), static_cast<int>(pos_.y),
		(animFrame_ / 5) * run_pict_width,	0, 
		run_pict_width, 26,
		run_pict_width/2, 25,//���S�_
		draw_scale(), 0.0f, runH, true,
		velocity_.x < 0);
}

void
Slasher::SlashDraw() {
	const auto xoffset=camera_->ViewOffset().x;
	DrawRectRotaGraph2(static_cast<int>(pos_.x+xoffset), static_cast<int>(pos_.y),
		(animFrame_ / 5) * slash_pict_width, 0, 
		slash_pict_width, 26,
		slash_pict_width/2, 25,//���S�_
		draw_scale(), 0.0f, slashH, true,
		velocity_.x < 0);
}
void
Slasher::Draw() {
	(this->*drawer_)();
}
#include "Thrower.h"
#include"../Collision/CollisionManager.h"
#include"../Collision/CircleCollider.h"
#include"../../Arithmetic.h"
#include"../../Geometry.h"
#include"../Effect.h"
#include"../../Scene/GameplayingScene.h"
#include"../../System/File.h"
#include"../../System/FileManager.h"
#include"../Camera.h"
#include"../../Arithmetic.h"
#include"../Stage.h"
#include"../Player/Player.h"
#include"Kunai.h"
#include"../../Scene/GameplayingScene.h"
#include"../ProjectileManager.h"
#include<DxLib.h>
#include<random>
#include<array>
using namespace std;
namespace {
	constexpr float naraku_y = FLT_MAX;//�ޗ��̍��W
	constexpr float effect_offset_scale = 50.0f;//�G�t�F�N�g�̃I�t�Z�b�g�X�P�[��
	constexpr float move_speed = 5.0f;//�����X�s�[�h
	constexpr int one_picture_frames = 5;//1��������̏���t���[��
	constexpr int throw_one_picture_frames = 10;//1��������̏���t���[��
	constexpr int throw_anim_num = 5;//�N�i�C�����A�j���̖���
	constexpr int jump_anim_num = 4;//�a��A�j���̖���
	constexpr int run_anim_num = 3;//���閇��
	constexpr float kunai_offset_y = -48.0f;
	constexpr int run_frames = one_picture_frames * run_anim_num;//����A�j���t���[����
	constexpr int throw_frames = throw_one_picture_frames * throw_anim_num;//�N�i�C�����A�j���t���[����
	constexpr int jump_frames = one_picture_frames * jump_anim_num;//�W�����v(�㏸�����~)�A�j���t���[����
	constexpr int throw_distance = 300;//������Ɣ��f���鎩�@�Ƃ̋���
	constexpr int looseness_distance = 50;//���f�̗V��
	constexpr float jump_power = 20.0f;//�W�����v��
	constexpr float gravity = 0.75f;//�d��
	constexpr int run_pict_width = 36;//����摜
	constexpr int throw_pict_width = 40;//�N�i�C�����摜1��������̕�
	constexpr int jump_pict_width = 32;//�W�����v�摜1��������̕�

	std::mt19937 mt_;//�����p
	std::uniform_int_distribution<int> turn_interval_range(120, 180);//���]�^�C�~���O�̕�
	//�`�掞�̌��̊G����̔{����Ԃ�
	inline float draw_scale() {
		return 4.0f;
	}

}
void 
Thrower::OnHit(CollisionInfo& me, CollisionInfo& other) {
	if (other.collider->GetTag() == tag_player_attack) {
		float sign = Sign(velocity_.x);
		effectManager_->EmitBlow3(pos_ + Vector2f(sign, 0) * effect_offset_scale,
			sign < 0.0f);
		isDeletable_ = true;
	}
}

Thrower::Thrower(GameplayingScene* gs) :Enemy(gs->GetPlayer(), gs->GetCamera()),
	stage_(gs->GetStage()),
	effectManager_(gs->GetEffectManager()),
	collisionManager_(gs->GetCollisionManager()),
	projectileManager_(gs->GetProjectileManager())
{
	life_ = 1;
	auto& fileMgr = FileManager::Instance();
	if (throwH_ == -1) {
		throwH_ = fileMgr.Load(L"Resource/Image/Enemy/Thrower/throw.png")->Handle();
	}
	if (jumpH_ == -1) {
		jumpH_ = fileMgr.Load(L"Resource/Image/Enemy/Slasher/jump_and_fall.png")->Handle();
	}
	if (runH_ == -1) {
		runH_ = fileMgr.Load(L"Resource/Image/Enemy/Thrower/run.png")->Handle();
	}
	if (idleH_ == -1) {
		idleH_ = fileMgr.Load(L"Resource/Image/Enemy/Slasher/idle.png")->Handle();
	}
	updater_ = &Thrower::RunUpdate;
	drawer_ = &Thrower::RunDraw;

}


const std::vector<Circle>&
Thrower::GetCircles()const {
	return  circles_;
}

Enemy*
Thrower::MakeClone() {
	return new Thrower(*this);
}
void
Thrower::OnDamage(int damage) {

}
void
Thrower::OnDead() {
}

void
Thrower::RunUpdate() {
	auto rc = camera_->GetViewRange();
	if (IsDivisible(frame_, turn_interval_range(mt_)) || !InRange(pos_.x, rc.LeftF(), rc.RightF())) {
		AimPlayer();
		velocity_ *= move_speed;
	}
	pos_ += velocity_;
	++frame_;
	animFrame_ = ModuloIncrement(animFrame_, run_frames);
	if ( fabsf(pos_.x - player_->Position().x) < throw_distance) {
		if (rand() % 5 == 0) {
			SpecialAttack();
		}
		else {
			updater_ = &Thrower::ThrowUpdate;
			drawer_ = &Thrower::ThrowDraw;
			AimPlayer();
			animFrame_ = 0;
			frame_ = 0;
		}
	}
	AdjustGround();
}

void 
Thrower::RunawayUpdate() {
	auto rc = camera_->GetViewRange();
	if (IsDivisible(frame_, turn_interval_range(mt_)) || !InRange(pos_.x, rc.LeftF(), rc.RightF())) {
		AimPlayer();
		velocity_.x = -velocity_.x;//�v���C���[���痣���悤��
		velocity_ *= move_speed;
	}
	pos_ += velocity_;
	++frame_;
	animFrame_ = ModuloIncrement(animFrame_, run_frames);
	if (fabsf(pos_.x - player_->Position().x) < throw_distance && fabsf(pos_.x - player_->Position().x) > throw_distance-looseness_distance) {
		if (rand() % 3 == 0) {
			SpecialAttack();
		}
		else {
			updater_ = &Thrower::ThrowUpdate;
			drawer_ = &Thrower::ThrowDraw;
			AimPlayer();
			animFrame_ = 0;
			frame_ = 0;
		}
	}


	AdjustGround();
}

void Thrower::AdjustGround()
{
	auto seg3 = stage_->GetThreeSegment(pos_);

	if (seg3[1].IsNil()) {
		pos_.y = naraku_y;
	}
	else {
		auto seg = seg3[1];
		assert(seg.vec.x > 0);
		float a = seg.vec.y / seg.vec.x;
		pos_.y = seg.start.y + a * (pos_.x - seg.start.x);

		//���̎�O�ɗ�����c
		if (velocity_.x > 0) {
			if (seg3[2].IsNil() || seg3[2].vec.x == 0.0f) {
				auto diff = seg.End().x - pos_.x;

				if (0 < diff && diff <= fabsf(velocity_.x) * 2) {
					velocity_.y = -jump_power;
					updater_ = &Thrower::JumpUpdate;
				}
			}
		}
		else {
			if (seg3[0].IsNil() || seg3[0].vec.x == 0.0f) {
				auto diff = pos_.x - seg.start.x;

				if (0 < diff && diff <= fabsf(velocity_.x) * 2) {
					velocity_.y = -jump_power;
					updater_ = &Thrower::JumpUpdate;
				}
			}
		}
	}
}

void 
Thrower::SpecialAttack() {
	AimPlayer();
	velocity_.y = -jump_power;
	updater_ = &Thrower::SpecialAttackUpdate;
	frame_ = 0;
	animFrame_ = 0;
}

void 
Thrower::SpecialAttackUpdate() {
	auto lastvelY = velocity_.y;
	if (specialAttackTimer_ == 0) {
		velocity_.y += gravity;
		pos_ += velocity_;
	}
	if (IsFalling(velocity_)) {
		if (lastvelY < 0.0f) {
			specialAttackTimer_ = 40;
			auto vel = player_->GetPosition() - pos_;
			
			lockonAngle_ = atan2f(vel.y, vel.x);
			addAngle_ = (vel.x > 0 ? -1.0f : 1.0f) * (DX_PI_F / 18.0f);
			lockonAngle_ -= addAngle_ * 4.0f;
		}

		if (--specialAttackTimer_ == 0) {
			updater_ = &Thrower::FallUpdate;
			frame_ = 0;
		}
		else {
			if (specialAttackTimer_ % 5 == 0) {
				Vector2f vel(cos(lockonAngle_), sinf(lockonAngle_));
				vel = vel.Normalized() * 4.0f;
				lockonAngle_ += addAngle_;
				auto& pm = projectileManager_;
				pm.AddProjectile(new Kunai(pos_ + Vector2f(0, kunai_offset_y) + vel, vel, camera_, effectManager_));

				//�e�ɃR���W����������
				auto colMgr = collisionManager_;
				colMgr->AddCollider(new CircleCollider(pm.GetProjectiles().back(),
					Circle(Position2f::ZERO, 5.0f),
					tag_enemy_bullet));
			}
		}
	}
}

void
Thrower::JumpUpdate() {
	velocity_.y += gravity;
	pos_ += velocity_;
	if (IsFalling(velocity_)) {
		updater_ = &Thrower::FallUpdate;
		frame_ = 0;
	}
}

void
Thrower::FallUpdate() {
	velocity_.y += gravity;
	pos_ += velocity_;
	auto groundy = stage_->ComputeGroundY(pos_);
	if (groundy < pos_.y) {
		pos_.y = groundy;
		updater_ = &Thrower::RunUpdate;
		frame_ = 0;
	}
	auto rc = camera_->GetViewRange();

	if (!InRange(pos_.x, rc.LeftF(), rc.RightF())) {
		AimPlayer();
	}
}

void
Thrower::ThrowUpdate() {
	animFrame_ = ModuloIncrement(animFrame_, throw_frames);
	if (animFrame_ == throw_frames-throw_one_picture_frames*2) {
		Vector2f vel = player_->GetPosition() - pos_;
		vel=vel.Normalized()* 6.0f;
		auto& pm = projectileManager_;
		pm.AddProjectile(new Kunai(pos_+Vector2f(0, kunai_offset_y) + vel, vel, camera_, effectManager_));

		//�e�ɃR���W����������
		auto colMgr = collisionManager_;
		colMgr->AddCollider(new CircleCollider(pm.GetProjectiles().back(),
			Circle(Position2f::ZERO, 5.0f),
			tag_enemy_bullet));
	}
	else {
		if (fabsf(player_->GetPosition().x - pos_.x) > throw_distance) {
			updater_ = &Thrower::RunUpdate;
			drawer_ = &Thrower::RunDraw;
		}
		else if (fabsf(player_->GetPosition().x - pos_.x) < throw_distance - looseness_distance) {
			updater_ = &Thrower::RunawayUpdate;
			drawer_ = &Thrower::RunDraw;
		}
		else if(animFrame_ == throw_frames - 1){
			if (rand() % 7 == 0) {
				SpecialAttack();
			}
		}
	}
}

void
Thrower::Update() {
	(this->*updater_)();
}


void
Thrower::RunDraw() {
	auto rc = camera_->GetViewRange();
	auto xpos = pos_.x;
	int xmargin = run_pict_width * 4;//������Ƃ����ł͓K����
	if (xpos < rc.Left() - xmargin || rc.Right() + xmargin < xpos) {
		return;
	}

	const auto xoffset = camera_->ViewOffset().x;
	DrawRectRotaGraph2(
		static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
		(animFrame_ / 5) * run_pict_width, 0,
		run_pict_width, 26,
		run_pict_width / 2, 25,//���S�_
		draw_scale(), 0.0f, runH_, true,
		velocity_.x < 0);
}

void
Thrower::ThrowDraw() {
	const auto xoffset = camera_->ViewOffset().x;
	DrawRectRotaGraph2(static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
		(animFrame_ / throw_one_picture_frames) * throw_pict_width, 0,
		throw_pict_width, 28,
		throw_pict_width / 2, 27,//���S�_
		draw_scale(), 0.0f, throwH_, true,
		velocity_.x < 0);
}
void
Thrower::Draw() {
	(this->*drawer_)();
}
#include "Asura.h"
#include"../../Arithmetic.h"
#include"../Camera.h"
#include"../../System/File.h"
#include"../../System/FileManager.h"
#include"../Collision/Collider.h"
#include"../../Scene/GameplayingScene.h"
#include"../ProjectileManager.h"
#include"AsuraBullet.h"
#include"../Effect.h"
#include"../Collision/CollisionManager.h"
#include"../Collision/CircleCollider.h"
#include<DxLib.h>
#include<random>

using namespace std;

namespace {
	float ground_line = 0.0f;
	float entering_speed = 3.0f;
	constexpr float draw_scale = 1.2f;
	constexpr float chichu_y = 1100.f;//���傤�ǑS�g�B���[��
	mt19937 mt_(1);
	uniform_int_distribution<int> rnd_range_(80,180);
	uniform_real_distribution<float> bullet_angle_range_(0.0f,DX_PI_F/4.0f);
}

Enemy*
Asura::MakeClone() {
	return new Asura(*this);
}

Asura::Asura(GameplayingScene* gs):Boss(gs) {
	updater_ = &Asura::EnteringUpdate;
	drawer_ = &Asura::NormalDraw;

	auto& fileMgr = FileManager::Instance();
	ashuraH_ = fileMgr.Load(L"Resource/Image/Enemy/asura/ashura.png")->Handle();

	auto rc = camera_->GetViewRange();
	//���E�����ɔz�u
	pos_.x = static_cast<float>(rc.Left() + rc.Right()) / 2.0f;
	pos_.y = chichu_y;//�n���[����
	ground_line = rc.Height() - 16.0f;//���傤�ǃ{�X��̒n�ʂ̍���
	circles_.emplace_back(Position2f(0.0f, -400.0f), 50.0f);

	life_ = 50;
	effectManager_ = gs->GetEffectManager();
	for (auto& b : energyBalls_) {
		b.frame = rnd_range_(mt_);
	}
}

//�o�ꉉ�o���
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

//�ʏ���
void 
Asura::NormalUpdate() {
	for (auto& b:energyBalls_) {
		if (--b.frame <= 0) {
			b.frame = rnd_range_(mt_);
			auto pos = b.pos + pos_;
			effectManager_->EmitEnergyBall(pos);
			float angle = bullet_angle_range_(mt_);
			for(int i=0;i<8;++i){
				Vector2f vel(cosf(angle), sinf(angle));
				vel *= 4.0f;
				auto& pm = gameScene_->GetProjectileManager();
				pm.AddProjectile(new AsuraBullet(pos + vel, vel, camera_,gameScene_->GetEffectManager()));

				//�e�ɃR���W����������
				auto colMgr=gameScene_->GetCollisionManager();
				colMgr->AddCollider(new CircleCollider(pm.GetProjectiles().back(),
					Circle(Position2f::ZERO, 5.0f),
					tag_enemy_bullet));
				angle += DX_PI_F / 4.0f;
			}
		}
	}
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

//�ޏꉉ�o���
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
		float x = static_cast<float>(rand() % w) - w / 2.0f;
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

//�ʏ�`��
void 
Asura::NormalDraw() {
	int w, h;
	GetGraphSize(ashuraH_, &w, &h);
	const auto xoffset = camera_->ViewOffset().x;
	int x = static_cast<int>(pos_.x + xoffset);
	int y = static_cast<int>(pos_.y);
	DrawRotaGraph2(
		x,y,
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
	pos_.x += ((frame_ % 3) - 1)*3;
	NormalDraw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
	NormalDraw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void 
Asura::OnHit(CollisionInfo& me, CollisionInfo& other) {
	if (other.collider->GetTag() == tag_player_attack) {
		if (updater_ == &Asura::NormalUpdate) {
			OnDamage(1);
		}
	}
}

// �U�����󂯂�
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




// ���C�x���g
void 
Asura::OnDead() {
	updater_ = &Asura::ExitingUpdate;
	drawer_ = &Asura::ExitingDraw;
	frame_ = 0;
}

// �X�V
void 
Asura::Update() {
	(this->*updater_)();
}

// �`��
void 
Asura::Draw() {
	(this->*drawer_)();
}
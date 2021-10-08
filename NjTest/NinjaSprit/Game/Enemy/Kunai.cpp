#include "Kunai.h"
#include<random>
#include<DxLib.h>
#include"../../System/FileManager.h"
#include"../../System/File.h"
#include"../Camera.h"
#include"../Collision/CircleCollider.h"
#include"../Collision/CollisionManager.h"
#include"../Effect.h"
using namespace std;

namespace {
	mt19937 mt_;
	uniform_real_distribution<float> dt_(-0.5f, 0.5f);
}

/// <summary>
/// ���C���e�R���X�g���N�^
/// </summary>
/// <param name="pos">�������W</param>
/// <param name="vel">�������x</param>
/// <param name="camera">�J����</param>
Kunai::Kunai(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> camera, std::shared_ptr<EffectManager> efk) :Projectile(camera), effectMgr_(efk) {
	vel_ = vel;
	initVel_ = vel;
	pos_ = pos;
	speed_ = vel.Magnitude();
	centripetalSpeed_ = 0.0f;// dt_(mt_);
	auto& fileMgr = FileManager::Instance();
	if (bulletH_ == -1) {
		bulletH_ = fileMgr.Load(L"Resource/Image/Enemy/Thrower/kunai.png")->Handle();
	}
}
Kunai::~Kunai() {

}

/// <summary>
/// ���t���[���X�V
/// </summary>
void
Kunai::Update() {
	const auto& vrect = camera_->GetViewRange();
	if (pos_.x >= vrect.Right() || pos_.x <= vrect.Left() || pos_.y >= vrect.Bottom() || pos_.y <= vrect.Top()) {
		isActive_ = false;
	}
	Vector2f centripetalVel(-vel_.y, vel_.x);
	centripetalVel.Normalize();
	centripetalVel *= centripetalSpeed_;
	vel_ = (vel_ * 5.0f + (vel_ + centripetalVel).Normalized() * speed_).Normalized() * speed_;
	Projectile::Update();
}
/// <summary>
/// ���t���[���`��
/// </summary>
void
Kunai::Draw() {
	auto angle = atan2f(vel_.y, vel_.x);
	const auto xoffset = camera_->ViewOffset().x;
	DxLib::DrawRotaGraph(
		static_cast<int>(pos_.x + xoffset),
		static_cast<int>(pos_.y),
		1.5f, angle, bulletH_, true);
	frame_ = (frame_ + 1) % 5;
}

/// <summary>
/// �Փˎ��C�x���g
/// </summary>
/// <param name="colInfo">�Փˏ��</param>
void
Kunai::OnHit(CollisionInfo& me, CollisionInfo& other) {
	if (other.collider->GetTag() == tag_player_attack || other.collider->GetTag() == tag_player_damage) {//�v���C���[�̍U����������
		auto rnd = rand() % 2;
		effectMgr_->EmitEliminate_s(pos_, rnd);//�G�e���ŃG�t�F�N�g
		isActive_ = false;
	}
}
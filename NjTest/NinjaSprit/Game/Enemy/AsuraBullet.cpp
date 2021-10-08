#include "AsuraBullet.h"
#include"../../System/File.h"
#include"../../System/FileManager.h"
#include"../Effect.h"
#include"../Camera.h"
#include"../Collision/Collider.h"
#include<random>
#include<DxLib.h>

using namespace std;

namespace {
	mt19937 mt_;
	uniform_real_distribution<float> dt_(-0.5f,0.5f);
}

/// <summary>
/// 阿修羅弾コンストラクタ
/// </summary>
/// <param name="pos">初期座標</param>
/// <param name="vel">初期速度</param>
/// <param name="camera">カメラ</param>
AsuraBullet::AsuraBullet(const Position2f& pos, const Vector2f& vel, std::shared_ptr<Camera> camera, std::shared_ptr<EffectManager> efk):Projectile(camera),effectMgr_(efk) {
	vel_ = vel;
	initVel_ = vel;
	pos_ = pos;
	speed_ = vel.Magnitude();
	centripetalSpeed_ = 0.0f;// dt_(mt_);
	auto& fileMgr = FileManager::Instance();
	if (bulletH_ == -1) {
		bulletH_ = fileMgr.Load(L"Resource/Image/Enemy/asura/fireball_b.png")->Handle();
	}
}
AsuraBullet::~AsuraBullet() {

}

/// <summary>
/// 毎フレーム更新
/// </summary>
void 
AsuraBullet::Update() {
	const auto& vrect = camera_->GetViewRange();
	if (pos_.x >= vrect.Right() || pos_.x <= vrect.Left() || pos_.y >= vrect.Bottom() || pos_.y <= vrect.Top()) {
		isActive_ = false;
	}
	Vector2f centripetalVel(-vel_.y, vel_.x);
	centripetalVel.Normalize();
	centripetalVel *= centripetalSpeed_;
	vel_ = (vel_*5.0f+(vel_ + centripetalVel).Normalized() * speed_).Normalized()*speed_;
	Projectile::Update();
}
/// <summary>
/// 毎フレーム描画
/// </summary>
void 
AsuraBullet::Draw() {
	auto angle = atan2f(vel_.y, vel_.x);
	const auto xoffset = camera_->ViewOffset().x;
	DxLib::DrawRectRotaGraph2(
		static_cast<int>(pos_.x + xoffset),
		static_cast<int>(pos_.y),
		64*frame_,0,64,32,44,14,
		1.0f, angle, bulletH_, true);
	frame_ = (frame_ + 1) % 5;
}

/// <summary>
/// 衝突時イベント
/// </summary>
/// <param name="colInfo">衝突情報</param>
void 
AsuraBullet::OnHit(CollisionInfo& me, CollisionInfo& other) {
	if (other.collider->GetTag() == tag_player_attack || other.collider->GetTag() == tag_player_damage) {//プレイヤーの攻撃だったら
		auto rnd = rand() % 2;
		effectMgr_->EmitEliminate_s(pos_,rnd);//敵弾消滅エフェクト
		isActive_ = false;
	}
}
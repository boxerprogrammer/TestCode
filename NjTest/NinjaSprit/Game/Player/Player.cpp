#include"Player.h"
#include<sstream>
#include<iomanip>
#include<DxLib.h>
#include"../../Geometry.h"
#include"../../Input/Input.h"
#include"../../Scene/GameplayingScene.h"
#include"SwordEquip.h"
#include"BombEquip.h"
#include"ShurikenEquip.h"
#include"ChainEquip.h"
#include"../Camera.h"
#include"../Stage.h"
#include"../../Geometry.h"
#include"../../System/FileManager.h"
#include"../../System/File.h"
#include"../Collision/Collider.h"
#include"../Effect.h"
#include"../../Arithmetic.h"
#include<cassert>
#include<cmath>
#include"ShadowClone.h"


using namespace std;

namespace {

	
	int frame_ = 0;
	size_t idx_ = 0;
	
	constexpr float collision_radius = 24.0f;
	constexpr float run_speed = 5.0f;
	constexpr float max_gravity = 0.75f;
	constexpr float gravity_rate = 0.125f;
	constexpr float jump_velocity_y = -25.0f;
	constexpr float block_scale = 3.0f;
	constexpr float radY = 18.0f * 3.0f;
	constexpr float epsilon = 0.01f;
}
Player::Player(GameplayingScene* gs) :Character(gs->GetCamera()) {
	auto& fileMgr = FileManager::Instance();
	//走りロード
	for (int i = 0; i < _countof(runH_); ++i) {
		wstringstream wss;
		wss << L"Resource/Image/Player/";
		wss << L"adventurer-run-";
		wss << setfill(L'0') << setw(2);
		wss << i << L".png";
		if (runH_[i] == -1) {
			runH_[i] = fileMgr.Load(wss.str().c_str())->Handle();
		}
	}
	//ジャンプロード
	for (int i = 0; i < _countof(jumpH_); ++i) {
		wstringstream wss;
		wss << L"Resource/Image/Player/";
		wss << L"adventurer-jump-";
		wss << setfill(L'0') << setw(2);
		wss << i << L".png";
		if (jumpH_[i] == -1) {
			jumpH_[i] = fileMgr.Load(wss.str().c_str())->Handle();
		}
	}
	//下降ロード
	for (int i = 0; i < _countof(fallH_); ++i) {
		wstringstream wss;
		wss << L"Resource/Image/Player/";
		wss << L"adventurer-fall-";
		wss << setfill(L'0') << setw(2);
		wss << i << L".png";
		if (fallH_[i] == -1) {
			fallH_[i] = fileMgr.Load(wss.str().c_str())->Handle();
		}
	}
	maskH_ = fileMgr.Load(L"Resource/Image/Player/shadow_mask.bmp")->Handle();
	frame_ = 0;
	idx_ = 0;

	class PlayerInputListener : public InputListener {
	private:
		Player& player_;
		bool decidedGravity_ = false;
	public:
		PlayerInputListener(Player& p) :player_(p) {}
		~PlayerInputListener() {
		}
		void Notify(const Input& input)override {
			if (input.IsPressed("up")) {
				
			}
			if (input.IsPressed("down")) {
				//しゃがみ
			}
			if (input.IsPressed("left")) {
				if (player_.updater_ != &Player::DamageUpdate) {
					player_.Move(Vector2f::LEFT * run_speed);
					player_.isRight_ = false;
				}
			}
			if (input.IsPressed("right")) {
				if (player_.updater_ != &Player::DamageUpdate) {
					player_.Move(Vector2f::RIGHT * run_speed);
					player_.isRight_ = true;
				}
			}
			if (input.IsTriggered("shot")) {
				if (player_.updater_ != &Player::DamageUpdate) {
					player_.Attack(input);
				}
			}
			if (input.IsTriggered("change")) {
				player_.NextEquip();
			}
			if (input.IsTriggered("jump")) {
				if (player_.updater_ != &Player::DamageUpdate) {
					if (player_.updater_ == &Player::NormalUpdate) {
						player_.Jump();
						decidedGravity_ = false;
					}
				}
			}
			if (input.IsReleased("jump")) {
				if (player_.updater_ != &Player::DamageUpdate) {
					if (player_.updater_ == &Player::RiseUpdate && !decidedGravity_) {
						player_.accelY_ = max(-player_.velY_ * gravity_rate, max_gravity);
						decidedGravity_ = true;
					}
				}
			}
			player_.AdditionalInput(input);
		}
	};
	collisionManager_ = gs->GetCollisionManager();
	gs->AddListener(make_shared< PlayerInputListener>(*this));
	equipments_[sword_equip_no]= (make_shared<SwordEquip>(gs->GetPlayer(), gs->GetCollisionManager(), gs->GetCamera()));
	equipments_[bomb_equip_no]=(make_shared<BombEquip>(gs->GetProjectileManager(), gs->GetCollisionManager(), gs->GetCamera()));
	equipments_[shuriken_equip_no]=(make_shared<ShurikenEquip>(gs->GetProjectileManager(), gs->GetCollisionManager(), gs->GetCamera()));
	equipments_[chain_equip_no]=(make_shared<ChainEquip>(gs->GetPlayer(), gs->GetCollisionManager(), gs->GetCamera()));
	if (changeSE_ == -1) {
		changeSE_ = fileMgr.Load(L"Resource/Sound/Game/changeweapon.wav")->Handle();
	}
	updater_ = &Player::InitUpdate;
	drawer_ = &Player::NormalDraw;

	fill(moveHistory_.begin(), moveHistory_.end(), pos_);
	lastPos_ = pos_;
	gameScene_ = gs;
	accelY_ = max_gravity;

}

Vector2f Player::GetVelocity()const {
	return pos_ - lastPos_;
}

void
Player::InitUpdate() {
	shadowClones_.push_back(make_shared<ShadowClone>(gameScene_, this, camera_));
	shadowClones_.push_back(make_shared<ShadowClone>(gameScene_, this, camera_));
	updater_ = &Player::NormalUpdate;
}
Player::~Player() {
}

void
Player::AdditionalInput(const Input& input) {
	CurrentEquipment()->AdditionalInput(input);
	for (auto& shadow : shadowClones_) {
		shadow->AdditionalInput(input);
	}
}

shared_ptr<Equipment>
Player::CurrentEquipment() {
	return equipments_[currentEquipmentNo_];
}
size_t
Player::CurrentEquipmentNo()const {
	return currentEquipmentNo_;
}

void
Player::Attack(const Input& input) {
	equipments_[currentEquipmentNo_]->Attack(*this, input);
	for (auto& shadow : shadowClones_) {
		shadow->Attack(input, currentEquipmentNo_);
	}
}

void
Player::NextEquip() {
	currentEquipmentNo_ = (currentEquipmentNo_ + 1) % equipments_.size();
	PlaySoundMem(changeSE_, DX_PLAYTYPE_BACK);
}

void
Player::SetPosition(const Position2f& p) {
	pos_ = p;
	fill(moveHistory_.begin(), moveHistory_.end(), pos_);
	lastPos_ = pos_;
}

const Position2f&
Player::Position()const {
	return pos_;
}

void
Player::Move(const Vector2f& v) {
	auto rc = camera_->GetViewRange();
	pos_ = Clamp(pos_+v, Vector2f( rc.LeftF(),rc.TopF() ), Vector2f( rc.RightF(),rc.BottomF() ));

}

void
Player::Jump() {
	accelY_ = max_gravity;
	velY_ = jump_velocity_y;
	updater_ = &Player::RiseUpdate;
	drawer_ = &Player::RiseDraw;
	frame_ = 0;
	idx_ = 0;
}

void 
Player::DamageUpdate() {
	if (--knockbackFrame_==0) {
		updater_ = &Player::NormalUpdate;
	}
}

void
Player::NormalUpdate() {
	if (frame_ % 5 == 0) {
		idx_ = (idx_ + 1) % _countof(runH_);
	}
	auto groundy = gameScene_->GetStage()->ComputeGroundY(pos_);
	if (groundy< pos_.y) {
		pos_.y = groundy;
	}
	if (groundy > pos_.y) {
		updater_ = &Player::FallUpdate;
		drawer_ = &Player::FallDraw;
	}


}

void
Player::RiseUpdate() {
	if (frame_ % 5 == 0) {
		idx_ = (idx_ + 1) % _countof(jumpH_);
	}
	velY_ += accelY_;
	pos_.y += velY_;
	if (velY_ >= 0.0f) {
		updater_ = &Player::FallUpdate;
		drawer_ = &Player::FallDraw;
		frame_ = 0;
		idx_ = 0;
	}
}

void
Player::FallUpdate() {
	if (frame_ % 5 == 0) {
		idx_ = (idx_ + 1) % _countof(fallH_);
	}
	velY_ += accelY_;
	pos_.y += velY_;
	auto groundy=gameScene_->GetStage()->ComputeGroundY(pos_);
	if (groundy < pos_.y) {
		velY_ = 0.0f;
		pos_.y = groundy;
		updater_ = &Player::NormalUpdate;
		drawer_ = &Player::NormalDraw;
		frame_ = 0;
		idx_ = 0;
	}
}


void
Player::SetCurrentPosition(Position2f& pos) {
	moveHistory_[currentMoveIndex_] = pos;
	currentMoveIndex_ = (currentMoveIndex_ + 1) % moveHistory_.size();
}
const Position2f&
Player::GetBackTimePosition(size_t backFrame)const {
	assert(backFrame < moveHistory_.size());
	auto idx = (currentMoveIndex_ + moveHistory_.size() - backFrame) % moveHistory_.size();
	return moveHistory_[idx];
}


void
Player::Update() {
	//共通部分
	++frame_;
	equipments_[currentEquipmentNo_]->Update();

	auto vec = gameScene_->GetStage()->ComputeOverlapWall(pos_, collision_radius);
	pos_ += vec;

	if (pos_ != lastPos_) {
		SetCurrentPosition(pos_);
	}
	lastPos_ = pos_;

	//個別部分
	(this->*updater_)();

	int time = 16;
	for (auto& shadow : shadowClones_) {
		shadow->Update(GetBackTimePosition(time));
		time += 16;
	}
}

Player::Direction
Player::GetDirection()const {
	Direction ret = Direction::left;
	if (isRight_) {
		ret = Direction::right;
	}
	return ret;
}

void
Player::NormalDraw() {

	auto xoffset = camera_->ViewOffset().x;
	if (updater_ == &Player::DamageUpdate) {
		xoffset += (knockbackFrame_ % 2) * 4;
	}
	
	auto gH = runH_[idx_];
	int w, h;
	GetGraphSize(gH, &w, &h);
	DrawRotaGraph2(static_cast<int>(pos_.x + xoffset),
		static_cast<int>(pos_.y),
		w / 2,
		h - 1,
		block_scale, 0.0f, gH, true, !isRight_, false);
}
void
Player::RiseDraw() {
	auto xoffset = camera_->ViewOffset().x;
	if (updater_ == &Player::DamageUpdate) {
		xoffset += (knockbackFrame_ % 2) * 4;
	}
	auto gH = jumpH_[idx_];
	int w, h;
	GetGraphSize(gH, &w, &h);
	DrawRotaGraph2(static_cast<int>(pos_.x + xoffset),
		static_cast<int>(pos_.y),
		w / 2,
		h - 1,
		block_scale, 0.0f, gH, true, !isRight_, false);
}
void
Player::FallDraw() {
	auto gH = fallH_[idx_];
	int w, h;
	GetGraphSize(gH, &w, &h);
	auto xoffset = camera_->ViewOffset().x;
	if (updater_ == &Player::DamageUpdate) {
		xoffset += (knockbackFrame_ % 2) * 4;
	}
	DrawRotaGraph2(static_cast<int>(pos_.x + xoffset),
		static_cast<int>(pos_.y),
		w / 2,
		h - 1,
		block_scale, 0.0f, gH, true, !isRight_, false);
}


void
Player::Draw() {

	DrawFormatString(400, 10, 0xffffff, L"life=%d", life_);

	equipments_[currentEquipmentNo_]->Draw();

	(this->*drawer_)();

	CreateMaskScreen();
	const auto xoffset = static_cast<int>(camera_->ViewOffset().x);
	auto gH = runH_[idx_];
	int w, h;
	GetGraphSize(gH, &w, &h);
	//分身の描画
	Rect maskRc(- 64 + xoffset,  - 128, 128, 129);
	{
		const auto& spos = GetBackTimePosition(16);
		Position2 p = Vector2FToVector2(spos);
		Rect mrc = maskRc;
		mrc.pos += p;
		
		DrawFillMask(mrc.Left(), mrc.Top(), 
			mrc.Right(),mrc.Bottom(),	maskH_);
		DrawRotaGraph2(static_cast<int>(spos.x + xoffset),
			static_cast<int>(spos.y),
			w / 2,
			h - 1,
			block_scale, 0.0f, gH, true, !isRight_, false);
	}
	{
		const auto& spos = GetBackTimePosition(32);
		Position2 p = Vector2FToVector2(spos);
		Rect mrc = maskRc;
		mrc.pos += p;

		DrawFillMask(mrc.Left(), mrc.Top(),
			mrc.Right(), mrc.Bottom(), maskH_);

		DrawRotaGraph2(static_cast<int>(spos.x + xoffset),
			static_cast<int>(spos.y),
			w / 2,
			h - 1,
			block_scale, 0.0f, gH, true, !isRight_, false);
	}
	DeleteMaskScreen();
	
	for (auto& shadow : shadowClones_) {
		shadow->Draw();
	}

}

void
Player::OnHit(CollisionInfo& me, CollisionInfo& another) {
	//ダメージイベント
	if (updater_ == &Player::DamageUpdate)return;
	if (me.collider->GetTag() == tag_player_damage){
		if (another.collider->GetTag() == tag_enemy_bullet) {
			gameScene_->GetEffectManager()->EmitHitEffect_s(pos_ + Vector2f(0, -40), another.vec.x >= 0);
			updater_ = &Player::DamageUpdate;
			knockbackFrame_ = 6;
			life_ = SaturateSubtract(life_, 1, 0);
		}
		else if (another.collider->GetTag() == tag_enemy_attack) {
			gameScene_->GetEffectManager()->EmitBlood(pos_ + Vector2f(0, -15), another.vec.x >= 0);
			updater_ = &Player::DamageUpdate;
			knockbackFrame_ = 8;
			life_ = SaturateSubtract(life_, 1, 0);
		}
	}
}
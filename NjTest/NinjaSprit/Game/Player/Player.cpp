#include"Player.h"
#include<sstream>
#include<iomanip>
#include<DxLib.h>
#include"../../Geometry.h"
#include"../../Input/Input.h"
#include"../../Scene/GameplayingScene.h"
#include"BombEquip.h"
#include"ShurikenEquip.h"
#include"ChainEquip.h"
#include"../Camera.h"
#include"../Stage.h"
#include<cassert>
#include<cmath>


using namespace std;

namespace {
	int runH_[6] = { -1,-1,-1,-1, -1, -1 };
	int jumpH_[4] = { -1, -1, -1, -1 };
	int fallH_[2] = { -1, -1 };
	int maskH = -1;
	int frame_ = 0;
	size_t idx_ = 0;
	int changeSE_ = -1;
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
	//走りロード
	for (int i = 0; i < _countof(runH_); ++i) {
		wstringstream wss;
		wss << L"Resource/Image/Player/";
		wss << L"adventurer-run-";
		wss << setfill(L'0') << setw(2);
		wss << i << L".png";
		if (runH_[i] == -1) {
			runH_[i] = LoadGraph(wss.str().c_str());
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
			jumpH_[i] = LoadGraph(wss.str().c_str());
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
			fallH_[i] = LoadGraph(wss.str().c_str());
		}
	}
	maskH = LoadMask(L"Resource/Image/Player/shadow_mask.bmp");
	frame_ = 0;
	idx_ = 0;

	class PlayerInputListener : public InputListener {
	private:
		Player& player_;
		bool decidedGravity_ = false;
	public:
		PlayerInputListener(Player& p) :player_(p) {}
		~PlayerInputListener() {
			OutputDebugStringA("\n Listener is deleted \n");
		}
		void Notify(const Input& input)override {
			if (input.IsPressed("up")) {
				
			}
			if (input.IsPressed("down")) {
				//しゃがみ
			}
			if (input.IsPressed("left")) {
				player_.Move(Vector2f::LEFT* run_speed);
				player_.isRight_ = false;
			}
			if (input.IsPressed("right")) {
				player_.Move(Vector2f::RIGHT * run_speed);
				player_.isRight_ = true;
			}
			if (input.IsTriggered("shot")) {
				player_.Attack(input);
			}
			if (input.IsTriggered("change")) {
				player_.NextEquip();
			}
			if (input.IsTriggered("jump")) {
				if (player_.updater_ == &Player::NormalUpdate) {
					player_.Jump();
					decidedGravity_ = false;
				}
			}
			if (input.IsReleased("jump")) {
				if (player_.updater_ == &Player::RiseUpdate && !decidedGravity_) {
					player_.accelY_ = max(-player_.velY_ *gravity_rate,max_gravity);
					decidedGravity_ = true;
				}
			}
			player_.CurrentEquipment()->AdditionalInput(input);
		}
	};
	collisionManager_ = gs->GetCollisionManager();
	gs->AddListener(make_shared< PlayerInputListener>(*this));
	equipments_.push_back(make_shared<BombEquip>(gs->GetProjectileManager(), gs->GetCollisionManager(), gs->GetCamera()));
	equipments_.push_back(make_shared<ShurikenEquip>(gs->GetProjectileManager(), gs->GetCollisionManager(), gs->GetCamera()));
	equipments_.push_back(make_shared<ChainEquip>(gs->GetPlayer(), gs->GetCollisionManager(), gs->GetCamera()));
	if (changeSE_ == -1) {
		changeSE_ = LoadSoundMem(L"Resource/Sound/Game/changeweapon.wav");
	}
	updater_ = &Player::NormalUpdate;
	drawer_ = &Player::NormalDraw;

	fill(moveHistory_.begin(), moveHistory_.end(), pos_);
	lastPos_ = pos_;
	gs_ = gs;
	accelY_ = max_gravity;
}
Player::~Player() {
	for (auto& run : runH_) {
		DxLib::DeleteGraph(run);
	}
	DeleteSoundMem(changeSE_);
	changeSE_ = -1;
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
	pos_ = Clamp(pos_+v, Vector2f( rc.Left(),rc.Top() ), Vector2f( rc.Right(),rc.Bottom() ));

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
Player::NormalUpdate() {
	if (frame_ % 5 == 0) {
		idx_ = (idx_ + 1) % _countof(runH_);
	}
	auto groundy = gs_->GetStage()->ComputeGroundY(pos_);
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
	auto groundy=gs_->GetStage()->ComputeGroundY(pos_);
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

	auto vec = gs_->GetStage()->ComputeOverlapWall(pos_, collision_radius);
	pos_ += vec;

	if (pos_ != lastPos_) {
		SetCurrentPosition(pos_);
	}
	lastPos_ = pos_;

	//個別部分
	(this->*updater_)();



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
	const int xoffset = camera_->ViewOffset().x;
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
	const int xoffset = camera_->ViewOffset().x;
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
	const int xoffset = camera_->ViewOffset().x;
	DrawRotaGraph2(static_cast<int>(pos_.x + xoffset),
		static_cast<int>(pos_.y),
		w / 2,
		h - 1,
		block_scale, 0.0f, gH, true, !isRight_, false);
}


void
Player::Draw() {
	equipments_[currentEquipmentNo_]->Draw();

	(this->*drawer_)();

	CreateMaskScreen();
	const int xoffset = camera_->ViewOffset().x;
	auto gH = runH_[idx_];
	int w, h;
	GetGraphSize(gH, &w, &h);
	{
		const auto& spos = GetBackTimePosition(16);
		DrawFillMask(spos.x - 64 + xoffset, spos.y - 128, spos.x + 64 + xoffset, spos.y+1,
			maskH);
		DrawRotaGraph2(static_cast<int>(spos.x + xoffset),
			static_cast<int>(spos.y),
			w / 2,
			h - 1,
			block_scale, 0.0f, gH, true, !isRight_, false);
	}
	{
		const auto& spos = GetBackTimePosition(32);
		DrawFillMask(spos.x - 64 + xoffset, spos.y - 128, spos.x + 64 + xoffset, spos.y + 1,
			maskH);
		DrawRotaGraph2(static_cast<int>(spos.x + xoffset),
			static_cast<int>(spos.y),
			w / 2,
			h - 1,
			block_scale, 0.0f, gH, true, !isRight_, false);
	}
	DeleteMaskScreen();
	

}

void
Player::OnHit(CollisionInfo&) {

}
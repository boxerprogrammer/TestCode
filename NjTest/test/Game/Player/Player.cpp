#include"Player.h"
#include<sstream>
#include<iomanip>
#include<DxLib.h>
#include"../../Geometry.h"
#include"../../Input.h"
#include"../../Scene/GameplayingScene.h"
#include"BombEquip.h"
#include"ShurikenEquip.h"
#include"ChainEquip.h"
#include"../Camera.h"
#include<cassert>


using namespace std;

namespace {
	int runH_[6] = { -1,-1,-1,-1, -1, -1 };
	int jumpH_[4] = { -1, -1, -1, -1 };
	int fallH_[2] = { -1, -1 };
	int maskH = -1;
	int frame_ = 0;
	int idx_ = 0;
	int changeSE_ = -1;
	constexpr int ground_line = 480;
	constexpr float max_gravity = 0.75f;
	constexpr float gravity_rate = 0.125f;
	constexpr float jump_velocity_y = -25.0f;
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
				//player_.Move({ 0, -5 });
			}
			if (input.IsPressed("down")) {
				//player_.Move({ 0, 5 });
			}
			if (input.IsPressed("left")) {
				player_.Move({ -5, 0 });
				player_.isRight_ = false;
			}
			if (input.IsPressed("right")) {
				player_.Move({ 5, 0 });
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
	//for (auto& spos : shadowPositions_) {
	//	spos = pos_;
	//}
	fill(moveHistory_.begin(), moveHistory_.end(), pos_);
	lastPos_ = pos_;
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
	pos_ += v;
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
	if (ground_line < pos_.y) {
		velY_ = 0.0f;
		pos_.y = ground_line;
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
	DrawRotaGraph(static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
		3.0f, 0.0f, runH_[idx_], true, !isRight_, false);
}
void
Player::RiseDraw() {
	const int xoffset = camera_->ViewOffset().x;
	DrawRotaGraph(static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
		3.0f, 0.0f, jumpH_[idx_], true, !isRight_, false);
}
void
Player::FallDraw() {
	const int xoffset = camera_->ViewOffset().x;
	DrawRotaGraph(static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
		3.0f, 0.0f, fallH_[idx_], true, !isRight_, false);
}


void
Player::Draw() {
	equipments_[currentEquipmentNo_]->Draw();

	(this->*drawer_)();

	CreateMaskScreen();
	const int xoffset = camera_->ViewOffset().x;
	{
		const auto& spos = GetBackTimePosition(16);
		DrawFillMask(spos.x - 64 + xoffset, spos.y - 64, spos.x + 64 + xoffset, spos.y + 64,
			maskH);
		DrawRotaGraph(static_cast<int>(spos.x + xoffset), static_cast<int>(spos.y),
			3.0f, 0.0f, runH_[idx_], true, !isRight_, false);
	}
	{
		const auto& spos = GetBackTimePosition(32);
		DrawFillMask(spos.x - 64 + xoffset, spos.y - 64, spos.x + 64 + xoffset, spos.y + 64,
			maskH);
		DrawRotaGraph(static_cast<int>(spos.x + xoffset), static_cast<int>(spos.y),
			3.0f, 0.0f, runH_[idx_], true, !isRight_, false);
	}

	DeleteMaskScreen();
}

void
Player::OnHit(CollisionInfo&) {

}
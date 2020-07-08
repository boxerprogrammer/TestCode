#include"Player.h"
#include<sstream>
#include<iomanip>
#include<DxLib.h>
#include"../Geometry.h"
#include"../Input.h"
#include"../Scene/GameplayingScene.h"
#include"BombEquip.h"
#include"ShurikenEquip.h"
#include"ChainEquip.h"


using namespace std;

namespace {
	int runH_[6];
	int frame_ = 0;
	int idx_ = 0;
	int changeSE_ = -1;
}

Player::Player(GameplayingScene* gs) {
	for (int i = 0; i < _countof(runH_); ++i) {
		wstringstream wss;
		wss << L"Resource/Image/Player/";
		wss << L"adventurer-run-";
		wss << setfill(L'0') << setw(2);
		wss << i << L".png";
		runH_[i] = LoadGraph(wss.str().c_str());
	}
	frame_ = 0;
	idx_ = 0;
		  
	class PlayerInputListener : public InputListener{
	private:
		Player& player_;
	public:
		PlayerInputListener(Player& p):player_(p){}
		~PlayerInputListener() {
			OutputDebugStringA("\n Listener is deleted \n");
		}
		void Notify(const Input& input)override{
			if (input.IsPressed("up")) {
				//player_.Move({ 0, -5 });
			}
			if (input.IsPressed("down")) {
				//player_.Move({ 0, 5 });
			}
			if (input.IsPressed("left")) {
				//player_.Move({ -5, 0 });
				player_.isRight_ = false;
			}
			if (input.IsPressed("right")) {
				//player_.Move({ 5, 0 });
				player_.isRight_ = true;
			}
			if (input.IsTriggered("shot")) {
				player_.Attack(input);
			}
			if (input.IsTriggered("change")) {
				player_.NextEquip();
			}
		}
	};
	gs->AddListener(make_shared< PlayerInputListener>(*this));
	equipments_.push_back(make_shared<BombEquip>(gs->GetProjectileManager()));
	equipments_.push_back(make_shared<ShurikenEquip>(gs->GetProjectileManager()));
	equipments_.push_back(make_shared<ChainEquip>(*this));
	if (changeSE_ == -1) {
		changeSE_ = LoadSoundMem(L"Resource/Sound/Game/changeweapon.wav");
	}
}
Player::~Player() {
	for (auto& run : runH_) {
		DxLib::DeleteGraph(run);
	}
	DeleteSoundMem(changeSE_);
	changeSE_ = -1;
}

void 
Player::Attack(const Input& input) {
	equipments_[currentEquipmentNo_]->Attack(*this,input);
}

void 
Player::NextEquip() {
	currentEquipmentNo_ = (currentEquipmentNo_ + 1) % equipments_.size();
	PlaySoundMem(changeSE_, DX_PLAYTYPE_BACK);
}

void 
Player::SetPosition(const Position2f& p) {
	pos_ = p;
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
Player::Update() {
	++frame_;
	if (frame_ % 5 == 0) {
		idx_ = (idx_ + 1) % _countof(runH_);
	} 
	equipments_[currentEquipmentNo_]->Update();
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
Player::Draw() {
	equipments_[currentEquipmentNo_]->Draw();
	DrawRotaGraph(pos_.x, pos_.y, 3.0f, 0.0f, runH_[idx_], true,!isRight_, false);
}
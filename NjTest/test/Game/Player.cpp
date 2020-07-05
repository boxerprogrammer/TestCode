#include"Player.h"
#include<sstream>
#include<iomanip>
#include<DxLib.h>
#include"../Geometry.h"
#include"../Input.h"
#include"../Scene/GameplayingScene.h"


using namespace std;

namespace {
	int runH_[6];
	int frame_ = 0;
	int idx_ = 0;
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
				player_.Move({ 0, -5 });
			}
			if (input.IsPressed("down")) {
				player_.Move({ 0, 5 });
			}
			if (input.IsPressed("left")) {
				player_.Move({ -5, 0 });
			}
			if (input.IsPressed("right")) {
				player_.Move({ 5, 0 });
			}
		}
	};
	gs->AddListener(make_shared< PlayerInputListener>(*this));
}
Player::~Player() {
	for (auto& run : runH_) {
		DxLib::DeleteGraph(run);
	}
}
void 
Player::SetPosition(const Position2& p) {
	pos_ = p;
}

void
Player::Move(const Vector2& v) {
	pos_ += v;
}

void 
Player::Update() {
	++frame_;
	if (frame_ % 5 == 0) {
		idx_ = (idx_ + 1) % _countof(runH_);
	} 
}
void 
Player::Draw() {
	DrawRotaGraph(pos_.x, pos_.y, 3.0f, 0.0f, runH_[idx_], true, false, false);
}
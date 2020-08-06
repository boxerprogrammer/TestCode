#include "Effect.h"
#include<DxLib.h>
#include<algorithm>
#include<sstream>
#include<random>
#include"Camera.h"

using namespace std;

namespace {
	int bloodH_ = -1;
	int bloodSE = -1;
	int dronH_ = -1;
	class Blood : public Effect{
	private:
		bool isTurn_ = false;
	public:
		Blood(const Position2f& p,std::shared_ptr<Camera> c,bool isTurn) :Effect(p,c),isTurn_(isTurn) {
			PlaySoundMem(bloodSE, DX_PLAYTYPE_BACK);
			isDeletable_ = false;
			frame_ = 0;
		}
		void Update()override {
			if (frame_ >= 30) {
				isDeletable_ = true;
			}
		}
		void Draw()override {
			int idx = frame_ / 3;
			const auto xoffset=camera_->ViewOffset().x;
			DrawRectRotaGraph2(
				static_cast<int>(pos_.x+xoffset), static_cast<int>(pos_.y-24*2),
				0,idx*24,
				64,24,
				isTurn_?0:64,24,
				3.0f,0.0f,
				bloodH_, true,isTurn_);
			++frame_;
		}
	};
	class Blow : public Effect {
	private:
		int delay_ = 0;
		bool isTurn_ = false;
		

		using Updater_t = void(Blow::*)();
		Updater_t updater_;
		void NormalUpdate() {
			if (frame_ >= 16) {
				isDeletable_ = true;
			}
			++frame_;
		}
		void WaitUpdate() {
			if (--delay_ == 0) {
				updater_=&Blow::NormalUpdate;
			}
		}
	public:
		Blow(const Position2f& p, shared_ptr<Camera> c ,bool isTurn,uint32_t delay=0) :Effect(p,c), isTurn_(isTurn),delay_(delay) {
			PlaySoundMem(bloodSE, DX_PLAYTYPE_BACK);
			isDeletable_ = false;
			frame_ = 0;
			if (delay == 0) {
				updater_ = &Blow::NormalUpdate;
			}
			else {
				updater_ = &Blow::WaitUpdate;
			}
		}
		void Update()override {
			(this->*updater_)();
		}
		void Draw()override {
			const auto xoffset = camera_->ViewOffset().x;
			int idx = frame_ / 2;
			DrawRectRotaGraph2(
				static_cast<int>(pos_.x+xoffset), static_cast<int>(pos_.y),
				32*idx, 0,
				32, 32,
				isTurn_ ? 0 : 32, 32,
				2.0f, 0.0f,
				dronH_, true, isTurn_);
			
		}
	};
}
mt19937 mt_;
EffectManager::EffectManager(shared_ptr<Camera> c):camera_(c) {
	if (bloodH_ == -1) {
		bloodH_ = LoadGraph(L"Resource/Image/Effect/blood.png");
	}
	if (bloodSE == -1) {
		bloodSE = LoadSoundMem(L"Resource/Sound/Game/bloodsplash.mp3");
	}
	if (dronH_ == -1) {
		dronH_ = LoadGraph(L"Resource/Image/Effect/dron.png");
	}
}
void 
EffectManager::EmitBlood(const Position2f& p, bool isTurn) {
	effects_.emplace_back(new Blood(p,camera_,isTurn));
}
void 
EffectManager::EmitBlow3(const Position2f& p, bool isTurn) {
	uniform_int_distribution<int> dst(-10, 10);

	effects_.emplace_back(new Blow(p+Vector2f(dst(mt_),dst(mt_)*2.0f), camera_, isTurn));
	effects_.emplace_back(new Blow(p+Vector2f(dst(mt_), dst(mt_)*2.0f), camera_, isTurn,5));
	effects_.emplace_back(new Blow(p+Vector2f(dst(mt_), dst(mt_)*2.0f), camera_, isTurn, 10));
}
void 
EffectManager::Update() {
	for (auto& e : effects_) {
		e->Update();
	}
	auto it = remove_if(effects_.begin(), effects_.end(),
		[](shared_ptr<Effect>& e) {
			return e->IsDeletable();
		});
	effects_.erase(it, effects_.end());
}
void 
EffectManager::Draw() {
	for (auto& e : effects_) {
		e->Draw();
	}
}


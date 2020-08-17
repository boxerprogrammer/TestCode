#include "Effect.h"
#include<DxLib.h>
#include<algorithm>
#include<sstream>
#include<random>
#include"Camera.h"
#include"../System/File.h"
#include"../System/FileManager.h"

using namespace std;

namespace {

	/// <summary>
	/// ååÇµÇ‘Ç´
	/// </summary>
	class Blood : public Effect{
	private:
		bool isTurn_ = false;
		int h_;
	public:
		Blood(const Position2f& p,int h,std::shared_ptr<Camera> c,bool isTurn) :
			Effect(p,c),
			isTurn_(isTurn),
			h_(h){
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
				h_, true,isTurn_);
			++frame_;
		}
	};

	/// <summary>
	/// è¨îöî≠
	/// </summary>
	class Blow : public Effect {
	private:
		int h_ = -1;
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
		Blow(const Position2f& p, int h,shared_ptr<Camera> c ,bool isTurn,uint32_t delay=0) :
			Effect(p,c), 
			isTurn_(isTurn),
			delay_(delay),
			h_(h){
			
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
				h_, true, isTurn_);
			
		}
	};
	/// <summary>
	/// ínñ îöî≠
	/// </summary>
	class GroundExplosion : public Effect {
	private:
		int h_ = -1;
		bool isTurn_ = false;


		using Updater_t = void(GroundExplosion::*)();
		Updater_t updater_;
		void NormalUpdate() {
			if (frame_ >= 20) {
				isDeletable_ = true;
			}
			++frame_;
		}
	public:
		GroundExplosion(const Position2f& p, int h, shared_ptr<Camera> c, bool isTurn) :
			Effect(p, c),
			isTurn_(isTurn),
			h_(h) {

			isDeletable_ = false;
			frame_ = 0;
			updater_ = &GroundExplosion::NormalUpdate;
		}
		void Update()override {
			(this->*updater_)();
		}
		void Draw()override {
			const auto xoffset = camera_->ViewOffset().x;
			int idx = frame_ / 2;
			DrawRectRotaGraph2(
				static_cast<int>(pos_.x + xoffset), static_cast<int>(pos_.y),
				128 * idx, 0,
				128, 80,
				64, 80,
				2.0f, 0.0f,
				h_, true, isTurn_);

		}
	};
}
mt19937 mt_;
EffectManager::EffectManager(shared_ptr<Camera> c):camera_(c) {
	auto& fileMgr = FileManager::Instance();
	if (bloodH_ == -1) {
		bloodH_ = fileMgr.Load(L"Resource/Image/Effect/blood.png")->Handle();
	}
	if (bloodSE_ == -1) {
		bloodSE_ = fileMgr.Load(L"Resource/Sound/Game/bloodsplash.mp3")->Handle();
	}
	if (hitSE_ == -1) {
		hitSE_ = fileMgr.Load(L"Resource/Sound/Game/hit.wav")->Handle();
	}
	if (blowH_ == -1) {
		blowH_ = fileMgr.Load(L"Resource/Image/Effect/dron.png")->Handle();
	}

	if (groundExpH_ == -1) {
		groundExpH_ = fileMgr.Load(L"Resource/Image/Effect/ground_exp.png")->Handle();
	}
	if (smokeExpH_ == -1) {
		smokeExpH_ = fileMgr.Load(L"Resource/Image/Effect/smoke_exp.png")->Handle();
	}

}

void 
EffectManager::EmitGroundExplosion(const Position2f& pos, bool isTurn) {
	effects_.emplace_back(new GroundExplosion(pos, groundExpH_, camera_, isTurn));
}

void
EffectManager::EmitSmokeExplosion(const Position2f& pos, bool isTurn) {

}

void 
EffectManager::EmitBlood(const Position2f& p, bool isTurn) {
	PlaySoundMem(bloodSE_, DX_PLAYTYPE_BACK);
	effects_.emplace_back(new Blood(p,bloodH_,camera_,isTurn));
}
void 
EffectManager::EmitBlow3(const Position2f& p, bool isTurn) {
	uniform_real_distribution<float> dst(-10, 10);
	PlaySoundMem(hitSE_, DX_PLAYTYPE_BACK);
	effects_.emplace_back(new Blow(p+Vector2f(dst(mt_),dst(mt_)*2.0f),blowH_, camera_, isTurn));
	effects_.emplace_back(new Blow(p+Vector2f(dst(mt_), dst(mt_)*2.0f), blowH_, camera_, isTurn,5));
	effects_.emplace_back(new Blow(p+Vector2f(dst(mt_), dst(mt_)*2.0f), blowH_, camera_, isTurn, 10));
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


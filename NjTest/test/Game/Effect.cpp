#include "Effect.h"
#include<DxLib.h>
#include<algorithm>

using namespace std;

namespace {
	int bloodH = -1;
	int bloodSE = -1;
	class Blood : public Effect{
	private:
		bool isTurn_ = false;
	public:
		Blood(const Position2f& p,bool isTurn) :Effect(p),isTurn_(isTurn) {
			PlaySoundMem(bloodSE, DX_PLAYTYPE_BACK);
			isDeletable_ = false;
			frame_ = 0;
		}
		void Update()override {
			if (frame_ >= 20) {
				isDeletable_ = true;
			}
		}
		void Draw()override {
			int xIdx = frame_ % 5;
			int yIdx = frame_ / 5;
			DrawRectRotaGraph2(
				static_cast<int>(pos_.x), static_cast<int>(pos_.y),
				xIdx*256,yIdx*256,
				256,256,
				isTurn_?0:256,256,
				1.0f,0.0f,
				bloodH, true,!isTurn_);
			++frame_;
		}
	};
}
EffectManager::EffectManager() {
	if (bloodH == -1) {
		bloodH = LoadGraph(L"Resource/Image/Player/blood.png");
	}
	if (bloodSE == -1) {
		bloodSE = LoadSoundMem(L"Resource/Sound/Game/bloodsplash.mp3");
	}
}
void 
EffectManager::EmitBlood(const Position2f& p, bool isTurn) {
	effects_.emplace_back(new Blood(p,isTurn));
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
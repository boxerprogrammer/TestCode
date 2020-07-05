#include "GameplayingScene.h"
#include"SceneController.h"
#include"ContinueScene.h"
#include"PauseScene.h"
#include<DxLib.h>
#include<algorithm>
#include<sstream>
#include<iomanip>
#include<cmath>
#include"../Input.h"
#include"../Application.h"
#include"../Geometry.h"
#include"../Game/Player.h"
#include"../Game/Background.h"

namespace {
	constexpr uint32_t fadeout_interval = 45;
	unsigned int waitTimer_ = 0;

}
using namespace std;
GameplayingScene::GameplayingScene(SceneController& c):
	Scene(c),
	updater_(&GameplayingScene::FadeinUpdate),
	drawer_(&GameplayingScene::FadeDraw){

	player_ = make_unique<Player>(this);
	player_->SetPosition(Position2(400, 480));

	waitTimer_ = 0;
	bg_ = make_unique<Background>();

}

GameplayingScene::~GameplayingScene() {


}

void 
GameplayingScene::Update(const Input& input) {
	
	(this->*updater_)(input);
}

void
GameplayingScene::Draw() {
	(this->*drawer_)();	
}


//待ち
void
GameplayingScene::NormalUpdate(const Input& input) {
	if (input.IsTriggered("OK")) {
		updater_ = &GameplayingScene::FadeoutUpdate;
		drawer_ = &GameplayingScene::FadeDraw;
		waitTimer_ = fadeout_interval;
	}
	if (input.IsTriggered("pause")) {
		controller_.PushScene(new PauseScene(controller_));
	}
	player_->Update();
	bg_->Update();
	for (auto& listener : listeners_) {
		listener->Notify(input);
	}
}

void
GameplayingScene::FadeinUpdate(const Input&) {
	if (++waitTimer_ == fadeout_interval) {
		updater_ = &GameplayingScene::NormalUpdate;
		drawer_ = &GameplayingScene::NormalDraw;
	}
}

//フェードアウト
void
GameplayingScene::FadeoutUpdate(const Input&) {
	
	if (--waitTimer_ == 0) {
		controller_.ChangeScene(new ContinueScene(controller_));
	}
}


void
GameplayingScene::NormalDraw() {
	bg_->Draw();
	player_->Draw();
}
void
GameplayingScene::FadeDraw() {
	NormalDraw();
	auto blendparam = 255 * (static_cast<float>(fadeout_interval - waitTimer_) / static_cast<float>(fadeout_interval));
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendparam);
	DxLib::DrawBox(0, 0, 800, 600, 0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void 
GameplayingScene::AddListener(std::shared_ptr<InputListener> listener) {
	listeners_.push_back(listener);
}

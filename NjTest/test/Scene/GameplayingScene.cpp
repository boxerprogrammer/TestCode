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
#include"../Game/ProjectileManager.h"
#include<cassert>

namespace {
	constexpr uint32_t fadeout_interval = 45;
	unsigned int waitTimer_ = 0;
	int weaponUIH_[3];
	int bgm = -1;
}
using namespace std;
GameplayingScene::GameplayingScene(SceneController& c):
	Scene(c),
	updater_(&GameplayingScene::FadeinUpdate),
	drawer_(&GameplayingScene::FadeDraw){

	waitTimer_ = 0;
	bg_ = make_unique<Background>();
	pm_ = make_unique<ProjectileManager>();

	player_ = make_unique<Player>(this);
	player_->SetPosition(Position2f(400, 480));

	weaponUIH_[0]= LoadGraph(L"Resource/Image/UI/bomb.png");
	weaponUIH_[1] = LoadGraph(L"Resource/Image/UI/shuriken.png");
	weaponUIH_[2] = LoadGraph(L"Resource/Image/UI/chain.png");
	bgm = LoadBGM(L"Resource/BGM/stage1_normal.mp3");
	ChangeVolumeSoundMem(150, bgm);
}

GameplayingScene::~GameplayingScene() {
	for (auto& h : weaponUIH_) {
		DeleteGraph(h);
		h = -1;
	}
	DeleteSoundMem(bgm);
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
	pm_->Update();
}

void
GameplayingScene::FadeinUpdate(const Input&) {
	if (++waitTimer_ == fadeout_interval) {
		updater_ = &GameplayingScene::NormalUpdate;
		drawer_ = &GameplayingScene::NormalDraw;
		PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
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
	pm_->Draw();
	//武器UI表示
	DrawBox(12, 12, 76, 76, 0x000000, false);
	DrawGraph(10, 10, weaponUIH_[player_->CurrentEquipmentNo()], true);
	DrawBox(10, 10, 74, 74, 0xffffff, false);
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


ProjectileManager& 
GameplayingScene::GetProjectileManager() {
	assert(pm_);
	return *pm_;
}
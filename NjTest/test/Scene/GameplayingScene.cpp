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
#include"../Game/Player/Player.h"
#include"../Game/Background.h"
#include"../Game/Player/ProjectileManager.h"
#include"../Game/Enemy/EnemyManager.h"
#include"../Game/Enemy/SideSpawner.h"
#include"../Game/Enemy/Slasher.h"
#include"../Game/Effect.h"
#include"../Game/CollisionManager.h"
#include"../Game/Stage.h"
#include"../Game/Camera.h"
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

	collisionManager_ = make_shared<CollisionManager>();

	camera_=make_shared<Camera>();

	waitTimer_ = 0;
	bg_ = make_unique<Background>(camera_);
	projectileManager_ = make_unique<ProjectileManager>();

	player_ = make_shared<Player>(this);
	player_->SetPosition(Position2f(400, 480));
	camera_->SetPlayer(player_);

	effectManager_ = make_shared<EffectManager>(camera_);

	enemyManager_ = make_shared<EnemyManager>();
	spawners_.emplace_back(new SideSpawner(
		Position2f(0,0),
		new Slasher(player_,effectManager_,camera_),
		enemyManager_,
		collisionManager_,
		camera_));

	

	weaponUIH_[0]= LoadGraph(L"Resource/Image/UI/bomb.png");
	weaponUIH_[1] = LoadGraph(L"Resource/Image/UI/shuriken.png");
	weaponUIH_[2] = LoadGraph(L"Resource/Image/UI/chain.png");
	bgm = LoadBGM(L"Resource/BGM/stage1_normal.mp3");
	ChangeVolumeSoundMem(150, bgm);
	stage_ = make_shared<Stage>(camera_);
	stage_->Load(L"resource/level/level1.fmf");
	camera_->SetStageSize(stage_->GetStageSize());
	
}

std::shared_ptr<Player>& 
GameplayingScene::GetPlayer() {
	return player_;
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
shared_ptr<CollisionManager> 
GameplayingScene::GetCollisionManager() {
	return collisionManager_;
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
	collisionManager_->Update();
	camera_->Update();
	player_->Update();
	bg_->Update();
	for (auto& listener : listeners_) {
		listener->Notify(input);
	}
	projectileManager_->Update();
	enemyManager_->Update();
	for (auto& spw : spawners_) {
		spw->Update();
	}
	effectManager_->Update();
}

void
GameplayingScene::FadeinUpdate(const Input&) {
	camera_->Update();
	player_->Update();
	bg_->Update();
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
	stage_->BackDraw();
	player_->Draw();
	projectileManager_->Draw();
	enemyManager_->Draw();
	effectManager_->Draw();
	stage_->FrontDraw();
	collisionManager_->DebugDraw();
	
	//武器UI表示
	DrawBox(12, 12, 76, 76, 0x000000, false);
	DrawGraph(10, 10, weaponUIH_[player_->CurrentEquipmentNo()], true);
	DrawBox(10, 10, 74, 74, 0xffffff, false);
}
void
GameplayingScene::FadeDraw() {
	NormalDraw();
	auto blendparam = static_cast<int>(255 * (static_cast<float>(fadeout_interval - waitTimer_) / static_cast<float>(fadeout_interval)));
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
	assert(projectileManager_);
	return *projectileManager_;
}

std::shared_ptr<Camera>& 
GameplayingScene::GetCamera() {
	return camera_;
}
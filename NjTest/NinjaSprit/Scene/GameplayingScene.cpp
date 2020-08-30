#include "GameplayingScene.h"
#include"SceneController.h"
#include"ContinueScene.h"
#include"PauseScene.h"
#include<DxLib.h>
#include<algorithm>
#include<sstream>
#include<iomanip>
#include<cmath>
#include"../System/Application.h"
#include"../Input/Input.h"
#include"../System/Application.h"
#include"../Geometry.h"
#include"../Game/Player/Player.h"
#include"../Game/Background.h"
#include"../Game/ProjectileManager.h"
#include"../Game/Enemy/EnemyManager.h"
#include"../Game/Enemy/SideSpawner.h"
#include"../Game/Enemy/Slasher.h"
#include"../Game/Effect.h"
#include"../Game/Collision/CollisionManager.h"
#include"../Game/Collision/CircleCollider.h"
#include"../Game/Stage.h"
#include"../Game/Camera.h"
#include"../System/FileManager.h"
#include"../System/File.h"
#include"../Game/Player/Equipment.h"
#include"../Arithmetic.h"
#include<cassert>

namespace {
	
	constexpr uint32_t fadeout_interval = 45;
	unsigned int waitTimer_ = 0;
	Position2 ashuraPos_(400, 1000);
	
}
using namespace std;
GameplayingScene::GameplayingScene(SceneController& c):
	Scene(c),
	updater_(&GameplayingScene::InitializeUpdate),
	drawer_(&GameplayingScene::FadeDraw),
	weaponUIH_()
{
}

std::shared_ptr<Stage>&
GameplayingScene::GetStage() {
	return stage_;
}

std::shared_ptr<Player>& 
GameplayingScene::GetPlayer() {
	return player_;
}

GameplayingScene::~GameplayingScene() {
	DxLib::DeleteSoundMem(bgm_);
	DxLib::DeleteSoundMem(bossBgm_);
	FileManager::Instance().DeleteAllResources();
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

std::shared_ptr<EnemyManager> 
GameplayingScene::GetEnemyManager() {
	return enemyManager_;
}

std::shared_ptr<EffectManager> 
GameplayingScene::GetEffectManager() {
	return effectManager_;
}

//初期化用アップデータ
void 
GameplayingScene::InitializeUpdate(const Input&) {
	auto& fileMgr = FileManager::Instance();
	collisionManager_ = make_shared<CollisionManager>();

	camera_ = make_shared<Camera>();

	waitTimer_ = 0;
	bg_ = make_unique<Background>(camera_);
	projectileManager_ = make_unique<ProjectileManager>();

	player_ = make_shared<Player>(this);
	player_->SetPosition(Position2f(400, 480));
	collisionManager_->AddCollider(new CircleCollider(player_, Circle(Position2f(0,-50), 30),tag_player_damage));
	camera_->SetPlayer(player_);

	effectManager_ = make_shared<EffectManager>(camera_);

	enemyManager_ = make_shared<EnemyManager>();
	stage_.reset(new Stage(camera_, this));
	stage_->Load(L"resource/level/level1.fmf");
	camera_->SetStageSize(stage_->GetStageSize());

	
	//AddSpawner(new SideSpawner(Position2f(0, 0),
	//		new Slasher(player_, effectManager_, camera_, stage_),
	//		enemyManager_,
	//		collisionManager_,
	//		camera_));

	weaponUIH_[sword_equip_no] = fileMgr.Load(L"Resource/Image/UI/sword.png")->Handle();
	weaponUIH_[bomb_equip_no] = fileMgr.Load(L"Resource/Image/UI/bomb.png")->Handle();
	weaponUIH_[shuriken_equip_no] = fileMgr.Load(L"Resource/Image/UI/shuriken.png")->Handle();
	weaponUIH_[chain_equip_no] = fileMgr.Load(L"Resource/Image/UI/chain.png")->Handle();
	bgm_ = LoadBGM(L"Resource/BGM/stage1_normal.mp3");
	ChangeVolumeSoundMem(bgmVolume_, bgm_);
	ashuraH_ = fileMgr.Load(L"Resource/Image/Enemy/ashura.png")->Handle();
	bossBgm_ = LoadBGM(L"Resource/BGM/boss.mp3");

	updater_ = &GameplayingScene::FadeinUpdate;
}

//敵発生器登録
void 
GameplayingScene::AddSpawner(Spawner* spawner) {
	spawners_.emplace_back(spawner);
}

//待ち
void
GameplayingScene::NormalUpdate(const Input& input) {
	if (!CheckStreamSoundMem(bgm_)) {
		PlayStreamSoundMem(bgm_, DX_PLAYTYPE_LOOP, false);
	}
	collisionManager_->Update();
	camera_->Update();
	player_->Update();
	bg_->Update();
	stage_->Update();
	for (auto& listener : listeners_) {
		listener->Notify(input);
	}
	projectileManager_->Update();
	enemyManager_->Update();
	for (auto& spw : spawners_) {
		spw->Update();
	}
	effectManager_->Update();
	if (stage_->IsBossMode()&&updater_==&GameplayingScene::NormalUpdate) {
		camera_->Lock();
		updater_=&GameplayingScene::BossEnterUpdate;
		drawer_ = &GameplayingScene::BossDraw;
	}
	if (input.IsTriggered("OK")) {
		Fadeout();
	}
	if (input.IsTriggered("pause")) {
		controller_.PushScene(new PauseScene(controller_));
		StopStreamSoundMem(bgm_);
	}
}

void GameplayingScene::Fadeout()
{
	updater_ = &GameplayingScene::FadeoutUpdate;
	drawer_ = &GameplayingScene::FadeDraw;
	waitTimer_ = fadeout_interval;
}

void
GameplayingScene::OnBossEnter() {

}
void
GameplayingScene::OnBossDie() {
	Fadeout();
}

void 
GameplayingScene::BossEnterUpdate(const Input&) {
	if (bgmVolume_ < 10) {
		StopSoundMem(bgm_);
		ChangeVolumeSoundMem(150, bossBgm_);
		PlaySoundMem(bossBgm_, DX_PLAYTYPE_LOOP);
		updater_ = &GameplayingScene::BossBattleUpdate;
	}
	else {
		bgmVolume_ -= 3;
		ChangeVolumeSoundMem(bgmVolume_, bgm_);
	}
}

void 
GameplayingScene::BossBattleUpdate(const Input& input) {
	
	NormalUpdate(input);
}
void 
GameplayingScene::BossDyingUpdate(const Input& input) {
	NormalUpdate(input);
}

void
GameplayingScene::FadeinUpdate(const Input&) {
	camera_->Update();
	player_->Update();
	bg_->Update();
	if (++waitTimer_ == fadeout_interval) {
		updater_ = &GameplayingScene::NormalUpdate;
		drawer_ = &GameplayingScene::NormalDraw;
		PlayStreamSoundMem(bgm_, DX_PLAYTYPE_LOOP,true);
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
GameplayingScene::BossDraw() {
	bg_->Draw();
	stage_->BackDraw();
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, 64);
	const auto& rc=Application::Instance().GetViewport().GetRect();
	DrawBox(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0x000000,true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//ashuraPos_.y = SaturateSubtract(ashuraPos_.y, 3, 600 - 16);
	//DrawRotaGraph2(ashuraPos_.x, ashuraPos_.y, 
	//	175,400,1.2f,0.0f,
	//	ashuraH_, true);
	enemyManager_->Draw();
	projectileManager_->Draw();
	player_->Draw();
	effectManager_->Draw();
	stage_->FrontDraw();
	collisionManager_->DebugDraw();
	stage_->DebugDraw();
	//武器UI表示
	DrawBox(12, 12, 76, 76, 0x000000, false);
	DrawGraph(10, 10, weaponUIH_[player_->CurrentEquipmentNo()], true);
	DrawBox(10, 10, 74, 74, 0xffffff, false);
}

void
GameplayingScene::NormalDraw() {
	bg_->Draw();
	stage_->BackDraw();
	enemyManager_->Draw();
	player_->Draw();
	projectileManager_->Draw();
	effectManager_->Draw();
	stage_->FrontDraw();
	collisionManager_->DebugDraw();
	stage_->DebugDraw();
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
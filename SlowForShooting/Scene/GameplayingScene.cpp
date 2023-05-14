#include"GameplayingScene.h"
#include"../InputState.h"
#include"GameoverScene.h"
#include"TitleScene.h"
#include"PauseScene.h"
#include"SceneManager.h"
#include"../DrawFunctions.h"
#include"../Game/Player.h"
#include"../Game/Force.h"
#include"../Game/Shot.h"
#include"../Game/Missile.h"
#include"../Game/ChargeShot.h"
#include"../Game/EnemyFactory.h"
#include"../Game/Enemy.h"
#include"../Game/BulletFactory.h"
#include"../Game/Stage.h"
#include"../Game/ItemFactory.h"

#include"../Game/OnetimeSpawner.h"
#include"../Game/TeamSpawner.h"
#include"../Game/InfiniteSpawner.h"
#include"../Game/PointInfiniteSpawner.h"

#include"../Game/Event.h"
#include"../SoundManager.h"
#include<DxLib.h>
#include<cassert>
#include<algorithm>

constexpr int rapid_fire_interval = 10;
constexpr int max_charge_frame = 80;//チャージ完了に必要な時間
constexpr int gauge_x = 10;
constexpr int gauge_y = 480-36;
constexpr float gauge_width_scale = 3.6f;

void GameplayingScene::FadeInUpdate(const InputState& input)
{
	SoundManager::GetInstance().SetBGMRate(static_cast<float>(60-fadeTimer_)/60.0f);
	fadeValue_ = static_cast<int>(255*static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameplayingScene::NormalUpdate;
		rapidFireCount_ = 0;
	}
	
}

void GameplayingScene::NormalUpdate(const InputState& input)
{	
	if (CheckPauseForSlow(input)) return;

	int mW, mH;
	stage_->GetMapSize(mW, mH);//パーツ単位
	if (eventWaitTimer_ == 0) {
		if (scroll_ + 1 < mW * 32 - 640) {
			scroll_ = scroll_ + 1;//ピクセル単位
		}
	}
	player_->Update(input);

	//もし連射(押してる間弾が出続ける)したかったら…
	bool forceShot = false;
	if (input.IsPressed(InputType::shot)) {
		//前のフレームが押されてなかった
		if (input.IsTriggered(InputType::shot)) {//押された瞬間は通常弾が出る
			for (auto& shot : shots_) {
				if (!shot->IsEnabled()) {
					if (forceShot) {
						//フォースから撃つ
						shot->Fire(player_->GetForce()->GetPosition() + Vector2(13.0f, 0.0f));
						break;
					}
					else {
						//自機から撃つ
						shot->Fire(player_->GetPosition() + Vector2(26.0f, 0.0f));
						if (player_->GetForce()->GetLevel() > 0 && player_->GetForce()->IsSepareted()) {
							forceShot = true;
							continue;
						}
						else {
							break;
						}
					}
					
				}
			}
		}
		++chargePower_;//押し続けていればチャージパワーがたまっていく
	}
	else {//ボタンを話せば、チャージ弾が出る(ただし、規定量たまっていれば)
		if (chargePower_ >= max_charge_frame) {
			for (auto& cshot : chargeShots_) {
				if (!cshot->IsEnabled()) {
					cshot->Fire(player_->GetPosition());
					break;
				}
			}
		}
		chargePower_ = 0;
	}

	if (player_->HasMissile()) {
		missileTimer_++;
		if (missileTimer_ == 120) {
			missileTimer_ = 0;
			bool twice = false;
			for (auto& missile : missiles_) {
				if (!missile->IsEnabled()) {
					if (!twice) {
						//上
						missile->Fire(player_->GetPosition() + Vector2(0.0f, -5.0f));
						missile->SetVelocity({ 0.0f,-3.0f });
						twice = true;
					}
					else {
						//下
						missile->Fire(player_->GetPosition() + Vector2(0.0f, 5.0f));
						missile->SetVelocity({ 0.0f,3.0f });
						break;
					}

				}
			}
		}
	}


	//全弾の状態を更新
	for (auto& shot : shots_) {
		if (shot->IsEnabled()) {
			shot->Update();
		}
	}
	for (auto& missile : missiles_) {
		if (missile->IsEnabled()) {
			missile->Update();
		}
	}

	//全チャージショットの状態を更新
	for (auto& cshot : chargeShots_) {
		if (cshot->IsEnabled()) {
			cshot->Update();
		}
	}

	if (eventWaitTimer_ == 0) {
		//イベント更新
		//マップに従ってスポーナーを生成
		auto chipX = static_cast<int>((640.0f + scroll_) / 32.0f);//一番右のXインデックスを見る
		if ((640 + scroll_) % 32 == 0) {
			for (int chipY = 0; chipY < mH; ++chipY) {//縦方向
				auto chipId = static_cast<EventChipId>(
						stage_->GetChipId(static_cast<int>(LayerType::Event),
					chipX, chipY));
				switch (chipId) {
				case EventChipId::wait_10:
					eventWaitTimer_ = 600;//60フレーム(10秒)待つ
					break;
				case EventChipId::wait_20:
					eventWaitTimer_ = 1200;//1200フレーム(20秒)待つ
					break;
				case EventChipId::change_music:
					SoundManager::GetInstance().PlayMusic(L"Data/Sound/BGM/middleboss1.mp3");
					break;
				}
			}
		}
	}
	eventWaitTimer_ = std::max(eventWaitTimer_ - 1, 0);

	//スポーナー関連の更新
	SpawnersUpdate(mW,mH);

	enemyFactory_->Update();
	bulletFactory_->Update();
	itemFactory_->Update();

	vy += 0.1f;
	py += vy;
	if (py >= 400) {
		vy = -8.0;
	}

	//自機の弾と、敵機の当たり判定①
	for (auto& shot : shots_) {
		if (!shot->IsEnabled())continue;
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//敵に弾がヒットしましたよ
			if (shot->GetRect().IsHit(enemy->GetRect())) {
				//敵も弾も消します
				shot->Kill();
				enemy->OnDamage(1);//1ダメージ与える
				break;
			}
		}
	}

	//ミサイルと敵の当たり判定
	for (auto& missile : missiles_) {
		if (!missile->IsEnabled())continue;
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//敵に弾がヒットしましたよ
			if (missile->GetRect().IsHit(enemy->GetRect())) {
				//敵も弾も消します
				missile->Kill();
				enemy->OnDamage(1);//1ダメージ与える
				break;
			}
		}
	}

	//自機の溜め弾と、敵機の当たり判定②
	for (auto& cshot : chargeShots_) {
		if (!cshot->IsEnabled())continue;
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//敵に弾がヒットしましたよ
			if (cshot->GetRect().IsHit(enemy->GetRect())) {
				//敵も弾も消します
				enemy->OnDamage(1);//1ダメージ与える
				break;
			}
		}
	}

	
	//自機とアイテムの当たり判定
	for (auto& item : itemFactory_->GetItems()) {
		if (!item->IsEnabled())continue;
		if (player_->GetRect().IsHit(item->GetRect())) {
			item->OnTouch();
			if (player_->GetForce()->GetLevel() == 0) {
				player_->GetForce()->SetPosition({320.0f,240.0f});
			}
			if (item->GetItemType() == ItemType::laser1) {
				player_->GetForce()->LevelUp();
			}
			else if (item->GetItemType() == ItemType::missile) {
				player_->SetHasMissile(true);
			}
			break;
		}
	}


	if (player_->IsCollidable()) {
		//自フォースと敵の当たり判定
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//敵に弾がヒットしましたよ
			if (player_->GetForce()->GetRect().IsHit(enemy->GetRect())) {
				//敵も弾も消します
				enemy->OnDamage(3);//1ダメージ与える
				break;
			}
		}

		//自機と敵の当たり判定③
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//敵に弾がヒットしましたよ
			if (player_->GetRect().IsHit(enemy->GetRect())) {
				//敵も弾も消します
				enemy->OnDamage(1);//1ダメージ与える
				player_->OnDamage();
				quakeX_ = 20.0f;
				quakeTimer_ = 60;
				break;
			}
		}
	}


	for (auto& bullet : bulletFactory_->GetBullets()) {
		if (player_->GetForce()->GetRect().IsHit(bullet->GetRect())) {
			bullet->OnDamaged(1);
		}
	}

	//自機と敵弾の当たり判定④
	if (player_->IsCollidable()) {
		for (auto& bullet : bulletFactory_->GetBullets()) {
			if (player_->GetRect().IsHit(bullet->GetRect())) {
				player_->OnDamage();
				bullet->OnDamaged(1);
				quakeX_ = 20.0f;
				quakeTimer_ = 60;
				break;
			}
		}
	}

	
	
	

	if (input.IsTriggered(InputType::next)) {
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
		fadeColor_ = 0xff0000;
	}
	if (input.IsTriggered(InputType::prev)) {
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	if (input.IsTriggered(InputType::pause)) {
		manager_.PushScene(new PauseScene(manager_));
	}
	if (quakeTimer_ > 0) {
		quakeX_ = -quakeX_;
		quakeX_ *= 0.95f;
		--quakeTimer_;
	}
	else {
		quakeX_ = 0.0f;
	}
}

bool 
GameplayingScene::CheckPauseForSlow(const InputState& input)
{
	frame_ = (frame_ + 1) % 65536;
	if (input.IsPressed(InputType::slow)) {
		if (frame_ % 30 == 0) {
			slowRate_++;
		}
	}
	else {
		slowRate_ = std::clamp(slowRate_ - 1, 0, slowRate_);
	}
	if (frame_ % (slowRate_ + 1) != 0) {
		return true;
	}
	return false;
}

void GameplayingScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast<int>(255*static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	if (++fadeTimer_ == fade_interval) {
		manager_.ChangeScene(new GameoverScene(manager_));
		return;
	}
	
}

void 
GameplayingScene::SetSpawnerByChipIdAndPosition(EnemySpawnerId id, const Position2& pos)
{
	//得られたチップIDから、敵のスポーナーを配置する。
	switch (static_cast<EnemySpawnerId>(id)) {
	case EnemySpawnerId::OnePatapata://パタパタ
		spawners_.push_back(
			std::make_shared<OnetimeSpawner>(
				pos,//座標
				EnemyType::Patapata,//プロトタイプ
				enemyFactory_//ファクトリ
				)
		);
		break;
	case EnemySpawnerId::TeamPatapata://チームパタパタ
		spawners_.push_back(
			std::make_shared<TeamSpawner>(
				pos,
				EnemyType::Patapata,
				enemyFactory_
				)
		);
		break;
	case EnemySpawnerId::InfinitePatapata://無限パタパタ
		spawners_.push_back(
			std::make_shared<InfiniteSpawner>(
				pos,
				EnemyType::Patapata,
				enemyFactory_
				)
		);
		break;
	case EnemySpawnerId::TeamInsect://チーム虫
		spawners_.push_back(
			std::make_shared<TeamSpawner>(
				pos,
				EnemyType::Insect,
				enemyFactory_
				)
		);
		break;
	case EnemySpawnerId::InfiniteRock://右から無限に岩が湧く
		spawners_.push_back(
			std::make_shared<InfiniteSpawner>(
				pos,
				EnemyType::Rock,
				enemyFactory_
				)
		);
		break;
	//特定場所から上に火山岩が吹き出す
	case EnemySpawnerId::PosInfVolcanicRock:
		spawners_.push_back(
			std::make_shared<PointInfiniteSpawner>(
				pos,
				EnemyType::VolcanicRock,
				enemyFactory_
				)
		);
		break;
	//アイテムを出現させる敵を一体出す
	case EnemySpawnerId::OneItemBringer:
		spawners_.push_back(
			std::make_shared<OnetimeSpawner>(
				pos,
				EnemyType::ItemBringer,
				enemyFactory_
				)
		);
		break;
		//アイテムを出現させる敵を一体出す
	case EnemySpawnerId::OneMissleBringer:
		spawners_.push_back(
			std::make_shared<OnetimeSpawner>(
				pos,
				EnemyType::MissileBringer,
				enemyFactory_
				)
		);
		break;
	}
}

void GameplayingScene::SpawnersUpdate(int mapW,int mapH)
{
	//マップに従ってスポーナーを生成
	auto chipX = static_cast<int>((640.0f + scroll_) / 32.0f);//一番右のXインデックスを見る
	if ((640 + scroll_) % 32 == 0) {
		for (int chipY = 0; chipY < mapH; ++chipY) {//縦方向
			auto chipId = static_cast<EnemySpawnerId>(stage_->GetChipId(1, chipX, chipY));
			SetSpawnerByChipIdAndPosition(chipId, Position2(640.0f+scroll_, chipY * 32.0f));
		}
	}
	//更新
	for (auto& spawner : spawners_) {
		spawner->Update(scroll_);
	}
	//いらなくなったスポーナーを削除
	spawners_.remove_if(
		[](const std::shared_ptr<Spawner> spawner) {
			return !spawner->IsEnabled();
		});
}

GameplayingScene::GameplayingScene(SceneManager& manager) :Scene(manager),
updateFunc_(&GameplayingScene::FadeInUpdate) {
	px = 320;
	py = 400;
	vx = 0;
	vy = 0;

	int sw, sh,bit;//画面幅&画面高&ビット数
	GetScreenState(&sw,&sh,&bit);//幅と高さを取得しておく
	tempScreenH_ = MakeScreen(sw,sh);//加工用画面を用意する
	assert(tempScreenH_ >= 0);//作れなかったらここで停止する

	//ハンドルを設定
	bgImgs_[0].handle= my::MyLoadGraph(L"Data/img/Game/bg-back.png");
	bgImgs_[1].handle = my::MyLoadGraph(L"Data/img/Game/bg-stars.png");
	bgImgs_[2].handle = my::MyLoadGraph(L"Data/img/Game/bg-planet2.png");
	//スクロールスピードを設定
	bgImgs_[0].scrollSpeed = 0.5f;//一番遠い(星雲)
	bgImgs_[1].scrollSpeed = 0.75f;//中間(星空)
	bgImgs_[2].scrollSpeed = 1.0f;//一番手前(惑星)
	//画像のサイズを代入しておく
	for (auto& img : bgImgs_) {
		//背景の幅と高さを記録しておく
		GetGraphSize(
			img.handle, //画像のハンドル
			&img.imgSize.w, //画像の幅
			&img.imgSize.h);//画像の高さ
	}

	gaugeImg_ = my::MyLoadGraph(L"Data/img/Game/gauge.png");
	chargedGaugeImg_ = my::MyLoadGraph(L"Data/img/Game/charged_gauge.png");
	player_ = std::make_shared<Player>();
	
	//弾の生成
	int shotH = my::MyLoadGraph(L"Data/img/Game/shot.png");
	for (auto& shot : shots_) {
		shot = std::make_shared<Shot>(shotH);
	}

	//ミサイルの生成
	int missileH = my::MyLoadGraph(L"Data/img/Game/missile.png");
	

	int chargeShotH = my::MyLoadGraph(L"Data/img/game/charge_shot.png");
	for (auto& cshot : chargeShots_) {
		cshot = std::make_shared<ChargeShot>(chargeShotH);
	}
	//順番は弾工場→敵工場の順番で生成してください
	bulletFactory_ = std::make_shared<BulletFactory>();
	itemFactory_ = std::make_shared<ItemFactory>();
	enemyFactory_ = std::make_shared<EnemyFactory>(player_, bulletFactory_,itemFactory_);
	
	for (auto& missile : missiles_) {
		missile = std::make_shared<Missle>(missileH, enemyFactory_);
	}

	stage_ = std::make_shared<Stage>();
	stage_->Load(L"Data/stage1.fmf");
	chipImg_ = LoadGraph(L"Data/img/game/map.png");

	SoundManager::GetInstance().SetBGMRate(0.0f);
	SoundManager::GetInstance().PlayMusic(L"Data/Sound/BGM/stage1.mp3");
}

GameplayingScene::~GameplayingScene()
{
	DeleteGraph(tempScreenH_);
}

void 
GameplayingScene::Update(const InputState& input) {
	(this->*updateFunc_)(input);
}

void
GameplayingScene::Draw() {
	//加工用スクリーンハンドルをセット
	SetDrawScreen(tempScreenH_);

	constexpr float bg_scale = 3.0f;
	//基準のスクロール
	
	for (auto& bg : bgImgs_) {
		
		auto bgWidth = bg.imgSize.w*bg_scale;

		int scroll = static_cast<int>(scroll_* bg.scrollSpeed) % 
							static_cast<int>(bgWidth);
		//背景の描画①
		DrawRotaGraph(
			static_cast<int>(bgWidth * 0.5f)  - scroll, //表示中心x座標
			static_cast<int>(bg.imgSize.h * 0.5f * bg_scale),//表示中心y座標
			3.0f, 0.0f, bg.handle,true,
			false, false);

		//背景の描画②
		DrawRotaGraph(
			static_cast<int>(bgWidth * (1 + 0.5))  - scroll, //表示中心x座標
			static_cast<int>(bg.imgSize.h * 0.5f * bg_scale),//表示中心y座標
			3.0f, 0.0f, bg.handle, true,
			false,
			false);
		//if (scroll == 0 && bg.isTurnV) {
		//	bg.currentTurnV = !bg.currentTurnV;
		//}
	}

	int mW, mH;
	stage_->GetMapSize(mW, mH);
	const auto& mapData = stage_->GetMapData();
	for (int chipY = 0; chipY < mH; ++chipY) {//縦方向
		for (int chipX = 0; chipX < mW; ++chipX) {//横方向
			auto chipId = stage_->GetChipId(0, chipX, chipY);
			if (chipId != 0) {
				DrawRectGraph(chipX * 32- scroll_, chipY * 32,
					(chipId%16)*32,
					(chipId/16) * 32,
					32,32,
					chipImg_, true);
			}
		}
	}

	//DrawRotaGraph(320, 240, 1.0f,0.0f,playingH_,true);
	//プレイヤーの描画
	player_->Draw();
	//通常弾の描画
	for (auto& shot : shots_) {
		if (shot->IsEnabled()) {
			shot->Draw();
		}
	}
	for (auto& cshot : chargeShots_) {
		if (cshot->IsEnabled()) {
			cshot->Draw();
		}
	}
	for (auto& missile : missiles_) {
		if (missile->IsEnabled()) {
			missile->Draw();
		}
	}

	//アイテムの表示
	itemFactory_->Draw();

	//敵の描画
	enemyFactory_->Draw();

	//DrawCircle(px, py, 20, 0xaaffaa, true);
	//弾の描画
	bulletFactory_->Draw();


	//チャージゲージ
	DrawExtendGraph(gauge_x, gauge_y,//左上
		static_cast<int>(gauge_x + std::min(chargePower_ ,max_charge_frame)* gauge_width_scale),
		gauge_y + 10,//右下
		(chargePower_>=max_charge_frame) ? chargedGaugeImg_ : gaugeImg_,
		false);
	//ゲージ枠
	DrawBox(gauge_x, gauge_y,//左上
		static_cast<int>(gauge_x + max_charge_frame * gauge_width_scale), gauge_y + 10,//右下
		0xffffff, false);
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, 640, 480, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	SetDrawScreen(DX_SCREEN_BACK);
	int iQuakeX = static_cast<int>(quakeX_);
	DrawGraph(iQuakeX, 0, tempScreenH_, false);
	if (quakeTimer_ > 0) {
		//画面をぼかす
		GraphFilter(tempScreenH_, DX_GRAPH_FILTER_GAUSS,16,1400);
		//加算合成する
		SetDrawBlendMode(DX_BLENDMODE_ADD, 192);
		DrawGraph(iQuakeX, 0, tempScreenH_, false);
		GraphFilter(tempScreenH_, DX_GRAPH_FILTER_GAUSS, 32, 2000);
		DrawGraph(iQuakeX, 0, tempScreenH_, false);
		//元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	
}
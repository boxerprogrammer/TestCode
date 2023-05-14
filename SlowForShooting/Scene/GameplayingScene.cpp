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
constexpr int max_charge_frame = 80;//�`���[�W�����ɕK�v�Ȏ���
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
	stage_->GetMapSize(mW, mH);//�p�[�c�P��
	if (eventWaitTimer_ == 0) {
		if (scroll_ + 1 < mW * 32 - 640) {
			scroll_ = scroll_ + 1;//�s�N�Z���P��
		}
	}
	player_->Update(input);

	//�����A��(�����Ă�Ԓe���o������)������������c
	bool forceShot = false;
	if (input.IsPressed(InputType::shot)) {
		//�O�̃t���[����������ĂȂ�����
		if (input.IsTriggered(InputType::shot)) {//�����ꂽ�u�Ԃ͒ʏ�e���o��
			for (auto& shot : shots_) {
				if (!shot->IsEnabled()) {
					if (forceShot) {
						//�t�H�[�X���猂��
						shot->Fire(player_->GetForce()->GetPosition() + Vector2(13.0f, 0.0f));
						break;
					}
					else {
						//���@���猂��
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
		++chargePower_;//���������Ă���΃`���[�W�p���[�����܂��Ă���
	}
	else {//�{�^����b���΁A�`���[�W�e���o��(�������A�K��ʂ��܂��Ă����)
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
						//��
						missile->Fire(player_->GetPosition() + Vector2(0.0f, -5.0f));
						missile->SetVelocity({ 0.0f,-3.0f });
						twice = true;
					}
					else {
						//��
						missile->Fire(player_->GetPosition() + Vector2(0.0f, 5.0f));
						missile->SetVelocity({ 0.0f,3.0f });
						break;
					}

				}
			}
		}
	}


	//�S�e�̏�Ԃ��X�V
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

	//�S�`���[�W�V���b�g�̏�Ԃ��X�V
	for (auto& cshot : chargeShots_) {
		if (cshot->IsEnabled()) {
			cshot->Update();
		}
	}

	if (eventWaitTimer_ == 0) {
		//�C�x���g�X�V
		//�}�b�v�ɏ]���ăX�|�[�i�[�𐶐�
		auto chipX = static_cast<int>((640.0f + scroll_) / 32.0f);//��ԉE��X�C���f�b�N�X������
		if ((640 + scroll_) % 32 == 0) {
			for (int chipY = 0; chipY < mH; ++chipY) {//�c����
				auto chipId = static_cast<EventChipId>(
						stage_->GetChipId(static_cast<int>(LayerType::Event),
					chipX, chipY));
				switch (chipId) {
				case EventChipId::wait_10:
					eventWaitTimer_ = 600;//60�t���[��(10�b)�҂�
					break;
				case EventChipId::wait_20:
					eventWaitTimer_ = 1200;//1200�t���[��(20�b)�҂�
					break;
				case EventChipId::change_music:
					SoundManager::GetInstance().PlayMusic(L"Data/Sound/BGM/middleboss1.mp3");
					break;
				}
			}
		}
	}
	eventWaitTimer_ = std::max(eventWaitTimer_ - 1, 0);

	//�X�|�[�i�[�֘A�̍X�V
	SpawnersUpdate(mW,mH);

	enemyFactory_->Update();
	bulletFactory_->Update();
	itemFactory_->Update();

	vy += 0.1f;
	py += vy;
	if (py >= 400) {
		vy = -8.0;
	}

	//���@�̒e�ƁA�G�@�̓����蔻��@
	for (auto& shot : shots_) {
		if (!shot->IsEnabled())continue;
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//�G�ɒe���q�b�g���܂�����
			if (shot->GetRect().IsHit(enemy->GetRect())) {
				//�G���e�������܂�
				shot->Kill();
				enemy->OnDamage(1);//1�_���[�W�^����
				break;
			}
		}
	}

	//�~�T�C���ƓG�̓����蔻��
	for (auto& missile : missiles_) {
		if (!missile->IsEnabled())continue;
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//�G�ɒe���q�b�g���܂�����
			if (missile->GetRect().IsHit(enemy->GetRect())) {
				//�G���e�������܂�
				missile->Kill();
				enemy->OnDamage(1);//1�_���[�W�^����
				break;
			}
		}
	}

	//���@�̗��ߒe�ƁA�G�@�̓����蔻��A
	for (auto& cshot : chargeShots_) {
		if (!cshot->IsEnabled())continue;
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//�G�ɒe���q�b�g���܂�����
			if (cshot->GetRect().IsHit(enemy->GetRect())) {
				//�G���e�������܂�
				enemy->OnDamage(1);//1�_���[�W�^����
				break;
			}
		}
	}

	
	//���@�ƃA�C�e���̓����蔻��
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
		//���t�H�[�X�ƓG�̓����蔻��
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//�G�ɒe���q�b�g���܂�����
			if (player_->GetForce()->GetRect().IsHit(enemy->GetRect())) {
				//�G���e�������܂�
				enemy->OnDamage(3);//1�_���[�W�^����
				break;
			}
		}

		//���@�ƓG�̓����蔻��B
		for (auto& enemy : enemyFactory_->GetEnemies()) {
			if (!enemy->IsCollidable())continue;
			//�G�ɒe���q�b�g���܂�����
			if (player_->GetRect().IsHit(enemy->GetRect())) {
				//�G���e�������܂�
				enemy->OnDamage(1);//1�_���[�W�^����
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

	//���@�ƓG�e�̓����蔻��C
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
	//����ꂽ�`�b�vID����A�G�̃X�|�[�i�[��z�u����B
	switch (static_cast<EnemySpawnerId>(id)) {
	case EnemySpawnerId::OnePatapata://�p�^�p�^
		spawners_.push_back(
			std::make_shared<OnetimeSpawner>(
				pos,//���W
				EnemyType::Patapata,//�v���g�^�C�v
				enemyFactory_//�t�@�N�g��
				)
		);
		break;
	case EnemySpawnerId::TeamPatapata://�`�[���p�^�p�^
		spawners_.push_back(
			std::make_shared<TeamSpawner>(
				pos,
				EnemyType::Patapata,
				enemyFactory_
				)
		);
		break;
	case EnemySpawnerId::InfinitePatapata://�����p�^�p�^
		spawners_.push_back(
			std::make_shared<InfiniteSpawner>(
				pos,
				EnemyType::Patapata,
				enemyFactory_
				)
		);
		break;
	case EnemySpawnerId::TeamInsect://�`�[����
		spawners_.push_back(
			std::make_shared<TeamSpawner>(
				pos,
				EnemyType::Insect,
				enemyFactory_
				)
		);
		break;
	case EnemySpawnerId::InfiniteRock://�E���疳���Ɋ₪�N��
		spawners_.push_back(
			std::make_shared<InfiniteSpawner>(
				pos,
				EnemyType::Rock,
				enemyFactory_
				)
		);
		break;
	//����ꏊ�����ɉΎR�₪�����o��
	case EnemySpawnerId::PosInfVolcanicRock:
		spawners_.push_back(
			std::make_shared<PointInfiniteSpawner>(
				pos,
				EnemyType::VolcanicRock,
				enemyFactory_
				)
		);
		break;
	//�A�C�e�����o��������G����̏o��
	case EnemySpawnerId::OneItemBringer:
		spawners_.push_back(
			std::make_shared<OnetimeSpawner>(
				pos,
				EnemyType::ItemBringer,
				enemyFactory_
				)
		);
		break;
		//�A�C�e�����o��������G����̏o��
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
	//�}�b�v�ɏ]���ăX�|�[�i�[�𐶐�
	auto chipX = static_cast<int>((640.0f + scroll_) / 32.0f);//��ԉE��X�C���f�b�N�X������
	if ((640 + scroll_) % 32 == 0) {
		for (int chipY = 0; chipY < mapH; ++chipY) {//�c����
			auto chipId = static_cast<EnemySpawnerId>(stage_->GetChipId(1, chipX, chipY));
			SetSpawnerByChipIdAndPosition(chipId, Position2(640.0f+scroll_, chipY * 32.0f));
		}
	}
	//�X�V
	for (auto& spawner : spawners_) {
		spawner->Update(scroll_);
	}
	//����Ȃ��Ȃ����X�|�[�i�[���폜
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

	int sw, sh,bit;//��ʕ�&��ʍ�&�r�b�g��
	GetScreenState(&sw,&sh,&bit);//���ƍ������擾���Ă���
	tempScreenH_ = MakeScreen(sw,sh);//���H�p��ʂ�p�ӂ���
	assert(tempScreenH_ >= 0);//���Ȃ������炱���Œ�~����

	//�n���h����ݒ�
	bgImgs_[0].handle= my::MyLoadGraph(L"Data/img/Game/bg-back.png");
	bgImgs_[1].handle = my::MyLoadGraph(L"Data/img/Game/bg-stars.png");
	bgImgs_[2].handle = my::MyLoadGraph(L"Data/img/Game/bg-planet2.png");
	//�X�N���[���X�s�[�h��ݒ�
	bgImgs_[0].scrollSpeed = 0.5f;//��ԉ���(���_)
	bgImgs_[1].scrollSpeed = 0.75f;//����(����)
	bgImgs_[2].scrollSpeed = 1.0f;//��Ԏ�O(�f��)
	//�摜�̃T�C�Y�������Ă���
	for (auto& img : bgImgs_) {
		//�w�i�̕��ƍ������L�^���Ă���
		GetGraphSize(
			img.handle, //�摜�̃n���h��
			&img.imgSize.w, //�摜�̕�
			&img.imgSize.h);//�摜�̍���
	}

	gaugeImg_ = my::MyLoadGraph(L"Data/img/Game/gauge.png");
	chargedGaugeImg_ = my::MyLoadGraph(L"Data/img/Game/charged_gauge.png");
	player_ = std::make_shared<Player>();
	
	//�e�̐���
	int shotH = my::MyLoadGraph(L"Data/img/Game/shot.png");
	for (auto& shot : shots_) {
		shot = std::make_shared<Shot>(shotH);
	}

	//�~�T�C���̐���
	int missileH = my::MyLoadGraph(L"Data/img/Game/missile.png");
	

	int chargeShotH = my::MyLoadGraph(L"Data/img/game/charge_shot.png");
	for (auto& cshot : chargeShots_) {
		cshot = std::make_shared<ChargeShot>(chargeShotH);
	}
	//���Ԃ͒e�H�ꁨ�G�H��̏��ԂŐ������Ă�������
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
	//���H�p�X�N���[���n���h�����Z�b�g
	SetDrawScreen(tempScreenH_);

	constexpr float bg_scale = 3.0f;
	//��̃X�N���[��
	
	for (auto& bg : bgImgs_) {
		
		auto bgWidth = bg.imgSize.w*bg_scale;

		int scroll = static_cast<int>(scroll_* bg.scrollSpeed) % 
							static_cast<int>(bgWidth);
		//�w�i�̕`��@
		DrawRotaGraph(
			static_cast<int>(bgWidth * 0.5f)  - scroll, //�\�����Sx���W
			static_cast<int>(bg.imgSize.h * 0.5f * bg_scale),//�\�����Sy���W
			3.0f, 0.0f, bg.handle,true,
			false, false);

		//�w�i�̕`��A
		DrawRotaGraph(
			static_cast<int>(bgWidth * (1 + 0.5))  - scroll, //�\�����Sx���W
			static_cast<int>(bg.imgSize.h * 0.5f * bg_scale),//�\�����Sy���W
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
	for (int chipY = 0; chipY < mH; ++chipY) {//�c����
		for (int chipX = 0; chipX < mW; ++chipX) {//������
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
	//�v���C���[�̕`��
	player_->Draw();
	//�ʏ�e�̕`��
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

	//�A�C�e���̕\��
	itemFactory_->Draw();

	//�G�̕`��
	enemyFactory_->Draw();

	//DrawCircle(px, py, 20, 0xaaffaa, true);
	//�e�̕`��
	bulletFactory_->Draw();


	//�`���[�W�Q�[�W
	DrawExtendGraph(gauge_x, gauge_y,//����
		static_cast<int>(gauge_x + std::min(chargePower_ ,max_charge_frame)* gauge_width_scale),
		gauge_y + 10,//�E��
		(chargePower_>=max_charge_frame) ? chargedGaugeImg_ : gaugeImg_,
		false);
	//�Q�[�W�g
	DrawBox(gauge_x, gauge_y,//����
		static_cast<int>(gauge_x + max_charge_frame * gauge_width_scale), gauge_y + 10,//�E��
		0xffffff, false);
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, 640, 480, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	SetDrawScreen(DX_SCREEN_BACK);
	int iQuakeX = static_cast<int>(quakeX_);
	DrawGraph(iQuakeX, 0, tempScreenH_, false);
	if (quakeTimer_ > 0) {
		//��ʂ��ڂ���
		GraphFilter(tempScreenH_, DX_GRAPH_FILTER_GAUSS,16,1400);
		//���Z��������
		SetDrawBlendMode(DX_BLENDMODE_ADD, 192);
		DrawGraph(iQuakeX, 0, tempScreenH_, false);
		GraphFilter(tempScreenH_, DX_GRAPH_FILTER_GAUSS, 32, 2000);
		DrawGraph(iQuakeX, 0, tempScreenH_, false);
		//���ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	
}
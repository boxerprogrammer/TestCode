#include "EnemyFactory.h"
#include"Enemy.h"
#include"Patapata.h"
#include"Insect.h"
#include"Rock.h"
#include"VolcanicRock.h"
#include"Bringer.h"
#include"ItemFactory.h"
#include<DxLib.h>
#include<algorithm>
#include"../DrawFunctions.h"

EnemyFactory::EnemyFactory(std::shared_ptr<Player> player,
						std::shared_ptr<BulletFactory> bFactory,
						std::shared_ptr<ItemFactory> itemFactory):
player_(player),
bulletFactory_(bFactory),
itemFactory_(itemFactory)
{
	handleMap_[EnemyType::Patapata] = 
		my::MyLoadGraph(L"Data/img/Game/enemy1.png");
	handleMap_[EnemyType::Insect] =
		my::MyLoadGraph(L"Data/img/Game/insect.png");
	handleMap_[EnemyType::Rock] =
		my::MyLoadGraph(L"Data/img/Game/rock.png");
	handleMap_[EnemyType::ItemBringer] =
		my::MyLoadGraph(L"Data/img/Game/bringer.png");
	burstMap_[BurstType::ZakoBurst] = 
		my::MyLoadGraph(L"Data/img/Game/enemy_burst.png");
	
}

std::list<std::shared_ptr<Enemy>>& EnemyFactory::GetEnemies()
{
	return enemies_;
}



std::shared_ptr<Enemy> EnemyFactory::Create(EnemyType type, const Position2 pos)
{
	switch (type) {
	case EnemyType::Patapata:
		enemies_.push_back(
			std::make_shared<Patapata>(
				player_,
				pos, 
				handleMap_[EnemyType::Patapata],
				burstMap_[BurstType::ZakoBurst],
				bulletFactory_));
		break;
	

	}
	return enemies_.back();//back�͍Ō���̗v�f��Ԃ�
}

std::shared_ptr<Enemy> 
EnemyFactory::Make(EnemyType type)
{
	switch (type) {
	case EnemyType::Patapata://�p�^�p�^
		return std::make_shared<Patapata>(
			player_,
			Position2(),
			handleMap_[EnemyType::Patapata],
			burstMap_[BurstType::ZakoBurst],
			bulletFactory_);

	case EnemyType::Insect://��
		return std::make_shared<Insect>(
			player_,
			Position2(),
			handleMap_[EnemyType::Insect],
			burstMap_[BurstType::ZakoBurst],
			bulletFactory_);
	case EnemyType::Rock://��
		return std::make_shared<Rock>(
			player_,
			Position2(),
			handleMap_[EnemyType::Rock],
			burstMap_[BurstType::ZakoBurst],
			bulletFactory_);
	case EnemyType::VolcanicRock:
		return std::make_shared<VolcanicRock>(
			player_,
			Position2(),
			handleMap_[EnemyType::Rock],
			burstMap_[BurstType::ZakoBurst],
			bulletFactory_);

	case EnemyType::ItemBringer://�A�C�e���������Ă�����
		return std::make_shared<Bringer>(
			player_,
			Position2(),
			handleMap_[EnemyType::ItemBringer],
			burstMap_[BurstType::ZakoBurst],
			ItemType::laser1,
			bulletFactory_,
			itemFactory_);
	case EnemyType::MissileBringer://�A�C�e���������Ă�����
		return std::make_shared<Bringer>(
			player_,
			Position2(),
			handleMap_[EnemyType::ItemBringer],
			burstMap_[BurstType::ZakoBurst],
			ItemType::missile,
			bulletFactory_,
			itemFactory_);
	}
	return nullptr;
}

void 
EnemyFactory::AddEnemy(std::shared_ptr<Enemy> enemy)
{
	enemies_.push_back(enemy);
}

void EnemyFactory::Update()
{
	frame_ = frame_ + 1;
	//�p�^�p�^�̐���
	if (frame_ % 60 == 0) {
		//Create(EnemyType::Patapata, { 640.0f,static_cast<float>(GetRand(480)) });
		//Create(EnemyType::Patapata, { 640.0f,static_cast<float>(GetRand(480)) });
		//Create(EnemyType::Patapata, { 640.0f,static_cast<float>(GetRand(480)) });
	}

	//���Ȃ��Ȃ����G�͏����Ă��炤
	//�������߂̂����ɁA���ۂɂ͏����ĂȂ��āA���ɂ悯�Ă邾��
	auto rmIt = std::remove_if(//�����ɍ��v�������̂�����
		enemies_.begin(), //�Ώۂ�enemies_�̍ŏ�����
		enemies_.end(),//�Ō�܂�

		//�����Ă��炤������\�������_��
		//true���Ə�����Bfalse���Ə����Ȃ��B
		[](const std::shared_ptr<Enemy>& enemy) {
			return !enemy->IsEnabled();
		});

	//remove�n�̊֐��͂������㩂������āA������Ăяo�������ł�
	// ���ۂɂ͏����Ă��܂���B
	// �s�v�Ȃ��̂����ɔ����Ă邾���B
	// �Ȃ̂ŁA���Ȃ炸erase�֐��Ŏ��ۂɏ����K�v������B
	//remove�n�֐��͕s�v�Ȃ��̂����ɔ����āA�������ꏊ��
	//�C�e���[�^�Ƃ��ĕԂ��B
		
	//���ۂɔ͈͂��w�肵�ď���
	enemies_.erase(rmIt, enemies_.end());
	//�����܂ł��Ȃ��Ǝ��ۂɂ͏����Ȃ��̂Œ���

	//�G�S���X�V
	for (auto& enemy : enemies_) {
		if (enemy->IsEnabled()) {
			enemy->Update();
		}
	}
}

void EnemyFactory::Draw()
{
	for (auto& enemy : enemies_) {
		if (enemy->IsEnabled()) {
			enemy->Draw();
		}
	}
}

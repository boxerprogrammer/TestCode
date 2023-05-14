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
	return enemies_.back();//backは最後尾の要素を返す
}

std::shared_ptr<Enemy> 
EnemyFactory::Make(EnemyType type)
{
	switch (type) {
	case EnemyType::Patapata://パタパタ
		return std::make_shared<Patapata>(
			player_,
			Position2(),
			handleMap_[EnemyType::Patapata],
			burstMap_[BurstType::ZakoBurst],
			bulletFactory_);

	case EnemyType::Insect://虫
		return std::make_shared<Insect>(
			player_,
			Position2(),
			handleMap_[EnemyType::Insect],
			burstMap_[BurstType::ZakoBurst],
			bulletFactory_);
	case EnemyType::Rock://岩
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

	case EnemyType::ItemBringer://アイテムを持ってくるやつ
		return std::make_shared<Bringer>(
			player_,
			Position2(),
			handleMap_[EnemyType::ItemBringer],
			burstMap_[BurstType::ZakoBurst],
			ItemType::laser1,
			bulletFactory_,
			itemFactory_);
	case EnemyType::MissileBringer://アイテムを持ってくるやつ
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
	//パタパタの生成
	if (frame_ % 60 == 0) {
		//Create(EnemyType::Patapata, { 640.0f,static_cast<float>(GetRand(480)) });
		//Create(EnemyType::Patapata, { 640.0f,static_cast<float>(GetRand(480)) });
		//Create(EnemyType::Patapata, { 640.0f,static_cast<float>(GetRand(480)) });
	}

	//いなくなった敵は消えてもらう
	//消す命令のくせに、実際には消してなくて、後ろによけてるだけ
	auto rmIt = std::remove_if(//条件に合致したものを消す
		enemies_.begin(), //対象はenemies_の最初から
		enemies_.end(),//最後まで

		//消えてもらう条件を表すラムダ式
		//trueだと消える。falseだと消えない。
		[](const std::shared_ptr<Enemy>& enemy) {
			return !enemy->IsEnabled();
		});

	//remove系の関数はちょっと罠があって、これを呼び出すだけでは
	// 実際には消えていません。
	// 不要なものを後ろに避けてるだけ。
	// なので、かならずerase関数で実際に消す必要がある。
	//remove系関数は不要なものを後ろに避けて、避けた場所を
	//イテレータとして返す。
		
	//実際に範囲を指定して消す
	enemies_.erase(rmIt, enemies_.end());
	//ここまでやらないと実際には消えないので注意

	//敵全部更新
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

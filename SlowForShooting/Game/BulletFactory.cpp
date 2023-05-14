#include "BulletFactory.h"
#include"../DrawFunctions.h"
#include"ZakoBullet.h"
#include<algorithm>

BulletFactory::BulletFactory()
{
	bulletImgTable_[BulletType::Zako] = my::MyLoadGraph(L"Data/img/game/bullet1.png");
}

void BulletFactory::Create(BulletType bulletType, const Position2& pos, const Vector2& vel)
{
	switch (bulletType) {
	case BulletType::Zako:
		bullets_.push_back(std::make_shared<ZakoBullet>(bulletImgTable_[BulletType::Zako]));
		bullets_.back()->Fire(pos, vel);
		break;
	}
	
}

const BulletList_t& BulletFactory::GetBullets() const
{
	return bullets_;
}

void BulletFactory::Update()
{
	//ïsóvÇ…Ç»Ç¡ÇΩíeÇÃçÌèú
	auto rmIt = std::remove_if(bullets_.begin(),
		bullets_.end(),
		[](const std::shared_ptr<Bullet>& bullet) {
			return !bullet->IsEnabled();
		});
	bullets_.erase(rmIt,bullets_.end());


	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void BulletFactory::Draw()
{
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}

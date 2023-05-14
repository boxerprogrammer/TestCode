#include "OnetimeSpawner.h"
#include"EnemyFactory.h"
#include"Enemy.h"

OnetimeSpawner::OnetimeSpawner(const Position2& pos, 
	EnemyType type,
	std::shared_ptr<EnemyFactory> factory):
	Spawner(pos,factory->Make(type), factory)
{
}

std::shared_ptr<Enemy> OnetimeSpawner::Spawn()
{
	isEnabled_ = false;//�G���Y�񂾂玀��
	protoType_->SetPos(pos_);//�G���g�̍��W��ݒ�
	factory_->AddEnemy(protoType_);//
	return protoType_;
}

void OnetimeSpawner::Update(int scroll)
{
	Spawn();
}

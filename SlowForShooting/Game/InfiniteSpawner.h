#pragma once
#include "Spawner.h"
class InfiniteSpawner :
    public Spawner
{
private:
	int timer_=0;//�G�����𒲐�����^�C�}�[
	Position2 initPos_;//�ŏ��ɐݒ肳�ꂽ���W
public:
	InfiniteSpawner(const Position2& pos,
		EnemyType type,
		std::shared_ptr<EnemyFactory> factory);
	virtual std::shared_ptr<Enemy> Spawn()override;
	virtual void Update(int scroll)override;
};


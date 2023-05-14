#pragma once
#include "Spawner.h"
class InfiniteSpawner :
    public Spawner
{
private:
	int timer_=0;//敵発生を調整するタイマー
	Position2 initPos_;//最初に設定された座標
public:
	InfiniteSpawner(const Position2& pos,
		EnemyType type,
		std::shared_ptr<EnemyFactory> factory);
	virtual std::shared_ptr<Enemy> Spawn()override;
	virtual void Update(int scroll)override;
};


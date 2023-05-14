#pragma once
#include "Spawner.h"
/// <summary>
/// 5体連続で発生させて、消える
/// </summary>
class TeamSpawner :
    public Spawner
{
private:
	int timer_;//敵発生を調整するタイマー
	int count_;//何体発生させたかカウント
public:
	TeamSpawner(const Position2& pos,
		EnemyType type,
		std::shared_ptr<EnemyFactory> factory);
	virtual std::shared_ptr<Enemy> Spawn()override;
	virtual void Update(int scroll)override;
};


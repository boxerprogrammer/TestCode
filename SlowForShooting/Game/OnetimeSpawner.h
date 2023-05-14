#pragma once
#include "Spawner.h"

/// <summary>
/// �G����̔�������������ł���G������
/// </summary>
class OnetimeSpawner : public Spawner
{
public:
	OnetimeSpawner(const Position2& pos,
		EnemyType type, 
		std::shared_ptr<EnemyFactory> factory);
	virtual std::shared_ptr<Enemy> Spawn()override;
	virtual void Update(int scroll)override;
};


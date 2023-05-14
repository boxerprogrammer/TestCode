#pragma once
#include "Spawner.h"

/// <summary>
/// “G‚ğˆê‘Ì”­¶‚³‚¹‚½‚çÁ–Å‚·‚é“G”­¶Ší
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


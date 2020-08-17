#pragma once
#include "Spawner.h"
class CollisionManager;
class GameplayingScene;
class EnemyManager;
class BossSpawner :
    public Spawner
{
private:
	std::shared_ptr<CollisionManager> collisionManager_;//

	void CreateUpdate();
	void SleepUpdate();

	using Updater_t = void (BossSpawner::*)();
	Updater_t updater_;
public:
	BossSpawner(const Position2f& pos, 
		Enemy* prototype,
		std::shared_ptr<EnemyManager> enemyManager,
		std::shared_ptr<CollisionManager> collisionManager,
		std::shared_ptr<Camera> camera);
	~BossSpawner() = default;

	/// <summary>
	/// 時間ごとの発生などが必要なため毎フレーム更新を行う
	/// </summary>
	void Update() override;
};


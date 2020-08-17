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
	/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
	/// </summary>
	void Update() override;
};


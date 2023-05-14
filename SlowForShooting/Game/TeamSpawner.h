#pragma once
#include "Spawner.h"
/// <summary>
/// 5�̘A���Ŕ��������āA������
/// </summary>
class TeamSpawner :
    public Spawner
{
private:
	int timer_;//�G�����𒲐�����^�C�}�[
	int count_;//���̔������������J�E���g
public:
	TeamSpawner(const Position2& pos,
		EnemyType type,
		std::shared_ptr<EnemyFactory> factory);
	virtual std::shared_ptr<Enemy> Spawn()override;
	virtual void Update(int scroll)override;
};


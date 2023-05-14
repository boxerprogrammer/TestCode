#pragma once
#include "Spawner.h"
/// <summary>
/// ��_(����̏ꏊ)�ɂ����閳���N���X�|�[�i�[
/// </summary>
class PointInfiniteSpawner :
    public Spawner
{
private:
	int timer_ = 0;//�G�����𒲐�����^�C�}�[
	Position2 initPos_;//�ŏ��ɐݒ肳�ꂽ���W
public:
	PointInfiniteSpawner(const Position2& pos,
		EnemyType type,
		std::shared_ptr<EnemyFactory> factory);
	virtual std::shared_ptr<Enemy> Spawn()override;
	virtual void Update(int scroll)override;
};


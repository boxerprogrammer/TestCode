#pragma once
#include "Spawner.h"
class SideSpawner :  public Spawner
{
private:
	int frame_=0;
public:
	SideSpawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager>& em);

	/// <summary>
	/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
	/// </summary>
	void Update() override;
};


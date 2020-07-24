#pragma once
#include "Spawner.h"
class CollisionManager;
class SideSpawner :  public Spawner
{
private:
	int frame_=0;
	std::shared_ptr<CollisionManager> collisionManager_;
public:
	SideSpawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager>& em,std::shared_ptr<CollisionManager> cm);

	/// <summary>
	/// ���Ԃ��Ƃ̔����Ȃǂ��K�v�Ȃ��ߖ��t���[���X�V���s��
	/// </summary>
	void Update() override;
};


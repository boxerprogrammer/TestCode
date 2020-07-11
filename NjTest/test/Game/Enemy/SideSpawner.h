#pragma once
#include "Spawner.h"
class SideSpawner :  public Spawner
{
private:
	int frame_=0;
public:
	SideSpawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager>& em);

	/// <summary>
	/// 時間ごとの発生などが必要なため毎フレーム更新を行う
	/// </summary>
	void Update() override;
};


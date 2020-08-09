#pragma once
#include "Spawner.h"
class CollisionManager;
/// <summary>
/// 画面の両側から敵を発生させるクラス
/// </summary>
class SideSpawner :  public Spawner
{
private:
	int frame_=0;//一定時間ごとに敵の発生の時などに使用する
	std::shared_ptr<CollisionManager> collisionManager_;//
public:
	/// <summary>
	/// 両側から発生器
	/// </summary>
	/// <param name="pos">設定される発生器の座標</param>
	/// <param name="prototype">敵のプロトタイプ</param>
	/// <param name="enemyManager">敵マネージャ(敵登録用)</param>
	/// <param name="collisionManager">衝突判定マネージャ(当たり判定登録用)</param>
	/// <param name="camera">敵発生器の発動タイミングのためのカメラへの参照</param>
	/// <returns></returns>
	SideSpawner(const Position2f& pos, Enemy* prototype, 
		std::shared_ptr<EnemyManager>& enemyManager,
		std::shared_ptr<CollisionManager> collisionManager, 
		std::shared_ptr<Camera> camera);

	/// <summary>
	/// 時間ごとの発生などが必要なため毎フレーム更新を行う
	/// </summary>
	void Update() override;
};


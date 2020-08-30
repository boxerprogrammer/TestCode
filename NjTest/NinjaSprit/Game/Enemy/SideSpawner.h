#pragma once
#include "Spawner.h"

#include<memory>
#include<list>

class CollisionManager;
/// <summary>
/// 画面の両側から敵を発生させるクラス
/// </summary>
class SideSpawner :  public Spawner
{
private:
	int frame_=0;//一定時間ごとに敵の発生の時などに使用する
	std::shared_ptr<CollisionManager> collisionManager_;//
	//発生数制限(デフォルトでは制限なし)
	//-1のときは無限発生(それ以外のときは指定発生数に抑制される)
	int limitSpawnNum_ = -1;
	int averageSpawnFrame_ = 60;//発生頻度(平均発生タイミング)
	bool needBalance_ = false;
	struct Regulator {//調整するための構造体
		std::weak_ptr<Enemy> ownedWRef;//弱参照
		bool fromRight = true;//右から来たかどうか
		Regulator(std::weak_ptr<Enemy> ref, bool rgt) :ownedWRef(ref),fromRight(rgt){ }
	};
	std::list<Regulator> regulators_;//調整器

public:
	/// <summary>
	/// 両側から発生器
	/// </summary>
	/// <param name="pos">設定される発生器の座標</param>
	/// <param name="prototype">敵のプロトタイプ</param>
	/// <param name="enemyManager">敵マネージャ(敵登録用)</param>
	/// <param name="collisionManager">衝突判定マネージャ(当たり判定登録用)</param>
	/// <param name="camera">敵発生器の発動タイミングのためのカメラへの参照</param>
	/// <param name="averageSpawnFrame">平均発生フレーム時間</param>
	/// <param name="limitSpawnNum">発生数制限</param>
	/// <param name="needBalance">左右バランシングを行う</param>
	SideSpawner(const Position2f& pos, 
		Enemy* prototype, 
		std::shared_ptr<EnemyManager> enemyManager,
		std::shared_ptr<CollisionManager> collisionManager, 
		std::shared_ptr<Camera> camera,
		int averageSpawnFrame=60,
		int limitSpawnNum=-1,
		bool needBalance=false);

	/// <summary>
	/// 時間ごとの発生などが必要なため毎フレーム更新を行う
	/// </summary>
	void Update() override;
};


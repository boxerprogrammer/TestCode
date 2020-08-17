#pragma once
#include<memory>
#include<list>
#include"../../Geometry.h"
class Enemy;
class EnemyManager;
class Camera;
/// <summary>
/// 敵発生器基底クラス
/// </summary>
class Spawner
{
protected:
	Position2f pos_;//画面内にスポーナーがあるかどうかの判定が必要なため座標がいる
	std::unique_ptr<Enemy> prototype_;//プロトタイプ
	std::shared_ptr<EnemyManager> enemyManager_;//敵マネージャへの参照
	std::shared_ptr<Camera> camera_;//カメラへの参照
	//持っているプロトタイプのクローンをつくる
	//内部でプロトタイプじしんにコピーを作らせている
	virtual Enemy* CreateClone()final;
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="pos">発生器の場所</param>
	/// <param name="prototype">敵プロトタイプ</param>
	/// <param name="enemyManager">敵マネージャ</param>
	/// <param name="camera">カメラへの参照</param>
	/// <returns></returns>
	Spawner(const Position2f& pos, Enemy* prototype, std::shared_ptr<EnemyManager> em,std::shared_ptr<Camera> c);
	virtual ~Spawner()=default;
	
	/// <summary>
	/// 時間ごとの発生などが必要なため毎フレーム更新を行う
	/// </summary>
	virtual void Update() = 0;
};

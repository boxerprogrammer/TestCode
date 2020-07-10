#pragma once
#include<memory>
#include<list>
#include"../Geometry.h"
class Enemy;
/// <summary>
/// 敵発生器基底クラス
/// </summary>
class Spawner
{
protected:
	Position2f pos_;//画面内にスポーナーがあるかどうかの判定が必要なため座標がいる
	std::shared_ptr<Enemy> prototype_;//プロトタイプ
	std::list<std::shared_ptr<Enemy>>& enemies_;//敵リストへの参照(持ち主は別にいる)
	///最初に与えられたプロトタイプに基づくクローン生成する
	///派生クラスのSpawnやUpdateから呼び出す。
	std::shared_ptr<Enemy> CreateClone();
public:
	Spawner(const Position2f& pos,std::shared_ptr<Enemy> prototype, std::list<std::shared_ptr<Enemy>>& enemies);
	virtual ~Spawner()=default;
	/// <summary>
	/// 生成時にリストに放り込むが同時にシェアポインタも返す
	/// </summary>
	/// <returns></returns>
	virtual std::shared_ptr<Enemy> Spawn() = 0;
	
	/// <summary>
	/// 時間ごとの発生などが必要なため毎フレーム更新を行う
	/// </summary>
	virtual void Update() = 0;
};

#pragma once
#include<memory>
#include"../Geometry.h"
#include"EnemyType.h"

class Enemy;
class EnemyFactory;
//敵発生器(Spawner)
//敵は基本的にこの発生器から発生する
//基底クラス
class Spawner
{
protected:
	Position2 pos_;//スポーナーじしんの座標
	std::shared_ptr<Enemy> protoType_;//プロトタイプ(クローン元)
	std::shared_ptr<EnemyFactory> factory_;//ファクトリへの参照(ファクトリのリストに追加するため)
	bool isEnabled_=true;//スポーナー自身の寿命
public:
	Spawner(const Position2& pos ,//座標
		std::shared_ptr<Enemy> protoType,//プロトタイプ
		std::shared_ptr<EnemyFactory> factory);//ファクトリ

	bool IsEnabled()const;

	virtual std::shared_ptr<Enemy> Spawn() = 0;//敵発生
	virtual void Update(int scroll) = 0;//更新
	
};


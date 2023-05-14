#pragma once
#include<memory>
#include<list>
#include<map>
#include"../Geometry.h"
#include"EnemyType.h"

class Player;
class Enemy;

class Player;
class BulletFactory;
class ItemFactory;
/// <summary>
/// 敵工場
/// </summary>
class EnemyFactory
{
private:
	std::map<EnemyType, int> handleMap_;//敵の種別と画像ハンドルのペア
	std::map<BurstType, int> burstMap_;//爆発の種別と画像ハンドルのペア
	std::list<std::shared_ptr<Enemy>> enemies_;
	std::shared_ptr<Player> player_;
	std::shared_ptr<BulletFactory> bulletFactory_;
	std::shared_ptr<ItemFactory> itemFactory_;
	int frame_ = 0;
public:
	EnemyFactory(std::shared_ptr<Player> player, 
		std::shared_ptr<BulletFactory> bFactory,
		std::shared_ptr<ItemFactory> itemFactory);

	std::list<std::shared_ptr<Enemy>>& GetEnemies();

	/// <summary>
	/// 敵オブジェクトの生成を行う
	/// 生成して、内部のリストで管理する
	/// </summary>
	/// <param name="type">敵種別</param>
	/// <param name="pos">生成座標</param>
	/// <returns>敵オブジェクト</returns>
	std::shared_ptr<Enemy> Create(EnemyType type,const Position2 pos);

	/// <summary>
	/// 敵オブジェクトの生成のみを行う
	/// ※内部のリストへは追加しない
	/// </summary>
	/// <param name="type">敵種別</param>
	/// <returns>敵オブジェクトを返す</returns>
	std::shared_ptr<Enemy> Make(EnemyType type);

	/// <summary>
	/// 引数として渡されたオブジェクトを
	/// 敵リストに追加する
	/// </summary>
	/// <param name="enemy">敵オブジェクト</param>
	void AddEnemy(std::shared_ptr<Enemy> enemy);

	/// <summary>
	/// 全敵の更新
	/// </summary>
	void Update();
	/// <summary>
	/// 全敵の描画
	/// </summary>
	void Draw();
};


#pragma once
#include<list>
#include<memory>
class Enemy;
/// <summary>
/// 敵全体を管理する
/// </summary>
class EnemyManager
{
private:
	//敵を管理するためにリストで管理
	using Enemies_t = std::list<std::shared_ptr<Enemy>>;
	Enemies_t enemies_;
public:
	/// <summary>
	/// 敵オブジェクトをこのクラス管理下に置く
	/// </summary>
	/// <param name="enemy">敵オブジェクト</param>
	void AddEnemy(Enemy* enemy);

	/// <summary>
	/// 敵すべてへの参照を返す
	/// </summary>
	/// <returns>敵オブジェクトリストへの参照</returns>
	Enemies_t& Enemies();

	/// <summary>
	/// 全ての敵のUpdateを呼び出す
	/// </summary>
	void Update();

	/// <summary>
	/// 全ての敵のDrawを呼び出す
	/// </summary>
	void Draw();
};


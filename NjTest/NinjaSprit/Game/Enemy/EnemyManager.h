#pragma once
#include<list>
#include<memory>
class Enemy;
/// <summary>
/// “G‘S‘Ì‚ğŠÇ—‚·‚é
/// </summary>
class EnemyManager
{
private:
	using Enemies_t = std::list<std::shared_ptr<Enemy>>;
	Enemies_t enemies_;
public:

	void AddEnemy(Enemy* enemy);
	Enemies_t& Enemies();
	void Update();
	void Draw();
};


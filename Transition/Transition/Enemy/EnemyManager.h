#pragma once

#include<memory>
#include<list>

class Enemy;

class EnemyManager
{
	std::list<std::shared_ptr<Enemy>> enemies_;
public:
	void Update();
	void Draw();
};


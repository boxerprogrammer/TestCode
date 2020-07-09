#pragma once
#include<memory>
class Enemy;
/// <summary>
/// �G������
/// </summary>
class Spawner
{
protected:
	std::unique_ptr<Enemy> prototype_;
public:
	virtual ~Spawner()=default;
	virtual Enemy* Spawn() = 0;
	virtual void Update() = 0;
};


#pragma once
#include "Character.h"
class Player;
class Enemy :
	public Character
{
protected:
	std::shared_ptr<Player> player_;
	int life_ = 0;
public:
	Enemy(std::shared_ptr<Player> p);
	virtual void Update() = 0;
	virtual void Draw() = 0;
};


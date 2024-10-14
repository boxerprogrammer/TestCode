#pragma once
#include "Enemy.h"
class Slime :
    public Enemy
{
public:
	virtual void Update()override;
	virtual void Draw()override;
};


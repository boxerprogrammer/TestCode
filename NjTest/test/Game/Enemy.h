#pragma once
#include "Character.h"
class Enemy :
	public Character
{
public:
	void Update()override;
	void Draw()override;
};


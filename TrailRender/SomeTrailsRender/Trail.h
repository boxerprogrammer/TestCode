#pragma once
#include"Geometry.h"
#include<list>
class Particle;
class Trail
{
private:
	const Particle& particle_;
	std::list<Position2> history_;
public:
	Trail(const Particle& particle);
	void Update();
	void Draw();
};


#pragma once
#include"Geometry.h"
#include<list>
class Particle;
class ParticleSystem;
class Trail
{
	friend ParticleSystem;
private:
	const Particle& particle_;
	std::list<Position2> history_;
public:
	Trail(const Particle& particle);
	~Trail();
	void Update();
	void Draw();
};


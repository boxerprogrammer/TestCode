#pragma once
#include<memory>
#include"Geometry.h"
class Trail;
class ParticleSystem;
class Particle
{
	friend ParticleSystem;
private:
	Position2 pos_;
	Vector2 vel_;
	std::unique_ptr<Trail> trail_;
	ParticleSystem& particleSystem_;
	bool isActive_ = false;
public:
	Particle(ParticleSystem& ps, const Position2& pos);
	const Position2& GetPosition()const;
	void Draw();
};



#pragma once
#include<memory>
#include"Geometry.h"
class Trail;
class Particle
{
private:
	Position2 pos_;
	Vector2 vel_;
	std::unique_ptr<Trail> trail_;
public:
	void Draw();
};



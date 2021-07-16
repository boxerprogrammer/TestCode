#pragma once
#include<memory>
#include<vector>
class Particle;
class ParticleSystem
{
private:
	std::vector<std::shared_ptr<Particle>> particles_;
public:
	void Draw();
};


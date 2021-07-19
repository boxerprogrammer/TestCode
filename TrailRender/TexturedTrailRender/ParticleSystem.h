#pragma once
#include<memory>
#include<vector>
#include"DxLib.h"
class Particle;
class Trail;
class ParticleSystem
{
private:
	std::vector<std::shared_ptr<Particle>> particles_;
	std::vector<unsigned short> indices_;
public:
	ParticleSystem();
	~ParticleSystem();
	void Emit(size_t emitNum);
	bool IsTerminated()const ;
	void ClearParticles();
	void ClearVertices();
	void AddTrail(const Trail& trail);
	void Draw();
};


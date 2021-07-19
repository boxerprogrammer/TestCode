#include<DxLib.h>
#include<iterator>
#include "ParticleSystem.h"
#include"Particle.h"
#include"Trail.h"
namespace {
	std::vector<VERTEX2D> vertices_;
	int handle_;
}
ParticleSystem::ParticleSystem() {
	handle_=LoadGraph(L"white.png");
}
ParticleSystem::~ParticleSystem() {
	DeleteGraph(handle_);
}


void 
ParticleSystem::Emit(size_t emitNum) {
	int mx, my;
	GetMousePoint(&mx,&my);
	for (int i = 0; i < emitNum; ++i) {
		particles_.push_back(std::make_shared<Particle>(*this, Position2( mx,my )));
	}
}
bool 
ParticleSystem::IsTerminated()const {
	for (const auto& p : particles_) {
		if (p->isActive_) {
			return false;
		}
	}
	return true;
}
void 
ParticleSystem::ClearParticles() {
	particles_.clear();
	ClearVertices();
}

void
ParticleSystem::Draw() {
	if (!vertices_.empty()) {
		DrawPolygonIndexed2D(vertices_.data(), vertices_.size(), indices_.data(), indices_.size() / 3, handle_, true);
	}
	for (auto& p : particles_) {
		p->Draw();
	}
}

void 
ParticleSystem::ClearVertices() {
	vertices_.clear();
	indices_.clear();
}

void
ParticleSystem::AddVertices(const std::vector<VERTEX2D>& vertices,const std::vector<unsigned short>& indices) {
	std::copy(vertices.begin(), vertices.end(), std::back_inserter(vertices_));
	std::copy(indices.begin(), indices.end(), std::back_inserter(indices_));
}
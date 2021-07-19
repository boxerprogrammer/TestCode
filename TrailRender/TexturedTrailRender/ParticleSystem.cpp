#include<DxLib.h>
#include<iterator>
#include<cassert>
#include "ParticleSystem.h"
#include"Particle.h"
#include"Trail.h"
namespace {
	std::vector<VERTEX2D> vertices_;
	int handle_;
	Vector2 GetOrthogonalVector(const Vector2& v) {
		return { -v.y,v.x };
	}
}
ParticleSystem::ParticleSystem() {
	handle_=LoadGraph(L"arrow2.png",true);
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
	if (!vertices_.empty() && !indices_.empty()) {
		DrawPolygonIndexed2D(vertices_.data(), vertices_.size(), indices_.data(), indices_.size() / 3, handle_, true);
	}
	vertices_.clear();
	indices_.clear();
	for (auto& p : particles_) {
		p->Draw();
		AddTrail(*p->trail_);
	}

}

void 
ParticleSystem::ClearVertices() {
	vertices_.clear();
	indices_.clear();
}

void
ParticleSystem::AddTrail(const Trail& trail) {
	if (trail.history_.empty())return;
	float thickness = 64.0f;
	Position2 lastpos= trail.history_.front();
	Vector2 lastRv = Vector2::Zero;
	float div=trail.history_.size();
	float u = 0.0f;
	//入る前のインデックス(サイズ)を記録
	size_t lastVertSize = vertices_.size();

	for (auto& pos : trail.history_) {
		Vector2 v = pos - lastpos;
		Vector2 rv = {};
		if (v == Vector2::Zero) {
			lastpos = pos;
			continue;
		}
		if (v != Vector2::Zero) {
			rv = GetOrthogonalVector(v).Normalized();// *(thickness * 0.5f);
			if (isnan(rv.x)) {
				lastpos = pos;
				continue;
			}
		}
		Vector2 rightV = rv;
		if (lastRv != Vector2::Zero) {
			if (isnan(lastRv.x)) {
				assert(0);
			}

			rightV = lastRv;// (rv + lastRv);
			if (rightV == Vector2::Zero) {
				lastpos = pos;
				continue;
			}
			rightV.Normalize();
		}
		assert(!isnan(rightV.x));

		Vector2 headV = rightV * (thickness * 0.5f);
		auto p1 = lastpos - headV;
		auto p2 = lastpos + headV;
		thickness *= 0.9f;
		headV = rv * (thickness * 0.5f);
		//auto p0 = pos - headV;
		//auto p3 = pos + headV;

		//DrawQuadrangleAA(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 0xffffff, true);
		VERTEX2D vert;
		vert.pos.x = p1.x;
		vert.pos.y = p1.y;
		vert.pos.z = 0;
		vert.u = u;
		vert.v = 0.0f;
		vert.rhw = 1.0f;
		vertices_.push_back(vert);
		vert.pos.x = p2.x;
		vert.pos.y = p2.y;
		vert.pos.z = 0;
		vert.u = u;
		vert.v = 1.0f;
		vert.rhw = 1.0f;
		u += 1.0f/div;
		vertices_.push_back(vert);

		lastpos = pos;
		lastRv = rv;
	}
	if (vertices_.empty())return;
	size_t vert_num = vertices_.size() - lastVertSize;
	if (vert_num == 0)return;
	std::vector<unsigned short> indices((vert_num / 2 - 1) * 2 * 3);
	unsigned short index = lastVertSize;
	for (int i = 0; i < indices.size() ; i+=6) {
		//三角形①
		indices[i] = index;
		indices[i+1] = index + 2;
		indices[i+2] = index + 1;

		//三角形②
		indices[i+3] = index + 1;
		indices[i+4] = index + 2;
		indices[i+5] = index + 3;
		index += 2;
	}
	std::copy(indices.begin(), indices.end(), std::back_inserter(indices_));
}
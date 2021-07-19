#include "Trail.h"
#include"Particle.h"
#include<DxLib.h>
constexpr size_t trail_limit = 24;
Trail::Trail(const Particle& particle):particle_(particle) {

}
Trail::~Trail() {
	history_.clear();
}

void
Trail::Draw() {
	//auto lastpos=particle_.GetPosition();
	//float thickness = 32.0f;
	//for (auto& pos : history_) {
	//	DrawLineAA(lastpos.x, lastpos.y, pos.x, pos.y, 0xffffff, thickness);
	//	thickness *= 0.85f;
	//	lastpos = pos;
	//}


	history_.push_front(particle_.GetPosition());
	if (history_.size() >= trail_limit) {
		history_.pop_back();
	}

}
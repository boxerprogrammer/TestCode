#include "Particle.h"
#include"Trail.h"
#include<DxLib.h>

Particle::Particle() {

}

void 
Particle::Draw() {
	DrawCircleAA(pos_.x, pos_.y, 30, 16, 0xfffffff);
	trail_->Draw();
}
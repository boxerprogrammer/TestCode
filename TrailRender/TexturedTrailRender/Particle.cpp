#include "Particle.h"
#include"Trail.h"
#include<DxLib.h>

constexpr float particle_speed_base = 8.0f;
constexpr float particle_speed_sigma = 2.0f;

Particle::Particle(ParticleSystem& ps,const Position2& pos):particleSystem_(ps) ,pos_(pos){
	float angle = (rand() % 360) / 360.0f * 2*DX_PI_F;
	vel_ = {cosf(angle),sinf(angle)};
	vel_ *= particle_speed_base+(float)(rand()%((int)particle_speed_sigma*2)- particle_speed_sigma);
	isActive_ = true;
	trail_ = std::make_unique<Trail>(*this);
}

const 
Position2& 
Particle::GetPosition()const {
	return pos_;
}

void 
Particle::Draw() {
	if (!isActive_)return;
	pos_ += vel_;
	vel_.y += 0.12f;
	//DrawCircleAA(pos_.x, pos_.y, 16, 12, 0xfffffff);
	trail_->Draw();
	if (pos_.x < -300 || pos_.y < -300 || pos_.x>940 || pos_.y>780) {
		isActive_ = false;
	}
}
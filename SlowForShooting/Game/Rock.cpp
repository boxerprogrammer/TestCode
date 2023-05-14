#include "Rock.h"
#include<DxLib.h>
#include"../DrawFunctions.h"

constexpr int img_width = 32;
constexpr int img_height = 32;
constexpr float img_scale = 3.0f;

//爆発系の定数
constexpr int burst_img_width = 48;
constexpr int burst_img_height = 48;
constexpr float burst_img_scale = 3.0f;
constexpr int burst_frame_num = 8;//アニメーション枚数
constexpr int burst_frame_speed = 10;//１枚に必要なフレーム数
constexpr float bullet_speed = 2.0f;//

Rock::Rock(std::shared_ptr<Player> player, const Position2& pos, int handle, int burstH, std::shared_ptr<BulletFactory> bFactory)
    :Enemy(player,pos,bFactory),handle_(handle),burstH_(burstH),
    drawFunc_(&Rock::NormalDraw),
    updateFunc_(& Rock::NormalUpdate)
{
	//ランダムに方向と速度を決定する
	vel_.x = -1.0f;
	vel_.y=static_cast<float>(GetRand(100)-50) / 50.0f;
	vel_.Normalize();
	float speed=static_cast<float>(GetRand(100)) / 20.0f;
	vel_ *= speed;	

	rect_ = { pos, { static_cast<int>(img_width * img_scale),static_cast<int>(img_height * img_scale) } };
}

void Rock::OnDamage(int damage)
{
    life_ -= damage;
	if (life_ <= 0) {
		updateFunc_ = &Rock::BurstUpdate;
		drawFunc_ = &Rock::BurstDraw;
	}
}


std::shared_ptr<Enemy> Rock::Clone()
{

    auto rock = std::make_shared<Rock>(*this);
	rock->scale_ = static_cast<float>(GetRand(20) + 20) / 10.0f;
	rock->rect_ = { rect_.center, { static_cast<int>(img_width * rock->scale_),
		static_cast<int>(img_height * rock->scale_) } };
	rock->vel_.x = -1.0f;
	rock->vel_.y = static_cast<float>(GetRand(100) - 50) / 50.0f;
	rock->vel_.Normalize();
	float speed = static_cast<float>(GetRand(60)+20) / 20.0f;
	rock->vel_ *= speed;
	return rock;
}


void Rock::NormalUpdate()
{

	rect_.center += vel_;
	
	if (rect_.center.x < -200) {
		isEnabled_ = false;
	}
}

void Rock::BurstUpdate()
{
	idx_++;
	if (idx_ == burst_frame_num * burst_frame_speed) {
		isEnabled_ = false;
	}
}

void Rock::NormalDraw()
{
	int imgX = 0;
	auto angle = atan2f(vel_.y, vel_.x) + 3.14159265f;
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,
		imgX, 0,
		img_width, img_height,
		scale_, angle,
		handle_, true);

	rect_.Draw(0x00ff00);
}

void Rock::BurstDraw()
{
	int imgX = (idx_ / burst_frame_speed) * burst_img_width;
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,
		imgX, 0,
		burst_img_width, burst_img_height,
		scale_, 0.0f,
		burstH_, true);
}



bool Rock::IsCollidable() const
{
	return isEnabled_ && (updateFunc_ == &Rock::NormalUpdate);
}

void Rock::Update()
{
	(this->*updateFunc_)();
}

void Rock::Draw()
{
	(this->*drawFunc_)();
}

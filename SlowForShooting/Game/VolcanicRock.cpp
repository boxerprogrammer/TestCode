#include "VolcanicRock.h"
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

VolcanicRock::VolcanicRock(std::shared_ptr<Player> player, const Position2& pos, int handle, int burstH, std::shared_ptr<BulletFactory> bFactory)
	:Enemy(player, pos, bFactory), handle_(handle), burstH_(burstH),
	drawFunc_(&VolcanicRock::NormalDraw),
	updateFunc_(&VolcanicRock::NormalUpdate)
{
	//ランダムに方向と速度を決定する
	//X方向を±(-1～1)
	vel_.x = static_cast<float>(GetRand(40) - 20) / 20.0f;
	//Y方向を-のみ(-20～-4)
	vel_.y = - static_cast<float>(GetRand(80) + 20) / 5.0f;
	vel_.Normalize();
	//速度を(-20～-4)にしている。
	float speed = static_cast<float>(GetRand(80) + 20) / 5.0f;
	vel_ *= speed;

	rect_ = { pos, { static_cast<int>(img_width * img_scale),static_cast<int>(img_height * img_scale) } };
}

void VolcanicRock::OnDamage(int damage)
{
	life_ -= damage;
	if (life_ <= 0) {
		updateFunc_ = &VolcanicRock::BurstUpdate;
		drawFunc_ = &VolcanicRock::BurstDraw;
	}
}


std::shared_ptr<Enemy> VolcanicRock::Clone()
{
	auto rock = std::make_shared<VolcanicRock>(*this);
	rock->scale_ = static_cast<float>(GetRand(10) + 10) / 10.0f;
	rock->rect_ = { rect_.center, { static_cast<int>(img_width * rock->scale_),
		static_cast<int>(img_height * rock->scale_) } };
	//xは-1～1
	rock->vel_.x = static_cast<float>(GetRand(40) - 20) / 20.0f;
	//y=-10～-2
	rock->vel_.y = -static_cast<float>(GetRand(80) + 20) / 10.0f;
	rock->vel_.Normalize();
	//スピードは2～10ですよ。
	float speed = static_cast<float>(GetRand(80) + 20) / 10.0f;
	rock->vel_ *= speed;
	return rock;
}


void VolcanicRock::NormalUpdate()
{

	rect_.center += vel_;
	vel_.y += gravity_;//重力の影響を受ける

	if (rect_.center.x < -200 || rect_.center.y>480+200) {
		isEnabled_ = false;
	}
}

void VolcanicRock::BurstUpdate()
{
	idx_++;
	if (idx_ == burst_frame_num * burst_frame_speed) {
		isEnabled_ = false;
	}
}

void VolcanicRock::NormalDraw()
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

void VolcanicRock::BurstDraw()
{
	int imgX = (idx_ / burst_frame_speed) * burst_img_width;
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,
		imgX, 0,
		burst_img_width, burst_img_height,
		scale_, 0.0f,
		burstH_, true);
}



bool VolcanicRock::IsCollidable() const
{
	return isEnabled_ && (updateFunc_ == &VolcanicRock::NormalUpdate);
}

void VolcanicRock::Update()
{
	(this->*updateFunc_)();
}

void VolcanicRock::Draw()
{
	(this->*drawFunc_)();
}

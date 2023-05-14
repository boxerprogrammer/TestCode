#include "Insect.h"
#include"../DrawFunctions.h"
#include"BulletFactory.h"
#include"Player.h"
#include<DxLib.h>


constexpr int img_width = 32;
constexpr int img_height = 32;
constexpr float img_scale = 2.0f;

//爆発系の定数
constexpr int burst_img_width = 48;
constexpr int burst_img_height = 48;
constexpr float burst_img_scale = 1.0f;
constexpr int burst_frame_num = 8;//アニメーション枚数
constexpr int burst_frame_speed = 5;//１枚に必要なフレーム数
constexpr float bullet_speed = 4.0f;//

void Insect::NormalUpdate()
{

	vel_.x = -2.0f;
	vel_.y = -sinf(angle_);
	angle_ += 0.01f;
	vel_.Normalize();
	vel_ *= 2.0f;

	rect_.center+=vel_;
	

	if (rect_.center.x < -100) {
		isEnabled_ = false;
	}
	fireFrame_--;
	if (fireFrame_ == 0) {
		fireFrame_ = (fire_interval + //60フレーム
			GetRand(fire_rand_rate * 2)  //+0～20フレーム
			- fire_rand_rate);//-10フレーム=50～70フレーム
		//自機狙い弾を作る
		//自機狙いベクトル = 終点(プレイヤー座標) - 始点(敵機自身の座標)
		auto vel = player_->GetPosition() - rect_.center;
		if (vel.SQLength() == 0.0f) {
			//敵機と自機が重なってるときは基底の方向
			//真左に飛ばしておきます。
			vel = { -bullet_speed,0.0f };
		}
		else {
			//状況によってvelが変化してしまうため、いったん正規化する(大きさ1)
			vel.Normalize();
			//vel *= bullet_speed;//スピードを乗算する
		}
		bulletFactory_->Create(BulletType::Zako, rect_.center, vel);
	}
}

void Insect::BurstUpdate()
{
	idx_++;
	if (idx_ == burst_frame_num * burst_frame_speed) {
		isEnabled_ = false;
	}
}

void Insect::NormalDraw()
{
	int imgX = 0;
	auto angle =  atan2f(vel_.y, vel_.x)+3.14159265f;
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,
		imgX, 0,
		img_width, img_height,
		img_scale, angle,
		handle_, true);

	rect_.Draw(0x00ff00);
}

void Insect::BurstDraw()
{
	int imgX = (idx_ / burst_frame_speed) * burst_img_width;
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,
		imgX, 0,
		burst_img_width, burst_img_height,
		burst_img_scale, 0.0f,
		burstH_, true);
}

Insect::Insect(std::shared_ptr<Player> player,
	const Position2& pos,
	int handle,
	int burstH,
	std::shared_ptr<BulletFactory> bFactory) :
	Enemy(player, pos, bFactory),
	handle_(handle),
	burstH_(burstH)
{
	updateFunc_ = &Insect::NormalUpdate;
	drawFunc_ = &Insect::NormalDraw;
	rect_ = { pos, { static_cast<int>(img_width * img_scale),static_cast<int>(img_height * img_scale) } };
}

void Insect::OnDamage(int damage)
{
	//ダメージが入ると、爆発アニメーションに移行する
	updateFunc_ = &Insect::BurstUpdate;
	drawFunc_ = &Insect::BurstDraw;
	
}

bool Insect::IsCollidable() const
{
	return isEnabled_ && (updateFunc_ == &Insect::NormalUpdate);
}

void Insect::Update()
{
	(this->*updateFunc_)();
}

void Insect::Draw()
{
	(this->*drawFunc_)();
}

std::shared_ptr<Enemy>
Insect::Clone()
{
	return std::make_shared<Insect>(*this);
}

#include "Patapata.h"
#include"../DrawFunctions.h"
#include"BulletFactory.h"
#include"../SoundManager.h"
#include"Player.h"
#include<DxLib.h>

constexpr int anim_frame_speed = 10;//１枚に必要なフレーム数
constexpr int anim_frame_num = 5;//アニメーション枚数
constexpr int img_width = 29;
constexpr int img_height = 29;
constexpr float img_scale = 2.0f;

//爆発系の定数
constexpr int burst_img_width = 48;
constexpr int burst_img_height = 48;
constexpr float burst_img_scale = 1.0f;
constexpr int burst_frame_num = 8;//アニメーション枚数
constexpr int burst_frame_speed = 5;//１枚に必要なフレーム数
constexpr float bullet_speed = 4.0f;//

void Patapata::NormalUpdate()
{
	rect_.center.x -= 4.0f;
	idx_ = (idx_ + 1) % (anim_frame_speed * anim_frame_num);
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
		bulletFactory_->Create(BulletType::Zako,rect_.center, vel);
	}
}

void Patapata::BurstUpdate()
{
	idx_++;
	if (idx_ == burst_frame_num*burst_frame_speed) {
		isEnabled_ = false;
	}
}

void Patapata::NormalDraw()
{
	int imgX = (idx_ / anim_frame_speed) * img_width;
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,
		imgX, 0,
		img_width, img_height,
		img_scale, 0.0f,
		handle_, true);

	rect_.Draw(0x00ff00);
}

void Patapata::BurstDraw()
{
	int imgX = (idx_ / burst_frame_speed) * burst_img_width;
	my::MyDrawRectRotaGraph(
		rect_.center.x, rect_.center.y,
		imgX, 0,
		burst_img_width, burst_img_height,
		burst_img_scale, 0.0f,
		burstH_, true);
}

Patapata::Patapata(std::shared_ptr<Player> player, 
				const Position2& pos, 
				int handle, 
				int burstH,
				std::shared_ptr<BulletFactory> bFactory) :
	Enemy(player, pos,bFactory), 
	handle_(handle),
	burstH_(burstH)
{
	updateFunc_ = &Patapata::NormalUpdate;
	drawFunc_ = &Patapata::NormalDraw;
	rect_ = { pos, { static_cast<int>(img_width * img_scale),static_cast<int>(img_height * img_scale) } };
}

void Patapata::OnDamage(int damage)
{
	//ダメージが入ると、爆発アニメーションに移行する
	updateFunc_ = &Patapata::BurstUpdate;
	drawFunc_ = &Patapata::BurstDraw;
	idx_ = 0;//インデックスは0にもどしておく
	SoundManager::GetInstance().Play(L"burst");
}

bool Patapata::IsCollidable() const
{
	return isEnabled_ && (updateFunc_ == &Patapata::NormalUpdate);
}

void Patapata::Update()
{
	(this->*updateFunc_)();
}

void Patapata::Draw()
{
	(this->*drawFunc_)();
}

std::shared_ptr<Enemy> 
Patapata::Clone()
{
	return std::make_shared<Patapata>(*this);
}

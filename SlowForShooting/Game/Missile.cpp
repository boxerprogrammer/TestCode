#include "Missile.h"
#include<DxLib.h>
#include"../DrawFunctions.h"
#include"EnemyFactory.h"
#include"Enemy.h"

constexpr int img_width = 19;
constexpr int img_height = 6;
constexpr float img_scale = 2.0f;

Missle::Missle(int handle,std::shared_ptr<EnemyFactory> ef) :handle_(handle),ef_(ef),
vel_(6.0f, 0.0f),//弾のスピード
rect_({}, { static_cast<int>(img_width * img_scale),
			static_cast<int>(img_height * img_scale) })
{
}

void Missle::SetVelocity(const Vector2& vec)
{
	vel_ = vec;
}

const Rect& Missle::GetRect() const
{
	return rect_;
}

void Missle::Kill()
{
	isEnabled_ = false;
}

void Missle::Fire(const Position2& pos)
{
	rect_.center = pos;
	isEnabled_ = true;

}

bool Missle::IsEnabled() const
{
	return isEnabled_;
}

void Missle::Update()
{
	//死んでるなら動かさない
	if (!isEnabled_)return;

	
	auto& enemies = ef_->GetEnemies();
	if (!enemies.empty()) {
		auto enemy = enemies.front();

		auto vec = enemy->GetRect().center - rect_.center;
		vec.Normalize();
		vec *= 0.1;

		vel_ += vec;
		vel_.Normalize();
		vel_ *= 3.0f;
	}
	rect_.center += vel_;//弾を現在速度で移動させる

	//画像のインデックスを切り替えてる
	imgIdx_ = (imgIdx_ + 1) % 2;

	//弾が画面外に出たら、死んだことにする
	if (rect_.center.x < -img_width || 640 + img_width < rect_.center.x) {
		isEnabled_ = false;
	}
	if (rect_.center.y < -img_height || 480 + img_height < rect_.center.y) {
		isEnabled_ = false;
	}
}

void Missle::Draw()
{
	//死んでるのなら表示しない
	if (!isEnabled_)return;
	my::MyDrawRectRotaGraph(rect_.center.x, rect_.center.y,
		imgIdx_ * img_width, 0,
		img_width, img_height,
		img_scale, 0.0f,
		handle_, true);

	rect_.Draw(0xff0000);
}

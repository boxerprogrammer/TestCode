#include "Shot.h"
#include<DxLib.h>
#include"../DrawFunctions.h"
#include"../SoundManager.h"

constexpr int img_width = 19;
constexpr int img_height = 6;
constexpr float img_scale = 2.0f;

Shot::Shot(int handle) :handle_(handle),
vel_(10.0f,0.0f),//弾のスピード
rect_({}, { static_cast<int>(img_width * img_scale),
			static_cast<int>(img_height * img_scale) })
{
}

void Shot::SetVelocity(const Vector2& vec)
{
	vel_ = vec;
}

const Rect& Shot::GetRect() const
{
	return rect_;
}

void Shot::Kill()
{
	isEnabled_ = false;
}

void Shot::Fire(const Position2& pos)
{
	rect_.center = pos;
	isEnabled_ = true;
	SoundManager::GetInstance().Play(L"shot");
}

bool Shot::IsEnabled() const
{
	return isEnabled_;
}

void Shot::Update()
{
	//死んでるなら動かさない
	if (!isEnabled_)return;
	rect_.center += vel_;//弾を現在速度で移動させる

	//画像のインデックスを切り替えてる
	imgIdx_ = (imgIdx_ + 1) % 2;

	//弾が画面外に出たら、死んだことにする
	if (rect_.center.x < -img_width ||  640 + img_width< rect_.center.x) {
		isEnabled_ = false;
	}
	if (rect_.center.y < -img_height || 480 + img_height < rect_.center.y) {
		isEnabled_ = false;
	}
}

void Shot::Draw()
{
	//死んでるのなら表示しない
	if (!isEnabled_)return;
	my::MyDrawRectRotaGraph(rect_.center.x, rect_.center.y,
		imgIdx_ * img_width, 0,
		img_width, img_height,
		img_scale, 0.0f,
		handle_,true);
	
	rect_.Draw(0xff0000);
}

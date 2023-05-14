#include "ZakoBullet.h"
#include"../DrawFunctions.h"

ZakoBullet::ZakoBullet(int handle):handle_(handle)
{
}

void ZakoBullet::Update()
{
	rect_.center += vel_;
	const auto& pos = rect_.center;
	if (pos.x < -8 || 640 + 8 < pos.x ||
		pos.y < -8 || 480 + 8 < pos.y) {
		isEnabled_ = false;
	}
}

void ZakoBullet::Draw()
{
	my::MyDrawRectRotaGraph(
		rect_.center.x,
		rect_.center.y,
		0, 0,
		8, 8,
		1.0f, 0.0f, handle_, true);
}

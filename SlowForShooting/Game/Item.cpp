#include "Item.h"
#include"../DrawFunctions.h"
#include<DxLib.h>
#include"../SoundManager.h"

namespace{
	constexpr int img_width = 24;
	constexpr int img_height = 24;
	constexpr float img_scale = 2.0f;
}
Item::Item(int handle, ItemType type):handle_(handle),type_(type),isEnabled_(true)
{
	rect_.size.w = img_width * img_scale;
	rect_.size.h = img_height * img_scale;
}

Item::~Item()
{
}

const Rect& Item::GetRect() const
{
	return rect_;
}

void Item::SetPosition(const Position2& pos)
{
	rect_.center = pos;
}

const ItemType Item::GetItemType() const
{
	return type_;
}

bool Item::IsEnabled() const
{
	return isEnabled_;
}

void Item::OnTouch()
{
	isEnabled_ = false;
	SoundManager::GetInstance().Play(L"powerup");
	
}

void Item::Update()
{
	rect_.center.x -= 1.0f;
	if (rect_.center.x < -100) {
		isEnabled_ = false;
	}
}

void Item::Draw()
{
	int idxX = 0;
	int idxY = 0;
	switch (type_) {
	case ItemType::laser1:
		idxX = 0;
		idxY = 2;
		break;
	case ItemType::missile:
		idxX = 1;
		idxY = 6;
	}

	my::MyDrawRectRotaGraph(
		rect_.center.x,
		rect_.center.y,
		img_width * idxX,//ˆê”Ô¶
		img_height * idxY,//ã‚©‚ç3”Ô‚ß
		img_width, img_height,
		img_scale, 0.0f,
		handle_,
		true
	);
	rect_.Draw(0x0000ff);
}

#include "BGPart.h"
#include<cassert>
#include<DxLib.h>
#include"../File/ImageFile.h"

BGPart::BGPart(const std::shared_ptr<File> file, float scale, Position2f pos, Vector2f rate, int priority) :
	pos_(pos),
	rate_(rate),
	scale_(scale),
	priority_(priority),
	screenW_(640),
	file_(file)
{
	auto checkP = std::dynamic_pointer_cast<ImageFile>(file);
	assert(checkP != nullptr);
	GetGraphSize(file->GetHandle(), &imgSize_.w, &imgSize_.h);
}

BGPart::~BGPart()
{

}

bool BGPart::operator<(const BGPart& val)
{
	return priority_ < val.priority_;
}

void BGPart::Update()
{
}

void BGPart::Move(const Vector2f& v)
{
	pos_ += v * rate_;
}

void BGPart::Draw()
{
	//‚Ü‚¸‰½–‡•K—v‚©ŒvŽZ‚·‚é
	int num = screenW_ / (imgSize_.w*scale_)+1+(screenW_ % (int)(imgSize_.w * scale_)>0?1:0);
	int sign =pos_.x>0.0f?1:-1;
	auto mod = ((int)fabsf(pos_.x) % (int)(imgSize_.w*scale_))*sign;
	int idxOffset = mod > 0 ? -1 : 0;
	for (int i = 0; i < num; ++i) {
		DrawRotaGraph(
			mod + (imgSize_.w / 2) * scale_ + (i+ idxOffset)*imgSize_.w * scale_,
			pos_.y + (imgSize_.h / 2) * scale_,
			scale_, 0.0f, file_->GetHandle(), true);
	}

}

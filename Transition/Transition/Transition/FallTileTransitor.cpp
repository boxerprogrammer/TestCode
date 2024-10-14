#include "FallTileTransitor.h"
#include<DxLib.h>
#include"../Application.h"

namespace {
	int CeilDivive(int lval,int rval){
		return (lval + ((rval - lval % rval) % rval) / rval);
	}
	constexpr int additional_time = 60;
}

FallTileTransitor::FallTileTransitor(int cellSize, float gravity,int interval) :cellSize_(cellSize),
Transitor(interval),
g_(gravity)
{
	const auto& wsize=Application::GetInstance().GetWindowSize();
	int xnum = (wsize.w / cellSize_)+1;
	int ynum = (wsize.h / cellSize_) + 1;
	tiles_.reserve(xnum * ynum);
	for (int yidx = 0; yidx < ynum; ++yidx) {
		for (int xidx = 0; xidx < xnum; ++xidx) {
			tiles_.push_back({xidx,yidx});
		}
	}
	std::shuffle(tiles_.begin(), tiles_.end(), mt_);
}

void FallTileTransitor::Update()
{
	
	if (frame_ < interval_+ additional_time) {
		++frame_;
		SetDrawScreen(newRT_);
	}
	else if (frame_ == interval_+ additional_time) {
		SetDrawScreen(DX_SCREEN_BACK);
	}
	if (IsEnd()) {
		return;
	}

	for (auto& cell : tiles_) {
		if (cell.vy > 0.0f) {
			cell.vy += g_;
			cell.yoffset += cell.vy;
		}
	}
	
	auto rit = tiles_.rbegin();
	const auto& wsize = Application::GetInstance().GetWindowSize();
	int xnum = (wsize.w / cellSize_) + 1;
	int ynum = (wsize.h / cellSize_) + 1;
	int fallNum = ((xnum * ynum) / interval_);

	for (int i=0; rit != tiles_.rend() && i<fallNum; ++rit) {
		
		if (rit->vy > 0.0f || rit->yoffset > 0) {
			continue;
		}
		else {
			++i;
			rit->vy += g_;
			rit->yoffset += rit->vy;
		}
	}

	


}

void FallTileTransitor::Draw()
{
	if (IsEnd()) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	float amp=frame_ % 31;
	int yoffset = 10*((amp / 30.0f) - 1.0f);
	const auto& wsize = Application::GetInstance().GetWindowSize();
	auto rate = (float)frame_ / (float)interval_;
	DrawRectGraph(0, yoffset, 0, 0, wsize.w, wsize.h, newRT_, true);
	for (const auto& cell : tiles_) {
		DrawRectGraph(
			cell.xidx * cellSize_,
			cell.yidx * cellSize_+cell.yoffset + yoffset,
			cell.xidx * cellSize_,
			cell.yidx * cellSize_,
			cellSize_, cellSize_,
			oldRT_, true);
	}
}


bool
FallTileTransitor::IsEnd() const
{
	return frame_ >= interval_+ additional_time;
}



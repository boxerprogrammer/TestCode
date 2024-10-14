#include "StripTransitor.h"
#include"../Application.h"
#include<DxLib.h>

StripTransitor::StripTransitor(int width, int interval):Transitor(interval),
width_(width)
{
}

void StripTransitor::Update()
{
	
	if (frame_ < interval_) {
		++frame_;
		SetDrawScreen(newRT_);
	}
	else if (frame_ == interval_) {
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

void StripTransitor::Draw()
{
	if (IsEnd()) {
		return;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	const auto& wsize=Application::GetInstance().GetWindowSize();
	auto rate = (float)frame_ / (float)interval_;
	int lp = (wsize.w / width_) + 1;
	DrawRectGraph(0, 0, 0, 0, wsize.w, wsize.h, oldRT_, true);
	for (int i = 0; i < lp; ++i) {
		if (i % 2 == 0) {
			DrawRectGraph(i * width_, wsize.h*(rate-1.0f), i * width_, 0, width_, wsize.h, newRT_, true);
		}
		else {
			DrawRectGraph(i * width_, wsize.h * (1.0f-rate), i * width_, 0, width_, wsize.h, newRT_, true);
		}
	}
}



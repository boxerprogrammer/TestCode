#include "IrisTransitor.h"
#include"../Application.h"
#include<DxLib.h>
#include<cmath>

IrisTransitor::IrisTransitor(bool irisOut,int interval, bool isTiled,int gHandle) :Transitor(interval),
irisOut_(irisOut),
isTiled_(isTiled),
gHandle_(gHandle)
{
	const auto& wsize=Application::GetInstance().GetWindowSize();
	handleForMaskScreen_ = MakeScreen(wsize.w, wsize.h,true);
	maskH_ = CreateMaskScreen();
	diagonalLength_ = std::hypotf(wsize.w, wsize.h)/2.0f;
}

IrisTransitor::~IrisTransitor()
{
	DeleteMaskScreen();
}

void IrisTransitor::Update()
{

	if (frame_ < interval_) {
		++frame_;
		SetDrawScreen(newRT_);
	}
	else if (frame_ == interval_) {
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

void IrisTransitor::Draw()
{
	if (IsEnd()) {
		return;
	}
	
	auto rate = (float)frame_ / (float)interval_;
	int backRT = oldRT_;
	int maskedRT = newRT_;
	if (irisOut_) {
		backRT = newRT_;
		maskedRT = oldRT_;
		rate = 1.0f - rate;
	}
	//
	float radius = (diagonalLength_ ) * rate;
	SetDrawScreen(handleForMaskScreen_);
	ClearDrawScreen();
	
	if (gHandle_ == -1) {
		if (isTiled_) {
			constexpr int xdiv = (640 / 50) + 1;
			constexpr int ydiv = (640 / 50) + 1;
			radius = (50 * diagonalLength_ / 320) * rate;
			for (int y = 0; y < xdiv; ++y) {
				for (int x = 0; x < xdiv; ++x) {
					DrawCircleAA(50 + x * 100, 50 + y * 100, radius, 32, 0xffffff, true);
				}
			}
		}
		else {
			DrawCircleAA(320, 240, radius, 32, 0xffffff, true);
		}
	}
	else {
		DrawRotaGraphFast(320, 240, rate * 3.5f, rate * 2.0f, gHandle_, true);
	}
	
	
	
	//隠し関数(現在のグラフィックハンドルをマスクスクリーンに転送)
	SetMaskScreenGraph(handleForMaskScreen_);
	//描画領域を反転する
	SetMaskReverseEffectFlag(true);

	SetDrawScreen(DX_SCREEN_BACK);
	SetUseMaskScreenFlag(false);
	DrawGraph(0, 0, backRT, true);
	SetUseMaskScreenFlag(true);
	DrawGraph(0, 0, maskedRT, true);
	SetUseMaskScreenFlag(false);
}



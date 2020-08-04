#include "Debugger.h"
#include<DxLib.h>
#include"../Geometry.h"
namespace {
	bool keyF1Pressed_ = false;
	bool debugDisp_ = false;
}

bool 
Debugger::IsDebugMode()const {
	return debugDisp_;
}

void
Debugger::Update() {
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_F1)) {
		if (!keyF1Pressed_) {
			debugDisp_ = !debugDisp_;
		}
		keyF1Pressed_ = true;
	}
	else {
		keyF1Pressed_ = false;
	}
#endif
}

Debugger& 
Debugger::Instance() {
	static Debugger instance;
	return instance;
}

void 
Debugger::DisplayPerformance() {
#ifdef _DEBUG
	if (IsDebugMode()) {
		DrawFormatString(32, 32, 0x000000, L"FPS=%.2f", DxLib::GetFPS());
		DrawFormatString(30, 30, 0xffffff, L"FPS=%.2f", DxLib::GetFPS());
		DrawFormatString(32, 62, 0x000000, L"DCCount=%d", DxLib::GetDrawCallCount());
		DrawFormatString(30, 60, 0xffffff, L"DCCount=%d", DxLib::GetDrawCallCount());
	}
#endif
}


void 
Debugger::Draw(const Circle& circle, unsigned int color, float xoffset , float yoffset, float thickness) {
#ifdef _DEBUG
	if (IsDebugMode()) {
		const auto& spos = circle.center;
		DrawCircleAA(
			spos.x+xoffset,
			spos.y+yoffset,
			circle.radius,20,
			color, false, thickness);
	}
#endif
}
void 
Debugger::Draw(const Segment& seg, unsigned int color, float xoffset , float yoffset, float thickness) {
#ifdef _DEBUG
	if (IsDebugMode()) {
		const auto& spos = seg.start+Vector2f(xoffset,yoffset);
		auto epos = spos + seg.vec;
		DrawLineAA(spos.x, spos.y, epos.x, epos.y, color, thickness);
	}
#endif
}

void 
Debugger::Draw(const Capsule& cap, unsigned int color, float xoffset , float yoffset , float thickness) {
#ifdef _DEBUG
	if (IsDebugMode()) {
		if (cap.seg.vec.SQMagnitude() == 0)return;
		uint32_t color = 0xffaaaa;

		
		auto& spos = cap.seg.start;
		auto epos = cap.seg.start + cap.seg.vec;

		auto offset = Vector2f(xoffset,yoffset);
		DrawCircleAA(
			static_cast<int>(spos.x + offset.x),
			static_cast<int>(spos.y),
			static_cast<int>(cap.radius),
			32,
			color, false, 2);

		auto v90 = cap.seg.vec;//元のベクトルのコピー
		v90 = { -v90.y, v90.x };//90°回転
		v90.Normalize();//正規化
		v90 *= cap.radius;//半径をかける
		auto p1 = spos + v90;//左上
		auto p2 = epos + v90;//右上
		auto p3 = epos - v90;//右下
		auto p4 = spos - v90;//左下

		DrawQuadrangleAA(p1.x + offset.x, p1.y,
			p2.x + offset.x, p2.y,
			p3.x + offset.x, p3.y,
			p4.x + offset.x, p4.y,
			color, false, 2);

		DrawCircleAA(epos.x + offset.x, epos.y, cap.radius, color, 32,false, 2);
	}
#endif
}
#include "Background.h"
#include<DxLib.h>
#include<sstream>
#include<iomanip>
#include"../System/Application.h"
#include"../Geometry.h"
#include"Camera.h"

namespace {
	constexpr float scroll_rate = 0.1f;
}

using namespace std;


Background::Background(std::shared_ptr<Camera> c):camera_(c){ 
	bgH_.resize(11);
	int skipno[2] = { 4,7 };
	int no = 9;
	for (int i = 0; i < bgH_.size(); ++i) {
		wstringstream wss;
		wss << L"Resource/Image/Background/Layer_";
		wss << setw(4) << setfill(L'0') << i << "_";
		if (std::count(begin(skipno), end(skipno), i) > 0) {
			wss << "Lights";
		}
		else {
			wss << no;
			--no;
		}
		wss << ".png";
		bgH_[bgH_.size() - 1 - i] = LoadGraph(wss.str().c_str());
	}
}
Background::~Background() {
	for (auto& bg : bgH_) {
		DxLib::DeleteGraph(bg);
	}
}

void 
Background::Update() {
	++frame_;
}
void 
Background::Draw() {
	auto rc = Application::Instance().GetViewport().GetRect();
	auto xoffset = camera_->ViewOffset().x;
	int wdh;
	int hgt;
	GetGraphSize(bgH_[0], &wdh, &hgt);
	const float scale = (float)rc.Width() / wdh;
	auto w = (float)rc.Width() / scale;

	auto scroll = -xoffset * scroll_rate;

	for (int i = 0; i < bgH_.size(); ++i) {
		auto lscroll = (int)(scroll * (float)(i + 1)) % wdh;
		DrawRectExtendGraph(
			rc.Left(), rc.Top(), rc.Right(), rc.Bottom(),
			lscroll,
			0,
			static_cast<int>(w),
			hgt,
			bgH_[i], true);
		if ((wdh - lscroll) < (float)rc.Width() / scale) {
			int x = static_cast<int>(-scale * (lscroll - wdh));
			DrawRectExtendGraph(
				rc.Left() + x, rc.Top(), (int)(rc.Left() + x + wdh * scale), rc.Bottom(),
				0,
				0,
				wdh,
				hgt,
				bgH_[i], true);
		}

	}
}
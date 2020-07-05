#include "Background.h"
#include<DxLib.h>
#include<sstream>
#include<iomanip>
#include"../Application.h"
#include"../Geometry.h"

using namespace std;

Background::Background() {
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

	auto wdh = 1440.0f;
	auto hgt = 480.0f;
	{
		int w, h;
		GetGraphSize(bgH_[0], &w, &h);
		wdh = w;
		hgt = h;
	}
	const float scale = (float)rc.Width() / wdh;
	auto w = (float)rc.Width() / scale;

	auto scroll = static_cast<float>(frame_) / 2.0f;

	for (int i = 0; i < bgH_.size(); ++i) {
		auto lscroll = (int)(scroll * (float)(i + 1)) % (int)wdh;
		DrawRectExtendGraph(
			rc.Left(), rc.Top(), rc.Right(), rc.Bottom(),
			lscroll,
			0,
			w,
			hgt,
			bgH_[i], true);
		if ((wdh - lscroll) < (float)rc.Width() / scale) {
			auto x = -scale * ((float)lscroll - wdh);
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
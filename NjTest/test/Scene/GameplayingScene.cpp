#include "GameplayingScene.h"
#include"SceneController.h"
#include"ContinueScene.h"
#include"PauseScene.h"
#include<DxLib.h>
#include<algorithm>
#include<sstream>
#include<iomanip>
#include<cmath>
#include"../Input.h"
#include"../Application.h"
#include"../Geometry.h"

namespace {
	constexpr uint32_t fadeout_interval = 45;
	unsigned int waitTimer_ = 0;

	int bgH_[11] = {};
	int runH_[6] = {};
	int idx_ = 0;
	int frame_ = 0;
}
using namespace std;
GameplayingScene::GameplayingScene(SceneController& c):
	Scene(c),
	updater_(&GameplayingScene::FadeinUpdate),
	drawer_(&GameplayingScene::FadeDraw) {
	
	int skipno[2] = { 4,7 };
	int no = 9;
	for (int i = 0; i < _countof(bgH_); ++i) {
		wstringstream wss;
		wss<< L"Resource/Image/Background/Layer_";
		wss << setw(4) << setfill(L'0') << i << "_";
		if (std::count(begin(skipno), end(skipno), i)>0) {
			wss << "Lights";
		}
		else {
			wss << no;
			--no;
		}
		wss << ".png";
		bgH_[_countof(bgH_)-1-i] = LoadGraph(wss.str().c_str());
	}
	for (int i = 0; i < _countof(runH_); ++i) {
		wstringstream wss;
		wss << L"Resource/Image/Player/";
		wss << L"adventurer-run-";
		wss << setfill(L'0') << setw(2);
		wss << i << L".png";
		runH_[i] = LoadGraph(wss.str().c_str());
	}
	waitTimer_ = 0;
	idx_ = 0;
	frame_ = 0;
}


void 
GameplayingScene::Update(const Input& input) {
	++frame_;
	if (frame_ % 5 == 0) {
		idx_ = (idx_ + 1) % _countof(runH_);
	}
	(this->*updater_)(input);
}

void
GameplayingScene::Draw() {
	(this->*drawer_)();	
}


//待ち
void
GameplayingScene::NormalUpdate(const Input& input) {
	if (input.IsTriggered("OK")) {
		updater_ = &GameplayingScene::FadeoutUpdate;
		drawer_ = &GameplayingScene::FadeDraw;
		waitTimer_ = fadeout_interval;
	}
	if (input.IsTriggered("pause")) {
		controller_.PushScene(new PauseScene(controller_));
	}
}

void
GameplayingScene::FadeinUpdate(const Input&) {
	if (++waitTimer_ == fadeout_interval) {
		updater_ = &GameplayingScene::NormalUpdate;
		drawer_ = &GameplayingScene::NormalDraw;
	}
}

//フェードアウト
void
GameplayingScene::FadeoutUpdate(const Input&) {
	
	if (--waitTimer_ == 0) {
		controller_.ChangeScene(new ContinueScene(controller_));
	}
}


void
GameplayingScene::NormalDraw() {
	auto rc=Application::Instance().GetViewport().GetRect();
	//for (auto handle : bgH_) {
	//	int w, h;
	//	GetGraphSize(handle, &w, &h);

	//	int top = rc.GetRect().Bottom() - h;
	//	int bottom = rc.GetRect().Bottom();
	//	DrawExtendGraph(0, top, 800, bottom, handle, true);
	//}
	auto wdh = 1440.0f;
	auto hgt = 480.0f;
	{
		int w, h;
		GetGraphSize(bgH_[0], &w, &h);
		wdh = w;
		hgt = h;
	}
	const float scale = (float)rc.Height()/ hgt;
	auto w = (float)rc.Width() / scale;

	auto scroll = static_cast<float>(frame_) / 2.0f;

	for (int i = 0; i < _countof(bgH_); ++i) {
		auto lscroll = (int)(scroll * (float)(i + 1)) % (int)wdh;
		DrawRectExtendGraph(
			rc.Left(), rc.Top(), rc.Right(), rc.Bottom(),
			lscroll,
			0,
			w,
			hgt,
			bgH_[i], true);
		//if ((wdh - lscroll) < (float)rc.Width() / scale) {
			auto x = -scale * ((float)lscroll - wdh);
			DrawRectExtendGraph(
				rc.Left() + x, rc.Top(), (int)(rc.Left() + x + wdh * scale), rc.Bottom(),
				0,
				0,
				wdh,
				hgt,
				bgH_[i], true);
		//}

	}
	

	DrawRotaGraph(400, 480, 4.0f, 0.0f, runH_[idx_], true, false, false);

}
void
GameplayingScene::FadeDraw() {
	NormalDraw();
	auto blendparam = 255 * (static_cast<float>(fadeout_interval - waitTimer_) / static_cast<float>(fadeout_interval));
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendparam);
	DxLib::DrawBox(0, 0, 800, 600, 0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

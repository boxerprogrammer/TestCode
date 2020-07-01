#include<DxLib.h>
#include "PauseScene.h"
#include"SceneController.h"
#include"../Input.h"
#include"../Application.h"
#include"../Geometry.h"
#include"TitleScene.h"


namespace {
	constexpr int  menu_multiply_param = 168;
	constexpr int default_x_offset = 96;
	constexpr int menu_y_start = 96;
	constexpr int menu_y_interval = 48;

	Rect rc = { {200,100}, {400,400} };
	constexpr int appear_time = 25;
	int frame_ = 0;
	int indicatorH_;
	int currentNo_ = 0;
	Size indicatorSize_;
}

using namespace std;

PauseScene::PauseScene(SceneController& c) :
	Scene(c),
	updater_(&PauseScene::AppearUpdate),
	drawer_(&PauseScene::AppearDraw){
	frame_ = 0;

	int y = menu_y_start;
	menuItems_.emplace_back(MenuItem( L"ƒQ[ƒ€‚É–ß‚é", 
		rc.pos + Position2(default_x_offset, y),
		[this]() {
			CloseMenu();
		}));
	y += menu_y_interval;
	menuItems_.emplace_back(MenuItem(L"ƒ^ƒCƒgƒ‹‚É–ß‚é",
		rc.pos + Position2(default_x_offset, y),
		[this]() {
			controller_.CleanChangeScene(new TitleScene(controller_));
		}));
	y += menu_y_interval;
	menuItems_.emplace_back(MenuItem(L"ƒQ[ƒ€‚ðI—¹‚·‚é",
		rc.pos + Position2(default_x_offset, y),
		[this]() {
			Application::Instance().Exit();
		}));
	indicatorH_= DxLib::LoadGraph(L"Resource/Image/UI/indicator.png");
	GetGraphSize(indicatorH_, &indicatorSize_.w, &indicatorSize_.h);
	frame_ = 0;
	currentNo_ = 0;
	
	
	
}
PauseScene::~PauseScene() {
	DeleteGraph(indicatorH_);
}


void
PauseScene::AppearDraw() {
	auto vh = rc.Height() / appear_time;
	Rect lrc = rc;
	auto centerY = rc.Top() + rc.Height() / 2;
	lrc.pos.y = centerY - frame_ * vh / 2;
	lrc.size.h = vh * frame_;
	//˜g‚Ì•`‰æ
	SetDrawBlendMode(DX_BLENDMODE_MULA, menu_multiply_param);
	DrawBox(lrc.Left(), lrc.Top(), lrc.Right(), lrc.Bottom(), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(lrc.Left(), lrc.Top(), lrc.Right(), lrc.Bottom(), 0xffffff, false);
}

void
PauseScene::AppearUpdate(const Input&) {
	if (++frame_ > appear_time) {
		updater_ = &PauseScene::NormalUpdate;
		drawer_ = &PauseScene::NormalDraw;
	}
}

void 
PauseScene::CloseMenu() {
	updater_ = &PauseScene::DisppearUpdate;
	drawer_ = &PauseScene::AppearDraw;
	frame_ = appear_time;
}

void 
PauseScene::NormalUpdate(const Input& input) {
	if (input.IsTriggered("pause")) {
		CloseMenu();
	}
	if (input.IsTriggered("down")) {
		currentNo_ = (currentNo_ + 1) % menuItems_.size();
	}
	if (input.IsTriggered("up")) {
		currentNo_ = (currentNo_ - 1+menuItems_.size()) % menuItems_.size();
	}

	for (auto& m : menuItems_) {
		m.isActive = false;
	}
	menuItems_[currentNo_].isActive = true;

	if (input.IsTriggered("OK")) {
		menuItems_[currentNo_].func();
	}
	
}

void
PauseScene::DisppearUpdate(const Input&) {
	if (--frame_ ==0) {
		controller_.PopScene();
	}
}

void 
PauseScene::Update(const Input& input) {
	
	(this->*updater_)(input);
}


void
PauseScene::NormalDraw() {
	//˜g‚Ì•`‰æ
	SetDrawBlendMode(DX_BLENDMODE_MULA, menu_multiply_param);
	DrawBox(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0xffffff, false);
	const wchar_t* pause_title = L"Pause Menu";
	auto widOfStr=DxLib::GetDrawStringWidth(pause_title, wcslen(pause_title));
	DrawString(rc.Left()+(rc.Width()-widOfStr)/2,rc.Top()+	32, pause_title, 0xffffff);

	auto& mpos = menuItems_[currentNo_].pos;
	
	for (auto& m : menuItems_) {
		int x = m.pos.x;
		uint32_t col = 0xffffff;
		if (m.isActive) {
			x += 16;
			col = 0xffaaaaa;
			DrawGraph(mpos.x - indicatorSize_.w, mpos.y, indicatorH_, true);
		}
		DrawString(x, m.pos.y, m.menuText.c_str(), col);
	}
}
void 
PauseScene::Draw() {
	//DrawString(300, 300, L"Pause", 0xffffff);
	(this->*drawer_)();
}
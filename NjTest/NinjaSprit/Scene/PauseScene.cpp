#include<DxLib.h>
#include "PauseScene.h"
#include "TitleScene.h"
#include"SceneController.h"
#include"../Input/Input.h"
#include"../System/Application.h"
#include"../Geometry.h"


namespace {
	Rect rc = { {200,100}, {400,400} };
	constexpr int appear_time = 25;
	int frame_ = 0;
	int indicatorH_ = 0;
	int indicatorWidth_;
	constexpr int pausetitle_y = 32;//ポーズタイトルのY
	constexpr int menubase_x = 64;//メニューの左
	constexpr int menubase_y = 96;//メニューの一番上のY
	constexpr int menu_y_interval = 48;//メニューの間隔
	const wchar_t pause_title[] = L"Pause Menu";
	size_t titleW_;
	size_t currentSelectNo_ = 0;
}

using namespace std;

PauseScene::PauseScene(SceneController& c) :
	Scene(c),
	updater_(&PauseScene::AppearUpdate),
	drawer_(&PauseScene::AppearDraw){
	frame_ = 0;
	indicatorH_ = LoadGraph(L"Resource/Image/UI/indicator.png");
	titleW_ = static_cast<size_t>(DxLib::GetDrawStringWidth(pause_title, static_cast<int>(wcslen(pause_title))));
	//MenuItem(const wchar_t* str, const Position2& p,  std::function<void(void)>& f)
	int y = menubase_y;
	menuItems_.emplace_back(L"ゲームに戻る",
		Position2(menubase_x, y),
		[this]() {
			CloseMenu();
		});
	y += menu_y_interval;
	menuItems_.emplace_back(L"タイトルに戻る",
		Position2(menubase_x, y),
		[this]() {
			controller_.CleanChangeScene(new TitleScene(controller_));
		});
	y += menu_y_interval;
	menuItems_.emplace_back(L"ゲームを終了する",
		Position2(menubase_x, y),
		[]() {
			Application::Instance().Exit();
		});
	currentSelectNo_ = 0;
	DxLib::GetGraphSize(indicatorH_, &indicatorWidth_, nullptr);
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
	//枠の描画
	SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
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
PauseScene::NormalUpdate(const Input& input) {
	if (input.IsTriggered("pause")) {
		CloseMenu();
	}
	if (input.IsTriggered("down")) {
		currentSelectNo_ = (currentSelectNo_ + 1) % menuItems_.size();
	}
	if (input.IsTriggered("up")) {
		currentSelectNo_ = (
			currentSelectNo_+menuItems_.size() - 1
			) % menuItems_.size();
	}
	
	for (auto& m : menuItems_) {
		m.isActive = false;
	}
	auto& selectedItem = menuItems_[currentSelectNo_];
	selectedItem.isActive = true;
	if (input.IsTriggered("OK")) {
		selectedItem.func();
	}
}

void PauseScene::CloseMenu()
{
	updater_ = &PauseScene::DisppearUpdate;
	drawer_ = &PauseScene::AppearDraw;
	frame_ = appear_time;
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
	//枠の描画
	SetDrawBlendMode(DX_BLENDMODE_MULA, 128);
	DrawBox(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(rc.Left(), rc.Top(), rc.Right(), rc.Bottom(), 0xffffff, false);

	//ポーズのタイトル
	DrawString(rc.Left()+(rc.Width()-titleW_)/2,
		rc.Top()+pausetitle_y,
		 pause_title,0xffffff);

	//メニューの表示
	for (auto& m : menuItems_) {
		uint32_t col = 0xffffff;
		int offset_x = 0;
		if (m.isActive) {
			col = 0xffaaaa;
			offset_x = 32;
		}

		DrawString(
			offset_x+rc.Left() + m.pos.x,
			rc.Top() + m.pos.y,
			m.menuText.c_str(),
			col);
	}
	//インジケータ(差し指)の表示
	auto& indPos=menuItems_[currentSelectNo_].pos;
	DrawGraph(rc.Left()+indPos.x - indicatorWidth_,
			rc.Top()+indPos.y, 
			indicatorH_, true);
}
void 
PauseScene::Draw() {
	//DrawString(300, 300, L"Pause", 0xffffff);
	(this->*drawer_)();
}
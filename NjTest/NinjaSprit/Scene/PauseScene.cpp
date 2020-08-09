#include<DxLib.h>
#include "PauseScene.h"
#include "TitleScene.h"
#include"KeyconfigScene.h"
#include"SceneController.h"
#include"../Input/Input.h"
#include"../System/Application.h"
#include"../Geometry.h"
#include"../System/File.h"
#include"../System/FileManager.h"
#include"../Arithmetic.h"


namespace {
	Rect rc = { {200,100}, {400,400} };
	constexpr int appear_time = 25;
	int frame_ = 0;
	
	int indicatorWidth_;
	constexpr int keyconfig_title_y = 32;//ポーズタイトルのY
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
	auto& fileMgr = FileManager::Instance();
	indicatorH_ = fileMgr.Load(L"Resource/Image/UI/indicator.png")->Handle();
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
	menuItems_.emplace_back(L"キーコンフィグ",
		Position2(menubase_x, y),
		[this]() {
			controller_.PushScene(new KeyconfigScene(controller_, { 20,20 }));
		});
	y += menu_y_interval;
	menuItems_.emplace_back(L"ゲームを終了する",
		Position2(menubase_x, y),
		[]() {
			Application::Instance().Exit();
		});
	currentSelectNo_ = 0;
	DxLib::GetGraphSize(indicatorH_, &indicatorWidth_, nullptr);
	openSE_ = fileMgr.Load(L"Resource/Sound/System/menuopen.wav")->Handle();
	closeSE_ = fileMgr.Load(L"Resource/Sound/System/menuclose.wav")->Handle();
	moveSE_ = fileMgr.Load(L"Resource/Sound/System/cursor.wav")->Handle();
	decideSE_ = fileMgr.Load(L"Resource/Sound/System/decide.wav")->Handle();
	cancelSE_ = fileMgr.Load(L"Resource/Sound/System/cancel.wav")->Handle();
	PlaySoundMem(openSE_, DX_PLAYTYPE_BACK);
}
PauseScene::~PauseScene() {
}


void
PauseScene::AppearDraw() {
	auto vh = rc.Height() / appear_time;
	Rect lrc = rc;
	auto centerY = rc.Center().y;
	lrc.pos.y = centerY - frame_ * static_cast<int>(vh / 2);
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
	if (input.IsTriggered("pause")||input.IsTriggered("cancel")) {
		CloseMenu();
	}
	if (input.IsTriggered("down")) {
		PlaySoundMem(moveSE_, DX_PLAYTYPE_BACK);
		currentSelectNo_ = ModuloIncrement(currentSelectNo_ , menuItems_.size());
	}
	if (input.IsTriggered("up")) {
		PlaySoundMem(moveSE_, DX_PLAYTYPE_BACK);
		currentSelectNo_ = ModuloDecrement(currentSelectNo_, menuItems_.size());
	}
	
	for (auto& m : menuItems_) {
		m.isActive = false;
	}
	auto& selectedItem = menuItems_[currentSelectNo_];
	selectedItem.isActive = true;
	if (input.IsTriggered("OK")) {
		PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK);
		selectedItem.func();
	}
}

void PauseScene::CloseMenu()
{
	updater_ = &PauseScene::DisppearUpdate;
	drawer_ = &PauseScene::AppearDraw;
	frame_ = appear_time;
	PlaySoundMem(closeSE_, DX_PLAYTYPE_BACK);
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
	DrawString(rc.Left()+static_cast<int>((rc.Width()-titleW_)/2),
		rc.Top()+keyconfig_title_y,
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
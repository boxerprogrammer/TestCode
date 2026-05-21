#include "PauseScene.h"
#include<DxLib.h>
#include"SceneController.h"
#include"TitleScene.h"
#include"../Input.h"
#include"../Application.h"
#include "KeyconfigScene.h"


constexpr int margin_size = 50;//端っこからの余白
constexpr int expand_interval = 30;//30フレームで表示

void PauseScene::AppearUpdate(Input& input)
{
	if (++frame_ >= expand_interval) {
		update_ = &PauseScene::NormalUpdate;
		draw_ = &PauseScene::NormalDraw;
		return;
	}
}



void PauseScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("pause")) {
		update_ = &PauseScene::DisappearUpdate;
		draw_ = &PauseScene::ExpandDraw;
		return;
	}
	if (input.IsTriggered("up")) {
		selectedIndex_=(selectedIndex_+ menuItems_.size() -1)% 
							menuItems_.size();
	}
	if (input.IsTriggered("down")) {
		selectedIndex_= (selectedIndex_+1)%menuItems_.size();
	}

	if (input.IsTriggered("ok")) {
		auto menuString = menuItems_[selectedIndex_];
		commandTable_[menuString](input);
	}

}


void PauseScene::DisappearUpdate(Input& input)
{
	if (--frame_ <= 0) {
		controller_.PopScene();
		return;
	}
}

void PauseScene::YesNoUpdate(Input& input)
{
	if (input.IsTriggered("left") || input.IsTriggered("right")) {
		isYes_ = !isYes_;
	}
	if (input.IsTriggered("ok")) {
		if (isYes_) {
			execYesFunc_(input);
			return;
		}
		else {
			update_ = &PauseScene::NormalUpdate;
			draw_ = &PauseScene::NormalDraw;
		}
	}
}

void PauseScene::YesNoDraw()
{
	NormalDraw();
	const auto& wsize = Application::GetInstance().GetWindowSize();
	auto centerX = wsize.w / 2;
	auto centerY = wsize.h / 2;
	constexpr int yesno_frame_height = 150;
	constexpr int yesno_frame_width = 300;
	int top = centerY - yesno_frame_height / 2;
	int bottom = centerY + yesno_frame_height / 2;
	int left = centerX - yesno_frame_width / 2;
	int right = centerX + yesno_frame_width / 2;

	DrawBox(left, top, right, bottom, 0x000000, true);
	DrawBoxAA(left, top, right, bottom, 0xffffff, false,3.0f);

	int x = left + 50;
	int y = top + 20;
	DrawFormatString(x, y, 0xffffff, L"%s", yesNoTitle_.c_str());
	y += 40;
	int idx = 0;
	if (!isYes_) {
		idx = 1;
	}
	for (const auto& item : yesNoItems_) {
		uint32_t itemColor = 0xffffff;
		if (item == yesNoItems_[idx]) {
			DrawString(x - 30, y, L"⇒", 0xff0000);
			itemColor = 0xff00ff;
		}
		DrawFormatString(x, y, itemColor, L"%s", item.c_str());
		x += 100;
	}
}

void PauseScene::ExpandDraw()
{
	DrawFrame(static_cast<float>(frame_) / static_cast<float>(expand_interval));
}

void PauseScene::NormalDraw()
{	
	DrawFrame(1.0f);
	DrawMenu();
}

void PauseScene::DrawFrame(float rate)
{
	const auto& wsize = Application::GetInstance().GetWindowSize();

	const auto centerY = wsize.h / 2;//画面の真ん中のY座標
	auto height = (wsize.h - margin_size) / 2;//広がり切った時の高さ/2(真ん中から見た高さ)

	//元の画面の彩度を落とすためのセロファン
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 168);
	DrawBoxAA(
		margin_size, centerY- height *rate,
		wsize.w - margin_size, centerY + height * rate,
		0x000000,
		true, 1.0f
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//枠の描画(白枠)
	DrawBoxAA(margin_size, centerY - height * rate,
		wsize.w - margin_size, centerY + height * rate,
		0xffffff,
		false,
		3.0f);
}

void PauseScene::DrawMenu()
{
	//メニューの先頭Y座標
	constexpr int menu_top_offset = margin_size + 100;
	constexpr int menu_left_offset = margin_size + 300;
	constexpr int menu_item_height = 40;
	constexpr uint32_t menu_item_color = 0xffffffff;
	constexpr uint32_t indicator_color = 0xffffaaaa;
	constexpr int menu_indent_size = 10;
	constexpr uint32_t selected_item_color = 0xffffaaff;
	int y = menu_top_offset;
	

	for (const auto& item : menuItems_) {
		int x = menu_left_offset;
		auto itemColor = menu_item_color;
		if (menuItems_[selectedIndex_] == item) {
			DrawString(x - 30, y, L"⇒", indicator_color);
			x += menu_indent_size;
			itemColor = selected_item_color;
		}
		DrawFormatString(x, y, itemColor, L"%s",
			item.c_str());
		y += menu_item_height;
	}

}

PauseScene::PauseScene(SceneController& ctrl):Scene(ctrl)
{
	frame_ = 0;
	update_ = &PauseScene::AppearUpdate;
	draw_ = &PauseScene::ExpandDraw;

	yesNoItems_ = { L"はい",L"いいえ" };

	menuItems_ = {
		L"ゲームに戻る",
		L"キーコンフィグ",
		L"設定",
		L"あそびかた",
		L"タイトルに戻る",
		L"ゲームを終了する"
	};
	commandTable_[L"ゲームに戻る"] = [this](Input& input) {
		update_ = &PauseScene::DisappearUpdate;
		draw_ = &PauseScene::ExpandDraw;
		};
	commandTable_[L"キーコンフィグ"] = [this](Input& input) {
		controller_.PushScene(std::make_shared<KeyconfigScene>(controller_, input));
		};
	commandTable_[L"設定"] = [this](Input& input) {
		DrawString(600, 360, L"設定選択", 0xffffff);
		};
	commandTable_[L"あそびかた"] = [this](Input& input) {
		DrawString(600, 360, L"あそびかた選択", 0xffffff);
		};
	commandTable_[L"タイトルに戻る"] = [this](Input& input) {
		execYesFunc_ = [this](Input& input) {
				controller_.ResetScene(std::make_shared<TitleScene>(controller_));
			};
		yesNoTitle_ = L"タイトルに戻る";
		update_ = &PauseScene::YesNoUpdate;
		draw_ = &PauseScene::YesNoDraw;
		};
	commandTable_[L"ゲームを終了する"] = [this](Input& input) {
		execYesFunc_ = [this](Input& input) {
				Application::GetInstance().Shutdown();
			};
		yesNoTitle_ = L"ゲームを終了する";
		update_ = &PauseScene::YesNoUpdate;
		draw_ = &PauseScene::YesNoDraw;
		};
		

}

void PauseScene::Update(Input& input)
{
	(this->*update_)(input);
}

void PauseScene::Draw()
{

	(this->*draw_)();
	
}

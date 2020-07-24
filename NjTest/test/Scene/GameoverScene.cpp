#include "GameoverScene.h"
#include"SceneController.h"
#include"TitleScene.h"
#include<DxLib.h>
#include"../Input.h"
namespace {
	constexpr uint32_t fadeout_interval = 45;
	unsigned int waitTimer_ = 0;
}

GameoverScene::GameoverScene(SceneController& c) :
	Scene(c),
	updater_(&GameoverScene::FadeinUpdate),
	drawer_(&GameoverScene::FadeDraw) {
	unsigned int waitTimer_ = 0;
}


void
GameoverScene::Update(const Input& input) {
	(this->*updater_)(input);
}
void
GameoverScene::Draw() {
	(this->*drawer_)();
}


//待ち
void
GameoverScene::WaitUpdate(const Input& input) {
	if (input.IsTriggered("OK")) {
		updater_ = &GameoverScene::FadeoutUpdate;
		drawer_ = &GameoverScene::FadeDraw;
		waitTimer_ = fadeout_interval;
	}
}

void
GameoverScene::FadeinUpdate(const Input&) {
	if (++waitTimer_ == fadeout_interval) {
		updater_ = &GameoverScene::WaitUpdate;
		drawer_ = &GameoverScene::NormalDraw;
	}
}

//フェードアウト
void
GameoverScene::FadeoutUpdate(const Input&) {
	if (--waitTimer_ == 0) {
		controller_.ChangeScene(new TitleScene(controller_));
	}
}


void
GameoverScene::NormalDraw() {
	DrawString(100, 100, L"Game Over Scene", 0xffffff);
}
void
GameoverScene::FadeDraw() {
	NormalDraw();
	auto blendparam = static_cast<int>(255 * (static_cast<float>(fadeout_interval - waitTimer_) / static_cast<float>(fadeout_interval)));
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendparam);
	DxLib::DrawBox(0, 0, 800, 600, 0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

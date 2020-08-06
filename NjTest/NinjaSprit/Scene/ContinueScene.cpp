#include "ContinueScene.h"
#include"SceneController.h"
#include"GameoverScene.h"
#include<DxLib.h>
#include"../Input/Input.h"
namespace {
	constexpr uint32_t fadeout_interval = 45;
	unsigned int waitTimer_ = 0;
}

ContinueScene::ContinueScene(SceneController& c) :
	Scene(c),
	updater_(&ContinueScene::FadeinUpdate),
	drawer_(&ContinueScene::FadeDraw) {
	unsigned int waitTimer_ = 0;
}


void
ContinueScene::Update(const Input& input) {
	(this->*updater_)(input);
}
void
ContinueScene::Draw() {
	(this->*drawer_)();
}


//待ち
void
ContinueScene::WaitUpdate(const Input& input) {
	if (input.IsTriggered("OK")) {
		updater_ = &ContinueScene::FadeoutUpdate;
		drawer_ = &ContinueScene::FadeDraw;
		waitTimer_ = fadeout_interval;
	}
}

void
ContinueScene::FadeinUpdate(const Input&) {
	if (++waitTimer_ == fadeout_interval) {
		updater_ = &ContinueScene::WaitUpdate;
		drawer_ = &ContinueScene::NormalDraw;
	}
}

//フェードアウト
void
ContinueScene::FadeoutUpdate(const Input&) {
	if (--waitTimer_ == 0) {
		controller_.ChangeScene(new GameoverScene(controller_));
	}
}


void
ContinueScene::NormalDraw() {
	DrawString(100, 100, L"Continue Scene", 0xffffff);
}
void
ContinueScene::FadeDraw() {
	NormalDraw();
	auto blendparam = static_cast<int>(255 * (static_cast<float>(fadeout_interval - waitTimer_) / static_cast<float>(fadeout_interval)));
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendparam);
	DxLib::DrawBox(0, 0, 800, 600, 0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

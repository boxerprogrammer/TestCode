#include<DxLib.h>
#include "GameoverScene.h"
#include"TitleScene.h"
#include"SceneController.h"
#include"../Input.h"
#include"../ResourceManager.h"
#include"../Application.h"

constexpr int fade_interval = 90;

GameoverScene::GameoverScene(SceneController& ctrl):Scene(ctrl),
update_(&GameoverScene::FadeInUpdate),
draw_(&GameoverScene::FadeDraw),
frame_(fade_interval)
{
	backH_ = ResourceManager::LoadGraph(L"Resource/img/gameover.jpg");
	gameoverStringH_ = ResourceManager::LoadGraph(L"Resource/img/gameover_string.png");
}


void GameoverScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameoverScene::Draw()
{
	(this->*draw_)();
}



//フェードイン
void GameoverScene::FadeInUpdate(Input& input) {
	if (--frame_ <= 0) {
		update_ = &GameoverScene::NormalUpdate;
		draw_ = &GameoverScene::NormalDraw;
	}
}
//フェードアウト
void GameoverScene::FadeOutUpdate(Input& input) {
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}
}
//通常更新
void GameoverScene::NormalUpdate(Input& input) {
	if (input.IsTriggered("ok")) {
		update_ = &GameoverScene::FadeOutUpdate;
		draw_ = &GameoverScene::FadeDraw;
	}
}

//フェード描画
void GameoverScene::FadeDraw() {
	NormalDraw();

	//黒いセロファンをアルファブレンディングしている
	auto size = Application::GetInstance().GetWindowSize();
	//0～60→0.0～1.0に変換している
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	//0.0～1.0*255.0→0.0～255.0に変換される
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * rate));
	//黒セロファン
	DrawBox(0, 0, size.w, size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//通常描画
void GameoverScene::NormalDraw() {
	DrawGraph(100, 100, backH_, true);
	DrawGraph(100, 100, gameoverStringH_, true);
	DrawString(100, 100, L"GameScene", 0xffffff);
}


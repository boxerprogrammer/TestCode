#include<DxLib.h>
#include "TitleScene.h"
#include"GameScene.h"
#include"SceneController.h"
#include"../Input.h"
#include"../ResourceManager.h"
#include"../Application.h"


constexpr int fade_interval = 60;

void TitleScene::FadeInUpdate(Input& input)
{
	if (--frame_ <= 0) {
		//一定時間が経ったら通常更新通常描画に切り替え
		update_ = &TitleScene::NormalUpdate;
		draw_ = &TitleScene::NormalDraw;
	}
}

void TitleScene::FadeOutUpdate(Input& input)
{
	if (++frame_ >= fade_interval) {
		controller_.ChangeScene(std::make_shared<GameScene>(controller_));
		return;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok")) {
		update_ = &TitleScene::FadeOutUpdate;
		draw_ = &TitleScene::FadeDraw;
	}
}

void TitleScene::NormalDraw()
{
	DrawGraph(100, 100, backH_, false);
	DrawGraph(100, 100, charH_, true);
	DrawString(100, 100, L"TitleScene", 0xffffff);
}

void TitleScene::FadeDraw()
{
	NormalDraw();
	//黒いセロファンをアルファブレンディングしている
	auto size=Application::GetInstance().GetWindowSize();
	//0～60→0.0～1.0に変換している
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	//0.0～1.0*255.0→0.0～255.0に変換される
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f*rate));
	//黒セロファン
	DrawBox(0, 0, size.w, size.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

TitleScene::TitleScene(SceneController& ctrl):Scene(ctrl),
update_(&TitleScene::FadeInUpdate),
draw_(&TitleScene::FadeDraw),
frame_(fade_interval)
{
	charH_ = ResourceManager::LoadGraph(L"Resource/img/girl.png");
	backH_ = ResourceManager::LoadGraph(L"Resource/img/back.jpg");
}

void TitleScene::Update(Input& input)
{
	//中身が何なのかわからないが、とにかく実行
	(this->*update_)(input);
}

void TitleScene::Draw()
{
	//中身が何なのかわからないがとにかく実行
	(this->*draw_)();
}

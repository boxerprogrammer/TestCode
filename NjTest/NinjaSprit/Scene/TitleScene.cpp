#include "TitleScene.h"
#include"GameplayingScene.h"
#include"SceneController.h"
#include"../System/Application.h"
#include<DxLib.h>
#include"../Input/Input.h"
#include"../Geometry.h"
#include"../System/File.h"
#include"../System/FileManager.h"

using namespace std;

namespace {
	constexpr uint32_t fadeout_interval = 45;
	constexpr uint32_t blink_interval_normal= 45;
	constexpr uint32_t blink_interval_fast = 5;
	unsigned int waitTimer_ = 0;
	unsigned int blinkInterval_ = blink_interval_normal;
	unsigned int blinkTimer_ = 0;

}

TitleScene::TitleScene(SceneController& c) :
	Scene(c),
	updater_(&TitleScene::FadeinUpdate),
	drawer_(&TitleScene::FadeDraw){
	blinkInterval_ =blink_interval_normal;
	auto& fileMgr = FileManager::Instance();
	titleH_=fileMgr.Load(L"Resource/Image/Title/title.png")->Handle();
	startH_=fileMgr.Load(L"Resource/Image/Title/pressstart.png")->Handle();
	startSE_ = fileMgr.Load(L"Resource/Sound/System/start.wav")->Handle();
}

//待ち
void 
TitleScene::WaitUpdate(const Input& input) {
	if (input.IsTriggered("OK")) {
		updater_ = &TitleScene::BlinkUpdate;
		blinkInterval_ = blink_interval_fast;
		auto ret=PlaySoundMem(startSE_, DX_PLAYTYPE_BACK);
		waitTimer_ = 30;
	}
}

//高速点滅
void 
TitleScene::BlinkUpdate(const Input&) {
	if (--waitTimer_ == 0) {
		updater_ = &TitleScene::FadeoutUpdate;
		waitTimer_ = fadeout_interval;//フェードアウトまでの時間
		drawer_ = &TitleScene::FadeDraw;
	}
}

void 
TitleScene::FadeinUpdate(const Input&) {
	if (++waitTimer_ == fadeout_interval) {
		updater_ = &TitleScene::WaitUpdate;
		drawer_ = &TitleScene::NormalDraw;
	}
}

//フェードアウト
void 
TitleScene::FadeoutUpdate(const Input&) {
	if (--waitTimer_ == 0) {
		controller_.ChangeScene(new GameplayingScene(controller_));
	}
}

TitleScene::~TitleScene() {
	DeleteGraph(titleH_);
	DeleteGraph(startH_);
}
void 
TitleScene::Update(const Input& input) {
	blinkTimer_ = (blinkTimer_ + 1) % 600;
	(this->*updater_)(input);
}
void
TitleScene::NormalDraw() {
	Position2 pos;
	int w,h;
	GetGraphSize(titleH_,&w, &h);
	Size isize(w, h);
	const auto& vsize=Application::Instance().GetViewport().GetSize();
	pos.x = static_cast<int>((vsize.w - isize.w) / 2);
	pos.y = static_cast<int>((vsize.h - isize.h) / 2);
	DrawExtendGraph(0,0,
		static_cast<int>(vsize.w),
		static_cast<int>(vsize.h),
		titleH_, false);
	
	if ((blinkTimer_ / blinkInterval_) % 2 == 1) {
		DrawRotaGraph(static_cast<int>(vsize.w / 2),400,1.0f,0.0f,startH_,true);
	}
}
void
TitleScene::FadeDraw() {
	const auto& vpSize=Application::Instance().GetViewport().GetSize().ToIntVector();
	NormalDraw();
	auto blendparam = static_cast<int>(255 * (static_cast<float>(fadeout_interval - waitTimer_) / static_cast<float>(fadeout_interval)));
	DxLib::SetDrawBlendMode(DX_BLENDMODE_MULA, blendparam);
	DxLib::DrawBox(0, 0, vpSize.x, vpSize.y, 0x000000, true);
	DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void
TitleScene::Draw() {
	(this->*drawer_)();
}
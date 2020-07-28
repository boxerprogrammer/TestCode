#include "Application.h"
#include<DxLib.h>
#include"Scene/SceneController.h"
#include"Scene/Scene.h"
#include"Input.h"
#include"Geometry.h"
#include"Debugger.h"

using namespace std;

namespace {
	Rect rcView_ = { {0,0},{800,600} };
	bool isExit_ = false;
}

Size
Viewport::GetSize()const {
	return rcView_.size;
}

Rect
Viewport::GetRect()const {
	return rcView_;
}

Application& 
Application::Instance() {
	static Application instance;
	return instance;
}

bool 
Application::Initialize() {

	//sceneController_ = make_unique<SceneController>();
	DxLib::SetWindowText(L"画面遷移テスト");
	DxLib::ChangeWindowMode(true);
	const auto& wsize = viewport_.GetSize();
	DxLib::SetGraphMode(static_cast<int>(wsize.w), static_cast<int>(wsize.h), 32);
	if (DxLib_Init()) {
		return false;
	}
	sceneController_.reset(new SceneController());
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	return true;
}

void
Application::Run() {
	Input input;
	auto& dinstance = Debugger::Instance();
	while (DxLib::ProcessMessage() == 0) {
		ClearDrawScreen();
		dinstance.Update();
		input.Update();
		sceneController_->Update(input);
		sceneController_->Draw();
		dinstance.DisplayPerformance();
		ScreenFlip();
		if (isExit_)break;
	}
}

void
Application::Exit() {
	isExit_ = true;
}

void 
Application::Terminate() {
	DxLib_End();
}

const Viewport& 
Application::GetViewport()const {
	return viewport_;
}

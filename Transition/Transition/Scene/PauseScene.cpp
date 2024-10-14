#include "PauseScene.h"
#include"../Input.h"
#include"SceneManager.h"
#include"KeyconfigScene.h"
#include<DxLib.h>

constexpr int change_interval = 30;
constexpr int pause_width = 400;
constexpr int pause_height = 300;

void PauseScene::BeginUpdate(Input& input)
{

	if (++frame_ >= change_interval) {
		updateFunc_ = &PauseScene::NormalUpdate;
		drawFunc_ = &PauseScene::NormalDraw;
		commandList_.push_back([]() {
			DrawString(300, 300, L"ChangeState to Normal",0xffffff);
			});
	}
}

void PauseScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("pause")) {
		updateFunc_ = &PauseScene::EndUpdate;
		drawFunc_ = &PauseScene::EndDraw;
		frame_ = 0;
		commandList_.push_back([]() {
			DrawString(300, 320, L"ChangeState to End",0xffffff);
			});
	}
	else if(input.IsTriggered("keyconfig")) {
		sceneManager_.PushScene(std::make_shared<KeyconfigScene>(sceneManager_));
	}
}

void PauseScene::EndUpdate(Input& input)
{
	if (++frame_ >= change_interval) {
		sceneManager_.PopScene();
	}
}

void PauseScene::BeginDraw()
{
	float rate = static_cast<float>(frame_)/ static_cast<float>(change_interval);
	DrawBox(90, 90, 90 + pause_width, 90 + pause_height*rate,
		0xffffff,false);
}

void PauseScene::NormalDraw()
{
	DrawBox(90, 90, 90 + pause_width, 90 + pause_height,
		0xffffff, false);
	DrawString(100, 100, L"Pause Scene", 0xffffff);
}

void PauseScene::EndDraw()
{
	float rate = static_cast<float>(frame_) / static_cast<float>(change_interval);
	DrawBox(90, 90, 90 + pause_width, 90 + pause_height * (1.0f-rate),
		0xffffff, false);
	DrawString(100, 100, L"P Scene", 0xffffff);
	for (auto& cmd : commandList_) {
		cmd();
	}
}

PauseScene::PauseScene(SceneManager& manager):Scene(manager)
{
	updateFunc_ = &PauseScene::BeginUpdate;
	drawFunc_ = &PauseScene::BeginDraw;
}

void PauseScene::Update(Input& input)
{
	//Œ»Ý‚ÌXVŠÖ”‚ðŒÄ‚Ño‚µ‚Ü‚·
	(this->*updateFunc_)(input);
	//‚±‚±‚æ‚è‰º‚Éˆ—‚ð‘‚©‚È‚¢‚Å‚­‚¾‚³‚¢
}

void PauseScene::Draw()
{
	(this->*drawFunc_)();
}

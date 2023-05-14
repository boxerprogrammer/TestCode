#include "OptionScene.h"
#include"../InputState.h"
#include"SceneManager.h"
#include"TitleScene.h"
#include<DxLib.h>
void 
OptionScene::Update(const InputState& input) {
	if (input.IsTriggered(InputType::prev)) {
		manager_.ChangeScene(new TitleScene(manager_));
	}
}
void 
OptionScene::Draw() {
	DrawString(300, 200, L"Option Scene", 0xffffff);
}
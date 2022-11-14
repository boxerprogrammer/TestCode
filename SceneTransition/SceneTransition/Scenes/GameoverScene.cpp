#include "GameoverScene.h"
#include<DxLib.h>
#include"SceneManager.h"
#include"TitleScene.h"

void 
GameoverScene::Update(const InputState& input) {
	if (IsTriggered(input, KEY_INPUT_RETURN)) {
		manager_.ChangeScene(new TitleScene(manager_));
	}
}
 void 
GameoverScene::Draw() {
	DrawString(200, 300, L"ゲームオーバー", 0xffffff);
}
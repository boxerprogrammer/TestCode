#include "TitleScene.h"
#include<DxLib.h>
#include"GamePlayingScene.h"
#include"SceneManager.h"

using namespace std;

void 
TitleScene::Update(const InputState& input) {
	if (IsTriggered(input, KEY_INPUT_RETURN)) {
		manager_.ChangeScene(make_unique<GamePlayingScene>(manager_));
	}
}
void 
TitleScene::Draw() {
	DrawString(200, 300, L"タイトルシーン", 0xffffff);
}
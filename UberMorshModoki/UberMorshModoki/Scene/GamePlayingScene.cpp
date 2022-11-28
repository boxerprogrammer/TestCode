#include "GamePlayingScene.h"
#include<DxLib.h>
#include"SceneManager.h"
#include"GameoverScene.h"

using namespace std;

void 
GamePlayingScene::Update(const InputState& input) {
	if (IsTriggered(input, KEY_INPUT_RETURN)) {
		manager_.ChangeScene(make_unique<GameoverScene>(manager_));
	}
}
void 
GamePlayingScene::Draw() {
	DrawString(200, 300, L"ÉQÅ[ÉÄíÜ", 0xffffff);
}
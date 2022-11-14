#include<DxLib.h>
#include<iostream>
#include<vector>
#include"scenes/Scene.h"
#include"input/InputState.h"
#include"scenes/SceneManager.h"
#include"scenes/TitleScene.h"

using namespace std;






//class StageClearScene : public Scene {
//public:
//	virtual void Update(const InputState& input)override {
//	}
//	virtual void Draw()override {
//		DrawString(200, 300, L"ステージクリア", 0xffffff);
//	}
//};






int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(800, 600, 32);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SceneManager sceneManager;
	sceneManager.ChangeScene(new TitleScene(sceneManager));
	InputState input;
	while(ProcessMessage() != -1) {
		ClearDrawScreen();
		input.Update();
		sceneManager.Update(input);
		sceneManager.Draw();
		ScreenFlip();
	}
	DxLib_End();
}

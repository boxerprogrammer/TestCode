#include<DxLib.h>
#include<iostream>
#include<vector>
#include"scenes/Scene.h"
#include"input/InputState.h"
#include"scenes/SceneManager.h"
#include"scenes/TitleScene.h"

using namespace std;



int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR,_In_ int) {
	ChangeWindowMode(true);
	SetGraphMode(800, 600, 32);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SceneManager sceneManager;//シーン管理クラス
	sceneManager.ChangeScene(new TitleScene(sceneManager));//最初のシーンの作成＆マネージャに登録
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

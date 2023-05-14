#include<DxLib.h>
#include<cassert>
#include<array>
#include<string>
#include"Scene/SceneManager.h"
#include"Scene/TitleScene.h"
#include"InputState.h"
#include"DrawFunctions.h"
#include"SoundManager.h"
#include<iostream>
void Function(int count,int value) {
	DrawFormatString(50, 50, 0xffffff, _T("Count=%d"), count);
	DrawFormatString(50, 100, 0xffffff, _T("Value=%d"), value);
}

void RecursiveFunction(int count) {
	if (count < 10) {
		RecursiveFunction(count + 1);
	}
	DrawFormatString(50, 150, 0xffffff, _T("RecursiveCount=%d"), count);
}



//3Dの要素をまとめた構造体
class Vector3 {
public:
	float x;
	float y;
	float z;
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	bool isWindowMode = true;
	ChangeWindowMode(isWindowMode);

	SetWindowText(L"ごっついシューティング");
	
	//ゲーム中にウィンドウモードを切り替えてもグラフィックスハンドルを
	//リセットしない
	//SetChangeScreenModeGraphicsSystemResetFlag(false);
	
	//ChangeWindowModeとSetWindowTextは、例外的にDxLib_Init()の前に書いてますが
	//基本的にDxLibの関数はDxLib_Init()実行後に書いてください。
	if (DxLib_Init() == -1) {
		return -1;
	}

	auto& soundManager = SoundManager::GetInstance();

	//裏画面に描画してくださいという指示
	SetDrawScreen(DX_SCREEN_BACK);

	int count = 0;
	int value = 0;
	++value;
	int handle = 0;
	

	InputState input;
	SceneManager sceneManager;
	sceneManager.ChangeScene(new TitleScene(sceneManager));

	std::wstring str = L"テスト";
	str += L"だよ";
	bool isTriggerEnter = false;
	while (ProcessMessage() != -1) {
		ClearDrawScreen();

		if (DxLib::CheckHitKey(KEY_INPUT_LALT)) {
			if (DxLib::CheckHitKey(KEY_INPUT_RETURN)) {
				if (!isTriggerEnter) {
					isWindowMode = !isWindowMode;
					ChangeWindowMode(isWindowMode);
					SetDrawScreen(DX_SCREEN_BACK);//描画先を再定義
				}
				isTriggerEnter = true;
			}
			else {
				SetDrawScreen(DX_SCREEN_BACK);//描画先を再定義
				isTriggerEnter = false;
			}

		}

		//入力の更新
		input.Update();

		sceneManager.Update(input);
		sceneManager.Draw();

		auto fps=GetFPS();//Frame Per Second;
		auto drawcall = GetDrawCallCount();//描画命令数

		DrawFormatString(10, 10, 0xffffff, L"FPS=%2.2f",fps);
		DrawFormatString(10, 30, 0xffffff, L"DC=%d",drawcall);

		//前画面と裏画面を入れ替えて、同期を待っている
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
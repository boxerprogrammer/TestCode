#include<DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	SetWindowText("OneFingerMonkPunch");
	ChangeWindowMode(true);
	DxLib_Init();

	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		DxLib::GetMouseInput();
		ScreenFlip();
	}

	DxLib_End();
}
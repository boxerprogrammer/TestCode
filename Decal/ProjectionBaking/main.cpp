#include<DxLib.h>
#include<cassert>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	auto model=MV1LoadModel(L"model/sphere.mv1");
	assert(model > 0);
	auto img = LoadGraph(L"img/crismas.png");
	assert(img > 0);
	
	SetDrawScreen(DX_SCREEN_BACK);
	while (ProcessMessage() != -1) {
		MV1SetUseOrigShader(true);
		ClearDrawScreen();
		DxLib::SetCameraNearFar(1.0f, 500.0f);
		SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -200), VGet(0, 0, 0));
		MV1DrawModel(model);
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
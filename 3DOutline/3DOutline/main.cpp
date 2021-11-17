#include<DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	auto model=MV1LoadModel(L"model/test.mqoz");
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 200, -400), VGet(0, 0, 0));
	DxLib::MV1SetRotationXYZ(model, VGet(0.0f, 3.14f / 4.0f, 0.0f));
	DxLib::MV1SetUseOrigShader(true);
	auto vs = LoadVertexShader(L"threed_outline.vso");
	auto ps = LoadPixelShader(L"threed_outline.pso");
	SetUseVertexShader(vs);
	SetUsePixelShader(ps);
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		MV1DrawModel(model);
		ScreenFlip();
	}
	return 0;
}
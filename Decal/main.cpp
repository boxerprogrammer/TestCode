#include<DxLib.h>
#include"DrawHelper.h"
#include<cassert>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	int jinbeH=LoadGraph(L"img/jinbe.png");
	int brushH=LoadGraph(L"img/santa.png");
	int ps = LoadPixelShader(L"decal_ps.pso");
	assert(ps >= 0);
	SetDrawScreen(DX_SCREEN_BACK);
	auto rtBase = MakeScreen(640, 480, true);
	auto rt = MakeScreen(640, 480,true);

	struct POS {
		float x;
		float y;
		float angle;
		float w;
	};

	auto cbuff = CreateShaderConstantBuffer(sizeof(POS));
	POS* pos=static_cast<POS*>(GetBufferShaderConstantBuffer(cbuff));
	DxLib::UpdateShaderConstantBuffer(cbuff);
	SetDrawScreen(rtBase);
	DrawRotaGraph(320, 240, 0.5f, 0.0f, jinbeH, true);

	while (ProcessMessage() != -1) {
		SetDrawScreen(rtBase);
		DrawGraph(0, 0,  rt, true);
		SetDrawScreen(rt);
		int mx, my;
		int minput = GetMouseInput();
		if (minput & MOUSE_INPUT_LEFT) {
			GetMousePoint(&mx, &my);
			pos->x = (float)mx/640.0f;
			pos->y = (float)my/480.0f;
			UpdateShaderConstantBuffer(cbuff);
			SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 4);
			MyLib::DrawRotaGraph(mx, my, 1.0f, 0.0f, brushH,rtBase,-1,ps);
		}
		else if (minput & MOUSE_INPUT_RIGHT) {
			pos->angle += 0.1f;
		}

		SetDrawScreen(DX_SCREEN_BACK);

		ClearDrawScreen();
		DrawRotaGraph(320, 240, 0.5f, 0.0f, jinbeH, true);
		DrawGraph(0, 0, rt, true);

		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
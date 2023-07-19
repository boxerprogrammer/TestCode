#include"DrawHelper.h"
#include<DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	//DxLib::SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);
	auto idlesH = LoadSequentialGraph(L"img/Fighter/sprites/",L"adventurer-run-",6);
	auto cloudH = LoadGraph(L"img/cloud.png");
	auto dotH = LoadGraph(L"img/dot.png");
	auto pshandle = LoadPixelShader(L"SpriteEffectPS.pso");
	auto cbuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
	float* gangle = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	float angle = 0.0f;
	int frame = 0;
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		angle += 0.01f;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		
		gangle[0] = angle;
		UpdateShaderConstantBuffer(cbuffer);
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);

		SetUsePixelShader(pshandle);
		int w, h;
		int idx = (frame / 8) % 6;
		GetGraphSize(idlesH[idx], &w, &h);
		MyDrawRotaGraph(320,240,8.0f,0.0f,idlesH[idx],cloudH,dotH , pshandle);
		ScreenFlip();
		++frame;
	}
	return 0;
}
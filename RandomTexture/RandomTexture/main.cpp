#include"DrawHelper.h"
#include<DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetGraphMode(1280, 720, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	//DxLib::SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);
	auto cloudH = LoadGraph(L"img/cloud.png");
	auto psStdHandle = LoadPixelShader(L"StandardTexture.pso");
	auto psRndHandle = LoadPixelShader(L"RandomTexture.pso");
	auto cbuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
	float* gangle = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	float angle = 0.0f;
	int frame = 0;
	DxLib::SetTextureAddressMode(DX_TEXADDRESS_WRAP);
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		angle += 0.01f;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		
		gangle[0] = angle;
		UpdateShaderConstantBuffer(cbuffer);
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);

		SetUsePixelShader(psRndHandle);
		MyDrawExtendGraph(0,0,640,720,cloudH,-1,-1, psStdHandle,4.0f,4.0f);
		MyDrawExtendGraph(640, 0, 1280, 720, cloudH, -1, -1, psRndHandle, 4.0f, 4.0f);
		ScreenFlip();
		++frame;
	}
	return 0;
}
#include<DxLib.h>
#include<array>
#include<vector>
//#include"resource1.h"

using namespace std;

void MyDrawGraph(int x, int y, int width, int height) {
	array<VERTEX2DSHADER, 4> verts;
	for (auto& v : verts) {
		v.rhw = 1.0f;
		v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
		v.spc = GetColorU8(0, 0, 0, 0);
	}
	verts[0].pos.x = x;
	verts[0].pos.y = y;
	verts[1].pos.x = x + width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	verts[2].pos.x = x;
	verts[2].pos.y = y + height;
	verts[2].v = 1.0f;
	verts[3].pos.x = x + width;
	verts[3].pos.y = y + height;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}

void MyDrawGraph(int texH, int normH, int shaderH, int x, int y, int width, int height) {
	SetUseTextureToShader(0, texH);
	SetUseTextureToShader(1, normH);
	SetUsePixelShader(shaderH);
	MyDrawGraph(x, y, width, height);
}

void MyDrawGraph(int x, int y, int graphH, int normH, int shaderH) {
	SetUseTextureToShader(0, graphH);
	SetUseTextureToShader(1, normH);
	SetUsePixelShader(shaderH);
	int w, h;
	GetGraphSize(graphH, &w, &h);
	MyDrawGraph(graphH, -1, shaderH, x, y, w, h);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//DxLib::SetWindowIconID(IDI_ICON1);
	SetGraphMode(1024, 1024, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	auto rtH = MakeScreen(1024, 1024);
	auto pshandle = LoadPixelShader(L"./raymarching.pso");
	auto cbuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
	float* gangle = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	float angle = 0.0f;

	while (ProcessMessage() != -1) {
		//SetRenderTargetToShader(0, rtH);
		ClearDrawScreen();
		angle += 0.05f;
		SetUseTextureToShader(0, rtH);
		
		gangle[0] = angle;
		UpdateShaderConstantBuffer(cbuffer);
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);

		SetUsePixelShader(pshandle);
		int w, h;
		GetGraphSize(rtH, &w, &h);
		MyDrawGraph(rtH, -1, pshandle, 0, 0, w, h);

		ScreenFlip();
	}
	return 0;
}
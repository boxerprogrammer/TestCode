#include<DxLib.h>
#include<array>
#include<vector>

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
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	auto ashuraH = LoadGraph(L"img/hotoke_asyura.png");
	auto auraH = LoadGraph(L"img/aura.png");
	auto pshandle = LoadPixelShader(L"spriteoutline.pso");
	auto cbuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
	float* gangle = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	float angle = 0.0f;
	SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);
	int baseH = MakeScreen(640, 480, true);
	int bubbleH = MakeScreen(640, 480, true);

	while (ProcessMessage() != -1) {
		SetDrawScreen(baseH);
		SetRenderTargetToShader(0, baseH);
		SetRenderTargetToShader(1, bubbleH);
		ClearDrawScreen();
		angle += 0.001f;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		SetUseTextureToShader(0, ashuraH);
		SetUseTextureToShader(1, auraH);

		gangle[0] = angle;
		UpdateShaderConstantBuffer(cbuffer);
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);

		SetUsePixelShader(pshandle);
		int w, h;
		GetGraphSize(ashuraH, &w, &h);
		MyDrawGraph(ashuraH, auraH, pshandle, 150, 50, w, h);


		SetRenderTargetToShader(0, -1);
		SetRenderTargetToShader(1, -1);
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		DrawGraph(0, 0, baseH, true);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawGraph(0, 0, bubbleH, true);
		GraphFilter(bubbleH, DX_GRAPH_FILTER_GAUSS, 8, 1600);
		DrawGraph(0, 0, bubbleH, true);
		DrawGraph(0, 0, bubbleH, true);
		DrawGraph(0, 0, bubbleH, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		ScreenFlip();
	}
	return 0;
}
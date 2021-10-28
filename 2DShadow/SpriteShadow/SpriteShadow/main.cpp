#include<DxLib.h>
#include<array>
#include<vector>

using namespace std;

void MyDrawGraph(int x,int y,int width,int height) {
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
	verts[1].pos.x = x+width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	verts[2].pos.x = x;
	verts[2].pos.y = y+height;
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

void MyDrawGraph(int x, int y, int graphH,int normH,int shaderH) {
	SetUseTextureToShader(0, graphH);
	SetUseTextureToShader(1, normH);
	SetUsePixelShader(shaderH);
	int w, h;
	GetGraphSize(graphH, &w, &h);
	MyDrawGraph(graphH,-1,shaderH,x, y, w, h);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE,  LPSTR, int) {
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(true);
	if (DxLib_Init()==-1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	auto characterH=LoadGraph(L"img/seiji_souridaijin_nobg3.png");
	auto characterNormalH = LoadGraph(L"img/seiji_souridaijin_nobg3_n.png");

	auto brickH=LoadGraph(L"img/brick.png");
	auto brickNormalH = LoadGraph(L"img/brick_n.png");

	auto psH = LoadPixelShader(L"SpriteShadowPS.pso");
	auto plpsH = LoadPixelShader(L"SpriteShadowPLPS.pso");
	auto cbufferH=CreateShaderConstantBuffer(sizeof(float)*4);
	float* pLight = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		int mx, my;
		GetMousePoint(&mx, &my);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);

		pLight[0] = mx;//x
		pLight[1] = my;//y
		pLight[2] = 0.0f;//z
		pLight[3] = 40000.0f;//w
		UpdateShaderConstantBuffer(cbufferH);
		SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 0);
		SetUseTextureToShader(0, brickH);
		SetUseTextureToShader(1, brickNormalH);
		SetUsePixelShader(plpsH);
		MyDrawGraph(brickH, brickNormalH, plpsH, 0, 0, 640, 480);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA,255);


		
		pLight[0] = mx;//x
		pLight[1] = my;//y
		pLight[2] = 0.0f;//z
		pLight[3] = 40000.0f;//w
		UpdateShaderConstantBuffer(cbufferH);
		SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 0);
		SetUseTextureToShader(0, characterH);
		SetUseTextureToShader(1, characterNormalH);
		SetUsePixelShader(psH);
		int w, h;
		GetGraphSize(characterH,&w,&h);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA,255);

		//MyDrawGraph(characterH, characterNormalH, psH, -40, 20, w, h);
		MyDrawGraph(characterH, characterNormalH, psH, 110, 20, w, h);
		//MyDrawGraph(characterH, characterNormalH, psH, 260, 20, w, h);


		DrawCircleAA(mx, my, 10, 16, 0xffffaa);

		ScreenFlip();
	}
	return 0;
}
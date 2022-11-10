#include<DxLib.h>
#include<array>

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

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(1024,768,32);
	DxLib_Init();
	//SetDrawScreen(DX_SCREEN_BACK);

	auto offscreen=MakeScreen(512,512,false);
	auto offscreen2 = MakeScreen(512, 512, false);
	auto wavePsH = LoadPixelShader(L"wave.pso");
	SetDrawScreen(offscreen);
	ClearDrawScreen();
	MyDrawGraph(0, 0, 512, 512);
	auto cbuffer = CreateShaderConstantBuffer(sizeof(float) * 4);
	float* gangle = static_cast<float*>(GetBufferShaderConstantBuffer(cbuffer));
	float angle = 0.0f;
	SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 0);
	auto hightH=LoadGraph(L"img/hight.png");
	//auto hightH = LoadGraph(L"img/square.png");

	auto psH=LoadPixelShader(L"hight2normal.pso");

	SetDrawScreen(DX_SCREEN_BACK);
	auto imgH = LoadGraph(L"img/img.png");
	//auto sphereH=MV1LoadModel(L"model/sphere.mv1");
	auto hightMap=LoadGraph(L"img/turbulant_n.png");
	auto sphereTex = LoadGraph(L"img/turbulant_col.png");
	//MV1SetPosition(sphereH, VGet(0, 0, 0));
	//MV1SetUseOrigShader(true);
	//auto vs3 = LoadVertexShader(L"threed.vso");
	auto ps3 = LoadPixelShader(L"ps.pso");
	

	SetUseLighting(true);
	SetLightDirection(VGet(1, -1, -1));
	SetGlobalAmbientLight({0.35,0.35,0.35,1});
	SetUseSpecular(true);

	while (ProcessMessage() != -1) {
		angle+=0.005f;
		gangle[0] = sin(angle);
		UpdateShaderConstantBuffer(cbuffer);
		SetShaderConstantBuffer(cbuffer, DX_SHADERTYPE_PIXEL, 3);
		SetDrawScreen(offscreen);
		ClearDrawScreen();
		SetUsePixelShader(wavePsH);
		MyDrawGraph(0, 0, 512, 512);

		SetDrawScreen(offscreen2);
		SetUseTextureToShader(0, offscreen);
		SetUsePixelShader(psH);
		MyDrawGraph(0, 0, 512, 512);

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		DrawGraph(0,0, offscreen,true);
		DrawGraph(512, 0, offscreen2, true);
		

		//SetCameraPositionAndTarget_UpVecY(VGet(0, 200, -400), VGet(0, 0, 0));
		//MV1SetRotationXYZ(sphereH, VGet(0, angle, 0));
		//auto add=MV1GetTextureSampleFilterMode(sphereH,0);
		//SetUseVertexShader(vs3);
		SetUsePixelShader(ps3);
		SetUseTextureToShader(0, imgH);
		SetUseTextureToShader(1, offscreen2);
		MyDrawGraph(300, 300, 512, 512);
		//SetUseTextureToShader(0, sphereTex);
		
		//MV1DrawModel(sphereH);

		ScreenFlip();
	}
	return 0;
}
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
	SetDrawScreen(DX_SCREEN_BACK);

	auto hightH=LoadGraph(L"img/hight.png");

	auto psH=LoadPixelShader(L"hight2normal.pso");

	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		DrawGraph(0,0, hightH,true);

		SetUseTextureToShader(0, hightH);
		SetUsePixelShader(psH);
		MyDrawGraph(512,0,512,512);

		ScreenFlip();
	}
	return 0;
}
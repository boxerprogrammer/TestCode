#include<DxLib.h>
#include<vector>

using namespace std;

vector< VERTEX2D> 
CreateScreenMesh(int x,int y,int w,int h,int divX,int divY){
	vector<VERTEX2D> v(3*2);

	for (auto& vv : v) {
		vv.dif = GetColorU8(255, 255, 255, 255);
	}

	int pidx = 0;
	//ç∂â∫éOäp
	v[0].pos.x = x;
	v[0].pos.y = y;
	v[0].u = 0;
	v[0].v = 0;
	v[0].rhw = 1.0;

	v[1].pos.x = x+w;
	v[1].pos.y = y+h;
	v[1].u = 1;
	v[1].v = 1;
	v[1].rhw = 1.0;

	v[2].pos.x = x ;
	v[2].pos.y = y + h;
	v[2].u = 0;
	v[2].v = 1;
	v[2].rhw = 1.0;
	//âEè„éOäp
	pidx = 1;
	int startIdx = pidx * 3;
	v[startIdx+0].pos.x = x;
	v[startIdx + 0].pos.y = y;
	v[startIdx + 0].u = 0;
	v[startIdx + 0].v = 0;
	v[startIdx + 0].rhw = 1.0;

	v[startIdx + 1].pos.x = x+w;
	v[startIdx + 1].pos.y = y ;
	v[startIdx + 1].u = 1;
	v[startIdx + 1].v = 0;
	v[startIdx + 1].rhw = 1.0;

	v[startIdx + 2].pos.x = x + w;
	v[startIdx + 2].pos.y = y + h;
	v[startIdx + 2].u = 1;
	v[startIdx + 2].v = 1;
	v[startIdx + 2].rhw = 1.0;

	return v;

}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	

	DxLib_Init();

	SetDrawScreen(DX_SCREEN_BACK);

	auto vertices=CreateScreenMesh(0, 0, 640, 480, 1, 1);
	auto rtH=MakeScreen(640, 480);
	while (ProcessMessage() != -1) {
		SetDrawScreen(rtH);
		ClearDrawScreen();

		DrawBox(100, 100, 200, 300, 0xffaaff,true);

		DrawCircleAA(300, 300, 120, 10, 0xaaffaa);

		DrawBox(400, 100, 500, 400, 0xaaaaff, true);

		DrawCircleAA(300, 300, 120, 10, 0xaaffaa);
		DrawCircleAA(400, 200, 40, 10, 0xfaaaff);

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();


		for (int i = 0; i < 3; ++i) {
			vertices[i].pos.y += 0.1f;
		}

		DrawPolygon2D(vertices.data(), 2, rtH, true);


		ScreenFlip();
	}

}
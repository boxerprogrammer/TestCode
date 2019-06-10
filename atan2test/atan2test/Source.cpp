#include<DxLib.h>
#include<math.h>

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(true);
	SetWindowText("atan2ó˚èKÉTÉìÉvÉã");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	auto arrowcatH=LoadGraph("img/arrowcat.png");

	float posx = 320;
	float posy = 240;
	float vx = 0;
	float vy = 0;
	while (!ProcessMessage()) {
		ClearDrawScreen();
		
		int mx, my;
		GetMousePoint(&mx, &my);

		auto angle = atan2(my - posy, mx - posx);
		DrawRotaGraph(posx, posy, 1.0f, angle, arrowcatH,true);

		auto m=GetMouseInput();
		if (m) {
			vx = cos(angle);
			vy = sin(angle);
		}
		posx += vx;
		posy += vy;

		ScreenFlip();
	}
	DxLib_End();
}
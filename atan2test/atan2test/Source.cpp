#include<DxLib.h>
#include<math.h>

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {
	SetGraphMode(640, 480, 32);
	ChangeWindowMode(true);
	SetWindowText("atan2ó˚èKÉTÉìÉvÉã");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	//SetDrawMode(DX_DRAWMODE_BILINEAR);
	auto arrowcatH=LoadGraph("img/arrowcat.png");

	int posx = 320;
	int posy = 240;

	while (!ProcessMessage()) {
		ClearDrawScreen();
		
		int mx, my;
		GetMousePoint(&mx, &my);

		auto angle = atan2(my - posy, mx - posx);
		DrawRotaGraph(posx, posy, 1.0f, angle, arrowcatH,true);

		ScreenFlip();
	}
	DxLib_End();
}
#include<DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	ChangeWindowMode(true);

	SetGraphMode(640, 480, 32);



	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	auto scrH=MakeScreen(384, 288);

	auto farBgH = LoadGraph("img/background.png");
	auto midBgH=LoadGraph("img/middleground.png");
	auto grdH = LoadGraph("img/sliced-tileset/ground.png");
	int playerH[6];
	LoadDivGraph("img/bearded-walk.png", 6, 6, 1, 40, 47, playerH);

	int houseH[3];
	houseH[0] = LoadGraph("img/props-sliced/house-a.png");
	houseH[1] = LoadGraph("img/props-sliced/house-b.png");
	houseH[2] = LoadGraph("img/props-sliced/house-c.png");

	int scroll = 0;
	auto houseX = 384;
	auto rightX = 384;
	auto houseIdx = rand() % 3;
	while (ProcessMessage() == 0) {
		SetDrawScreen(scrH);
		ClearDrawScreen();

		

		DrawGraph(-(scroll/3%384), 0, farBgH,false);
		DrawGraph(-(scroll/3%384)+384, 0, farBgH, false);

		DrawGraph(-(scroll/2 % 384), 0, midBgH, true);
		DrawGraph(-(scroll/2 % 384) + 384, 0, midBgH, true);

		DrawTile(-(scroll*2%32), 288 - 32, 384, 288, 1, 1, 1.0f, 0.0f, grdH, true);

		houseX = rightX - scroll;

		int w, h;
		GetGraphSize(houseH[houseIdx], &w, &h);
		DrawGraph(houseX,288-32-h, houseH[houseIdx],true);

		if (houseX + w < 0) {
			houseIdx = rand() % 3;
			rightX = scroll + 384;
		}

		DrawGraph(180, 288 - 32 - 47, playerH[scroll / 4 % 6], true);

		++scroll;

		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, 640, 480, scrH, false);

		ScreenFlip();
	}
	DxLib_End();
}
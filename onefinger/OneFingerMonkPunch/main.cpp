#include<DxLib.h>
#include<cmath>

const int screen_width = 640;
const int screen_height = 480;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	SetWindowText("OneFingerMonkPunch");
	SetGraphMode(screen_width, screen_height, 32);
	ChangeWindowMode(true);
	DxLib_Init();

	auto titleH=LoadGraph("img/title-screen.png");
	int titleWidth, titleHeight;
	GetGraphSize(titleH, &titleWidth, &titleHeight);

	auto bgH = LoadGraph("img/bg/background.png");
	int bgWidth, bgHeight;
	GetGraphSize(bgH, &bgWidth, &bgHeight);

	auto floorH = LoadGraph("img/bg/tileset.png");

	SetDrawScreen(DX_SCREEN_BACK);

	int lastinput = 0;
	int frame = 0;

	float rate = 2.0f;
	int modulous = bgWidth * rate;

	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		int mouseinput=DxLib::GetMouseInput();

		int ofst = frame % (int)(bgWidth*rate);
		DrawRotaGraph2(-ofst, 0, 0, 0, rate, 0.0f, bgH, true);
		DrawRotaGraph2(-ofst + bgWidth * rate, 0, 0, 0, rate, 0.0f, bgH, true);

		int floorOfst = frame % (int)(48*rate);
		for (int i = 0; i <=( 640 / (48 * rate))+1; ++i) {
			DrawRectRotaGraph2(-floorOfst+i*48*rate, 480 - 80, 0, 168, 48, 40, 0, 0, rate, 0.0f, floorH, true);
		}
		for (int i = 0; i < titleHeight; ++i) {
			int offsetx = sin((i + frame * 2)*3.14f / 180.0f)*min(max(titleHeight / 2 - i, 0), 40) / 2 + cos((i + frame)*3.14f / 20.0f)*min((titleHeight - i) / 8, 3);
			DrawRectGraph((screen_width - titleWidth) / 2 + offsetx,
				(screen_height - titleHeight) / 2 + i,
				0, i,
				titleWidth,
				1,
				titleH, true);


			
		}

		ScreenFlip();

		lastinput = mouseinput;
		++frame;
	}

	DxLib_End();
}
#include<DxLib.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	ChangeWindowMode(true);
	SetGraphMode(1280, 720, 32);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	int cutinH = LoadGraph("img/cutin.png");

	int yajuH = LoadGraph("img/aztek_tile.jpg");

	int maskH = LoadMask("img/cutin_msk.png");

	int bgH = LoadGraph("img/bg.jpg");


	int imgHs[30];
	LoadDivGraph("img/prominence.png", 30, 10, 3, 256, 256, imgHs);

	CreateMaskScreen();
	SetUseMaskScreenFlag(false);
	//DrawMask(0, 0, maskH, DX_MASKTRANS_BLACK);
	char keystate[256];
	int frame = 0;
	bool onMask = false;
	int frame2 = 0;
	int wait = 10;
	int flameFrame = 0;
	bool onNagare = false;
	SetFontSize(32);
	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		DrawGraph(0, 0, yajuH, true);

		DxLib::GetHitKeyStateAll(keystate);

		if (keystate[KEY_INPUT_RETURN]) {
			onMask = true;
		}

		if (onMask) {
			frame = min(frame + 50, 1280);
			if (frame == 1280) {
				onNagare = true;
			}
		}
		

		if (onNagare) {
			--wait;
			if (wait < 0) {
				frame2 += 80;
			}
		}

		if (onNagare&&frame2 > 1280) {
			onMask = false;
			wait = 30;
			frame = 0;
			frame2 = 0;
			onNagare = false;
		}


		FillMaskScreen(0);

		DrawFillMask(frame2, 0, frame2+frame, 720, maskH);

		if (onMask) {
			SetUseMaskScreenFlag(true);
		}

		//for (int i = 0; i < 20; ++i) {
		//	for (int j = 0; j < 15; ++j) {
		//		if ((i + j) % 2 == 0) {
		//			DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, 0xffffff, true);
		//		}
		//		else {
		//			DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, 0x000000, true);
		//		}
		//	}
		//}


		DrawGraph(0, 0, bgH, false);
		DrawBox(0, 720 - 256, 1280, 720 - 240, GetColor(255, 255,224), true);
		for (int i = 0; i < 16; ++i) {
			int j = (i+flameFrame/2) % 16;
			DrawBox(0, 720 - 16 * j, 1280, 720 - 16 * (j-1), GetColor(255,16*(15-i),12*(15-i)), true);
		}
		for (int i = 0; i < (2560 / 256); ++i) {
			if ((i)%2) {
				DrawTurnGraph(i * 256, 720 - 256*2, imgHs[(flameFrame + i) % 30], true);
			}
			else {
				DrawGraph(i * 256, 720 - 256*2, imgHs[(flameFrame + i) % 30], true);
			}
		}
		DrawString(52, 52, "カットインテスト", 0x000000);
		DrawString(50, 50, "カットインテスト", 0xffffff);
		flameFrame = (flameFrame + 1) % 30;
		SetUseMaskScreenFlag(false);

		DrawRectGraph(frame2, 0, 0, 0, frame, 720, cutinH, true);
		//DrawGraph(0, 0, cutinH, true);

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}
#include<DxLib.h>

int main() {
	ChangeWindowMode(true);
	DxLib_Init();

	SetDrawScreen(DX_SCREEN_BACK);
	auto bg=LoadGraph(L"img/bg.jpg");
	auto bb = LoadGraph(L"img/bb.png");
	float angle = 0.0f;
	float scale = 0.01f;
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		DrawExtendGraph(0, 0,640,480, bg, true);
		for (int j = 0; j < 15; ++j) {
			for (int i = 0; i < 20; ++i) {
				DrawRotaGraph(i * 32, j * 32, (i+j+1)*scale, angle,bb,true);
			}
		}
		ScreenFlip();
		angle += 0.01f;
		scale *= 1.01f;
	}


	DxLib_End();

}
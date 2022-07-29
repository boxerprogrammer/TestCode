#include<DxLib.h>
#include<cmath>

using namespace std;

int WINAPI WinMain(HINSTANCE, HINSTANCE,  LPSTR,int ) {
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	auto bg = LoadGraph(L"./img/BG.png"); 
	auto fg = LoadGraph(L"./img/FG.png");
	auto light = LoadGraph(L"./img/light.png");
	auto frameMask = LoadMask(L"./img/FrameMask.png");
	
	float angle = 0;
	while (ProcessMessage() != -1) {
		auto mask = CreateMaskScreen();
		FillMaskScreen(0);//É}ÉXÉNÇçïÇ≈ìhÇËÇ¬Ç‘Ç∑
		ClearDrawScreen();

		
		
		DrawExtendGraph(-106, 0,840,480, bg, false);
		FillMaskScreen(1);
		DrawMask(200, 200, frameMask, false);
		DrawExtendGraph(-106, 0, 840, 480, fg, false);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawRotaGraph(300+100*cos(angle), 250+50*sin(angle), 1.0f, 0.0f, light, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		


		


		angle += 0.1;
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
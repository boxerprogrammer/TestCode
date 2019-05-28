#include<DxLib.h>
#include<cmath>
#include<cassert>

const int screen_width = 640;
const int screen_height = 480;

struct ImageResource {
	int handle;
	int width;
	int height;
	
};



ImageResource 
ImageResourceFromFile(const char* path) {
	ImageResource ret = {};
	ret.handle = LoadGraph(path);
	assert(ret.handle > 0);
	GetGraphSize(ret.handle, &ret.width, &ret.height);
	return ret;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	SetWindowText("OneFingerMonkPunch");
	SetGraphMode(screen_width, screen_height, 32);
	ChangeWindowMode(true);
	DxLib_Init();

	
	auto title=ImageResourceFromFile("img/title-screen.png");
	auto pushenter = ImageResourceFromFile("img/press-enter-text.png");
	auto bg = ImageResourceFromFile("img/bg/background.png");

	auto floor_tile = ImageResourceFromFile("img/bg/tileset.png");
	const auto floorBlockWidth = 48;
	const auto floorBlockHeight = 40;

	SetDrawScreen(DX_SCREEN_BACK);

	int lastinput = 0;
	int frame = 0;

	int lightningFrame = 45 + rand() % 60;

	const float rate = 2.0f;
	int modulous = bg.width * rate;

	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		int mouseinput=DxLib::GetMouseInput();

		int ofst = frame % (int)(bg.width*rate);
		DrawRotaGraph2(-ofst, 0, 0, 0, rate, 0.0f, bg.handle, true);
		DrawRotaGraph2(-ofst + bg.width * rate, 0, 0, 0, rate, 0.0f, bg.handle, true);
		
		int floorOfst = frame % (int)(floorBlockWidth*rate);
		for (int i = 0; i <=(screen_width / (floorBlockWidth * rate))+1; ++i) {
			DrawRectRotaGraph2(-floorOfst+i* floorBlockWidth*rate, screen_height - floorBlockHeight*rate, 0, 168, floorBlockWidth, floorBlockHeight, 0, 0, rate, 0.0f, floor_tile.handle, true);
		}
		--lightningFrame;
		if (lightningFrame==5 || lightningFrame==1) {
			//明るくする
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			DrawBox(0, 0, screen_width, screen_height, 0x555555, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			
		}

		if (lightningFrame == 0) {
			lightningFrame = 45 + rand() % 60;
		}


		//タイトルロゴ表示
		for (int i = 0; i < title.height; ++i) {
			int offsetx = sin((i + frame * 2)*3.14f / 180.0f)*min(max(title.height / 2 - i, 0), 40) / 2 + cos((i + frame)*3.14f / 20.0f)*min((title.height- i) / 8, 3);
			DrawRectGraph((screen_width - title.width) / 2 + offsetx,
				(screen_height - title.height) / 2-50 + i,
				0, i,
				title.width,
				1,
				title.handle, true);
		}

		if ((frame / 25) % 2 == 0) {
			DrawGraph((screen_width - pushenter.width) / 2, (screen_height - pushenter.height) / 2+20, pushenter.handle, true);
		}
		

		ScreenFlip();

		lastinput = mouseinput;
		++frame;
	}

	DxLib_End();
}
#include<stdio.h>
#include<vector>
#include<DxLib.h>


int main() {
	auto fp=fopen("anim.anm", "r");
	char patternName[64];
	char blank[256];
	int ret = 0;
	//パターン取得
	while ((ret=fscanf(fp, "PATTERN_NAME=%s\n", patternName)) == 0) {
		if (fgets(blank, 256, fp) == nullptr) {
			break;
		}
	}
	printf("patternname=%s\n", patternName);
	struct FrameData {
		char filename[64];
		char titlename[16];
		int srcX;
		int srcY;
		int layerNum;
		int Width;
		int Height;
		int destX;
		int destY;
		int delay;
		int layerAdd;
		int ckeyFlg;
		int ckeyNum;
	};
	std::vector<FrameData> frames;
	while (true) {
		int frameno = 0;
		//各フレーム情報取得
		while ((ret = fscanf(fp, "\tFRAME_NUMBER=%d\n", &frameno)) == 0) {
			if (fgets(blank, 256, fp) == nullptr) {
				break;
			}
		}
		
		FrameData framedata = {};
		fscanf(fp, "\t\tFILENAME=%s\n", framedata.filename);
		fscanf(fp, "\t\tWIN_TITLE=%s\n", framedata.titlename);
		fscanf(fp, "\t\tSRC_X=%d\n", &framedata.srcX);
		fscanf(fp, "\t\tSRC_Y=%d\n", &framedata.srcY);
		fscanf(fp, "\t\tLAYER_NUM=%d\n", &framedata.layerNum);
		fscanf(fp, "\t\tWIDTH=%d\n", &framedata.Width);
		fscanf(fp, "\t\tHEIGHT=%d\n", &framedata.Height);
		fscanf(fp, "\t\tDEST_X=%d\n", &framedata.destX);
		fscanf(fp, "\t\tDEST_Y=%d\n", &framedata.destY);
		fscanf(fp, "\t\tDELAY=%d\n", &framedata.delay);
		if (fgetc(fp) == EOF) {
			break;
		}
		frames.push_back(framedata);
		
	}
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	auto frameH=LoadGraph(frames[0].filename);
	int index = 0;
	auto lastcount=GetTickCount();
	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		auto& frame = frames[index];
		DrawRectRotaGraph(100, 100, frame.srcX, frame.srcY, frame.Width, frame.Height,4.0f,0.0f, frameH, true);
		if (GetTickCount()-lastcount >= frame.delay) {
			index=(index+1)%frames.size();
			lastcount = GetTickCount();
		}
		ScreenFlip();
	}
}
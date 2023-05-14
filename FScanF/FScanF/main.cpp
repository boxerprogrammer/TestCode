#include<DxLib.h>
#include<iostream>
#include<vector>

using namespace std;

struct Vector2 {
	float x, y;
	Vector2() :x(0), y(0) {}
	Vector2(float inx, float iny) :x(inx), y(iny) {}
};
using Position2 = Vector2;

struct Segment {
	Position2 start;
	Position2 end;
	void Draw(unsigned int col=0xffffff);
	Segment(float x1, float y1, float x2, float y2);
	Segment():Segment(0,0,0,0) {}
};

Segment::Segment(float x1, float y1, float x2, float y2):
start(x1,y1),
end(x2,y2){
	start.x = start.x * 200 + 320;
	start.y = 240-start.y * 200 ;
	end.x = end.x * 200 + 320;
	end.y = 240- end.y * 200 ;
}

void
Segment::Draw(unsigned int col) {

	DxLib::DrawLineAA(start.x , start.y , end.x, end.y, col, 3.0f);
	DrawCircleAA(end.x, end.y, 10.0f, 32, col, true);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	vector<Segment> segments;

	FILE* fp = nullptr;
	fopen_s(&fp,"./textdata2.txt","r");
	if (fp == nullptr)return 0;
	while (!feof(fp)) {
		float x, y, z;
		fscanf_s(fp, "RightArmVec = %f, %f, %f\n", &x,&y,&z);
		segments.emplace_back(0.2f, 0.0f,x, y);
		fscanf_s(fp, "LeftArmVec = %f, %f, %f\n", &x,&y,&z);
		segments.emplace_back(-0.2f,0.0f, x, y);
	}
	fclose(fp);
	ChangeWindowMode(true);
	const size_t dataNum = segments.size()/2;
	if (DxLib_Init() == -1) {
		return 0;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	int currentIdx = 0;
	int frame = 0;
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		DrawFormatString(10, 10, 0xffffff, L"segment num=%d", dataNum);
		
		currentIdx = frame / 2;
		segments[currentIdx * 2].Draw(0xffaaaa);
		segments[currentIdx * 2+1].Draw(0xaaaaff);
		ScreenFlip();
		frame = (frame + 1) % (dataNum*2);
	}
	DxLib_End();
}

//640x480の範囲でRay Tracing in One Weekendをやる
//テキストファイルに書き出すのは重たいので、640x480の内部メモリに結果を
//書き出して、それをCreateGraphFromBaseImageなどでハンドルを得る。
//そしてそれをDrawGraph等で表示する。

#include<DxLib.h>
#include<array>
#include<vector>

using namespace std;

struct Color {
	UINT8 r, g, b,a;
	Color() :r(0), g(0), b(0), a(0) {}
	Color(UINT8 inr, UINT8 ing, UINT8 inb, UINT8 ina) :r(inr), g(ing), b(inb), a(ina) {};
	void SetColor(UINT8 inr, UINT8 ing, UINT8 inb, UINT8 ina) {
		r = inr;
		g = ing;
		b = inb;
		a = ina;
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	
	vector<Color> data(640 * 480);
	for (auto& d : data) {
		d.SetColor(rand() % 256, rand() % 256, rand() % 256, 255);
	}
	SetGraphMode(640, 480, 32);
	DxLib::ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);



	BASEIMAGE bimg = {};

	//CreateBaseImageToFile(L"baseimage.png", &bimg);
	bimg.GraphData = data.data();
	//bimg.GraphDataCount = data.size();
	bimg.ColorData.ColorBitDepth = 32;
	bimg.ColorData.BlueLoc = 8;
	bimg.ColorData.GreenLoc = 16;
	bimg.ColorData.AlphaLoc= 24;
	bimg.ColorData.RedLoc = 0;
	bimg.ColorData.PixelByte = 4;
	bimg.ColorData.AlphaWidth = 8;
	bimg.ColorData.RedWidth = 8;
	bimg.ColorData.BlueWidth = 8;
	bimg.ColorData.GreenWidth = 8;
	bimg.ColorData.RedMask = 0x000000ff;
	bimg.ColorData.BlueMask = 0x0000ff00;
	bimg.ColorData.GreenMask = 0x00ff0000;
	bimg.ColorData.AlphaMask= 0xff000000;

	bimg.ColorData.Format = DX_BASEIMAGE_FORMAT_NORMAL;

	bimg.Width = 640;
	bimg.Height = 480;
	bimg.Pitch = 640 * sizeof(data[0]);

	auto gH=CreateGraphFromBaseImage(&bimg);

	while (ProcessMessage() != -1) {
		for (auto& d : data) {
			d.SetColor(rand() % 256, rand() % 256, rand() % 256, 255);
		}
		
		ReCreateGraphFromBaseImage(&bimg,gH);
		DrawGraph(0,0,gH,false);
		auto fps=GetFPS();
		DrawFormatString(10, 10, 0xffffff, L"%f", fps);
		ScreenFlip();
	}
	DxLib_End();
}
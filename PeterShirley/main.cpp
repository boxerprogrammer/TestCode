//640x480の範囲でRay Tracing in One Weekendをやる
//テキストファイルに書き出すのは重たいので、640x480の内部メモリに結果を
//書き出して、それをCreateGraphFromBaseImageなどでハンドルを得る。
//そしてそれをDrawGraph等で表示する。
#define NOMINMAX
#include<DxLib.h>
#include<array>
#include<vector>
#include<assert.h>
#include"Ray.h"
constexpr int screen_width = 640;
constexpr int screen_height = 360;
constexpr float screen_width_f = 640.0f;
constexpr float screen_height_f = 360.0f;
using namespace std;


struct ColorU8 {
	UINT8 r, g, b, a;
	ColorU8() :r(0), g(0), b(0), a(0) {}
	ColorU8(UINT8 inr, UINT8 ing, UINT8 inb, UINT8 ina) :r(inr), g(ing), b(inb), a(ina) {};
	void SetColor(UINT8 inr, UINT8 ing, UINT8 inb, UINT8 ina) {
		r = inr;
		g = ing;
		b = inb;
		a = ina;
	}
};
using ColorData_t = vector<ColorU8>;
void 
DrawPixelData(ColorData_t& data, int x, int y, UINT8 r, UINT8 g, UINT8 b) {
	assert(data.size() == screen_width * screen_height);
	assert(0 <= x && x < screen_width);
	assert(0 <= y && y < screen_height);
	data[(size_t)x + (size_t)screen_width * (size_t)y].SetColor(r, g, b, 255);
}

///0～1のRGB浮動小数で色を打つ
///@param x X座標
///@param y Y座標
///@param r 赤(0～1)
///@param g 緑(0～1)
///@param b 青(0～1)
void
DrawPixelData(ColorData_t& data, int x, int y, Color col) {
	DrawPixelData(data, x, y, min(int(col.x * 0xff), 0xff), min(int(col.y * 0xff), 0xff), min(int(col.z * 0xff), 0xff));
}

///0～1のRGB浮動小数で色を打つ
///@param x X座標
///@param y Y座標
///@param r 赤(0～1)
///@param g 緑(0～1)
///@param b 青(0～1)
void
DrawPixelDataFloat(ColorData_t& data, int x, int y, float r, float g, float b) {
	DrawPixelData(data, x, y, min(int(r * 0xff), 0xff), min(int(g * 0xff), 0xff), min(int(b * 0xff), 0xff));
}

DxLib::BASEIMAGE 
CreateRaytracingCanvas(ColorData_t& data) {
	BASEIMAGE bimg = {};
	bimg.GraphData = data.data();
	bimg.ColorData.ColorBitDepth = 32;
	bimg.ColorData.BlueLoc = 8;
	bimg.ColorData.GreenLoc = 16;
	bimg.ColorData.AlphaLoc = 24;
	bimg.ColorData.RedLoc = 0;
	bimg.ColorData.PixelByte = 4;
	bimg.ColorData.AlphaWidth = 8;
	bimg.ColorData.RedWidth = 8;
	bimg.ColorData.BlueWidth = 8;
	bimg.ColorData.GreenWidth = 8;
	bimg.ColorData.RedMask = 0x000000ff;
	bimg.ColorData.GreenMask = 0x0000ff00;
	bimg.ColorData.BlueMask = 0x00ff0000;
	bimg.ColorData.AlphaMask = 0xff000000;
	bimg.ColorData.Format = DX_BASEIMAGE_FORMAT_NORMAL;
	bimg.Width = screen_width;
	bimg.Height = screen_height;
	bimg.Pitch = screen_width * sizeof(data[0]);
	return bimg;
}

Color 
RayColor(const Ray& ray) {
	auto unitDirection = ray.Direction().Normalized();
	auto t = 0.5 * (unitDirection.y + 1.0);
	return(1.0 - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//画面関連
	constexpr float aspect_ratio = screen_width_f / screen_height_f;
	//カメラ関連
	constexpr auto viewport_height = 2.0f;
	constexpr auto viewport_width = aspect_ratio*viewport_height;
	constexpr auto focal_length = 1.0f;

	//レイに関するやつ
	auto origin = Position3(0.0f, 0.0f, 0.0f);
	auto horisontal = Vector3(viewport_width,0.0f,0.0f);
	auto vertical = Vector3(0.0f, viewport_height, 0.0f);
	auto lower_left_corner = origin - horisontal / 2.0f - vertical / 2.0f - Vector3(0.0f, 0.0f, focal_length);
	


	ColorData_t data(screen_width * screen_height);

	SetGraphMode(screen_width, screen_height, 32);
	DxLib::ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	auto bimg = CreateRaytracingCanvas(data);
	auto gH=CreateGraphFromBaseImage(&bimg);

	while (ProcessMessage() != -1) {
#pragma omp parallel for
		for (int y = 0; y < screen_height; ++y) {
#pragma omp parallel for
			for (int x = 0; x < screen_width; ++x) {
				auto u = static_cast<float>(x) / (screen_width_f - 1.0f);
				auto v = static_cast<float>(y) / (screen_height_f - 1.0f);
				Ray ray(origin, lower_left_corner + u * horisontal + v * vertical - origin);
				DrawPixelData(data,x, screen_height-y-1,RayColor(ray));
			}
		}

		ReCreateGraphFromBaseImage(&bimg,gH);
		DrawGraph(0,0,gH,false);
		auto fps=GetFPS();
		DrawFormatString(10, 10, 0xffffff, L"%f", fps);
		ScreenFlip();
	}
	DxLib_End();
}
#include<DxLib.h>
#include"Geometry.h"

Vector2f SmoothDamp(const Vector2f& from, const Vector2f& to, Vector2f& vel, float smoothTime, float deltaTime) {
	float omega = 2.0f / smoothTime;
	float x = omega * deltaTime;
	float exp = 1.0f/(1.0f + x + 0.48f * x * x + 0.235f * x * x * x);//テイラー展開
	auto change = from - to;
	auto temp = (vel + change*omega) * deltaTime;
	vel = (vel - temp*omega) * exp;
	return to + (change + temp) * exp;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {


	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	Position2f to;
	Position2f from;

	to = from = Position2f(320, 240);

	auto tm=GetNowCount();
	while (ProcessMessage() != -1) {
		ClearDrawScreen();

		int x, y;
		GetMousePoint(&x, &y);
		to.x = x;
		to.y = y;
		float delta = GetNowCount()-tm;
		
		Vector2f v;
		from=SmoothDamp(from, to, v, 8.0f, 2.0f);

		DrawCircleAA(to.x, to.y, 20, 32, 0xaaaaff);
		DrawCircleAA(from.x, from.y, 15, 32, 0xFf8888);

		ScreenFlip();
	}


}
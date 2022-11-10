#include<DxLib.h>
#include<cmath>
#include<algorithm>

using namespace std;
struct Vector2 {
	float x, y;
	Vector2 operator+(const Vector2& v) {
		return { v.x + x,v.y + y };
	}
	void operator+=(const Vector2& v) {
		x += v.x;
		y += v.y;
	}
};

using Position2 = Vector2;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(640, 480, 32);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	float angle = DX_PI_F / 2.0f;
	float radius = 10.0f;
	float scale = 0.1f;
	Position2 pos = {}, center = {}, lastpos = {};
	center.x = 640.0f / 2.0f-200.0f;
	center.y = 480.0f / 2.0f;
	int snd = LoadSoundMem(L"./fire.wav");
	int img= LoadGraph(L"./fireball.png");
	int title = LoadGraph(L"./title.png");
	int fireScreenBack = MakeScreen(640, 480,true);
	int fireScreenFront = MakeScreen(640, 480,true);
	char keystate[256];
	bool start = false;
	float vecx = 0.5f;
	int count = 0;
	while (ProcessMessage() != -1) {
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_RETURN]) {
			start = true;
			PlaySoundMem(snd, DX_PLAYTYPE_BACK);
		}
		ClearDrawScreen();
		for (int i = 0; i < 10; ++i) {
			if (start) {
				if (count < 300) {
					scale = clamp(scale + 0.01f, 0.0f, 4.0f);
					center += {vecx, 0.0f};
					lastpos = pos;
					pos.x = center.x + (cos(angle)-0.5*sin(angle)) * radius;
					pos.y = center.y + sin(angle) * radius;
					radius += 0.5f;
					angle += 0.032f;
					vecx += 0.005f;
					++count;
				}
			}
			else {
				pos.x = center.x + cos(0.0f) * radius;
				pos.y = center.y + sin(0.0f) * radius;
			}
			if (radius <= 100.0f) {
				SetDrawScreen(fireScreenBack);
				SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				DrawRotaGraph(pos.x, pos.y, scale, 0.0f, img, true);
			}

			if (radius > 100.0f) {
				if (count < 300) {
					SetDrawScreen(fireScreenFront);
					SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
					DrawRotaGraph(pos.x, pos.y, scale, 0.0f, img, true);
				}
			}
		}
		SetDrawScreen(DX_SCREEN_BACK);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawGraph(0, 0, fireScreenBack,true);
		DrawRotaGraph(320, 240, 1.0, 0.0f, title, true);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawGraph(0, 0, fireScreenFront, true);

		
		ScreenFlip();
	}
	DxLib_End();
}
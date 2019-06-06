#include<DxLib.h>
#include"Geometry.h"

int WINAPI WinMain() {
	ChangeWindowMode(true);
	SetWindowText("矩形当たり判定サンプル");
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	Rect rcA(100,100,100,100);
	Rect rcB(300,300,100,100);
	char keystate[256];
	Vector2 v(0,0);
	while (!ProcessMessage()) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);
		v = Vector2(0, 0);
		if (keystate[KEY_INPUT_LEFT]) {
			v.x-=10;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			v.x+=10;
		}
		if (keystate[KEY_INPUT_UP]) {
			v.y-=10;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			v.y+=10;
		}
		rcA.pos += v;
		int color = 0xffffff;
		Size overlap(0,0);
		if (IsCollided(rcA, rcB,overlap)) {
			color = 0xffaaaa;
			Vector2 rv(overlap.w, overlap.h);
			rv.Normalize();
			rcA.pos -= v * rv;
		}

		//矩形の表示
		rcA.Draw(color);
		rcB.Draw(color);

		ScreenFlip();
	}
	DxLib_End();
}
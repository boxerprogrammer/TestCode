#include<DxLib.h>
#include"Geometry.h"
#include<cmath>


/// <summary>
/// 長方形の当たり判定を返す
/// </summary>
/// <param name="rcA">矩形A</param>
/// <param name="rcB">矩形B</param>
/// <returns>
///  rcAとrcBが重なってる:true
///  rcAとrcBが重なってない:false
///</returns>
bool IsHitAABB(const Rect& rcA,const Rect& rcB) {
	Vector2 diff= rcA.Center() - rcB.Center();
	
	return (abs(diff.x)<(rcA.Width()+rcB.Width())/2&&
		abs(diff.y) < (rcA.Height() + rcB.Height()) / 2);
}
bool IsHitCircles(const Circle& cA, const Circle& cB) {
	float xdiff = cB.center.x - cA.center.x;
	float ydiff = cB.center.y - cA.center.y;
	return hypot(xdiff,ydiff)<=cA.radius+cB.radius;
}

float Clamp(float value, float minVal = 0.0f, float maxVal = 1.0f) {
	return max(min(maxVal, value), minVal);
}

bool IsHitCircleAndFan(const Circle& c, const Fan& fan) {
	
	float diff = (c.center - fan.center).Magnitude();
	//①まず、半径外にいれば確定で当たってないので除外するで
	if (diff > fan.Radius() + c.radius)return false;
	//②中心が扇形のv1とv2の間に入っていれば確定当たり。はっきりわかんだね。
	auto v = (c.center - fan.center);
	auto betAngle = fan.GetAngle();
	if ( GetAngle2Vector(fan.v1,v) < betAngle ){
		return true;
	}
	//③最後はそれぞれの線分との距離を測る(ここがちょっとめんどい)
	//v1と比較
	if ((v - fan.v1*Clamp(Dot(v, fan.v1)/fan.v1.Square())).Magnitude() <= c.radius) {
		return true;
	}
	//v2と比較
	if ((v - fan.v2 * Clamp(Dot(v, fan.v2)/fan.v2.Square())).Magnitude() <= c.radius) {
		return true;
	}

	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText(L"円の当たり判定");
	if (DxLib::DxLib_Init()) {
		return -1;
	}
	auto circleH=LoadGraph(L"TestCircle.png");
	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	Rect rcA = { {0,0},100,100 };
	Rect rcB = { {300,300},80,80 };

	Circle cA = { { 250.0f,50.0f},50.0f };
	Circle cB = { { 300.0f,300.0f},50.0f };

	Fan fan(Position2(150, 150), Vector2(150, 0), DX_PI_F*2.0f / 3.0f);

	while (!DxLib::ProcessMessage()) {
		DxLib::ClearDrawScreen();
		char keystate[256];
		DxLib::GetHitKeyStateAll(keystate);

		int speed = 4;

		int vax = 0, vay = 0;
		int vbx = 0, vby = 0;

		if (keystate[KEY_INPUT_UP]) {
			vay = -speed;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			vay = +speed;
		}
		if (keystate[KEY_INPUT_LEFT]) {
			vax = -speed;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			vax = +speed;
		}

		rcA.pos.x += vax;
		rcA.pos.y += vay;
		cA.center.x += vax;
		cA.center.y+= vay;

		if (keystate[KEY_INPUT_W]) {
			rcB.pos.y-= speed;
		}
		if (keystate[KEY_INPUT_S]) {
			rcB.pos.y += speed;
		}
		if (keystate[KEY_INPUT_A]) {
			rcB.pos.x -= speed;
		}
		if (keystate[KEY_INPUT_D]) {
			rcB.pos.x += speed;
		}

		if (keystate[KEY_INPUT_Z]) {
			fan.AddAngle(0.005f);
		}

		if (keystate[KEY_INPUT_X]) {
			fan.AddAngle(-0.005f);
		}

		unsigned int color = 0xffffff;//白
		if (IsHitCircles(cA, cB)) {
			color = 0xffaaaa;//ピンク
			

		}

		

		//円B
		DrawCircle(cB.center.x, cB.center.y, cB.radius, color);

		//DrawCircleGauge(320, 240, 50.0, circleH, 0.0);

		if (IsHitCircleAndFan(cA, fan)) {
			color = 0xffaaaa;
			fan.Draw(color);
		}
		else {
			fan.Draw();
		}
		
		
		//円A
		DrawCircle(cA.center.x, cA.center.y, cA.radius, color);

		//DrawBox(rcA.Left(), rcA.Top(), 
		//	rcA.Right(), rcA.Bottom(), 
		//	color, true);

		//DrawBox(rcB.Left(), rcB.Top(),
		//	rcB.Right(), rcB.Bottom(),
		//	color, true);

		DxLib::ScreenFlip();
	}

	DxLib::DxLib_End();

	return 0;
}
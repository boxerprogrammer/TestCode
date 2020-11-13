#include<DxLib.h>
#include"Geometry.h"
#include<cmath>


/// <summary>
/// �����`�̓����蔻���Ԃ�
/// </summary>
/// <param name="rcA">��`A</param>
/// <param name="rcB">��`B</param>
/// <returns>
///  rcA��rcB���d�Ȃ��Ă�:true
///  rcA��rcB���d�Ȃ��ĂȂ�:false
///</returns>
bool IsHitAABB(const Rect& rcA,const Rect& rcB) {
	Vector2 diff= rcA.Center() - rcB.Center();
	
	return (abs(diff.x)<(rcA.width+rcB.width)/2&&
		abs(diff.y) < (rcA.height + rcB.height) / 2);
}
bool IsHitCircles(const Circle& cA, const Circle& cB) {
	float xdiff = cB.center.x - cA.center.x;
	float ydiff = cB.center.y - cA.center.y;
	return hypot(xdiff,ydiff)<=cA.r+cB.r;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText(L"�~�̓����蔻��");
	if (DxLib::DxLib_Init()) {
		return -1;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	Rect rcA = { 0,0,100,100 };
	Rect rcB = { 300,300,80,80 };

	Circle cA = { { 100,100},150.0f };
	Circle cB = { { 300,300},50.0f };

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

		rcA.left += vax;
		rcA.top += vay;
		cA.center.x += vax;
		cA.center.y+= vay;

		if (keystate[KEY_INPUT_W]) {
			rcB.top -= speed;
		}
		if (keystate[KEY_INPUT_S]) {
			rcB.top += speed;
		}
		if (keystate[KEY_INPUT_A]) {
			rcB.left -= speed;
		}
		if (keystate[KEY_INPUT_D]) {
			rcB.left += speed;
		}

		unsigned int color = 0xffffff;//��
		if (IsHitCircles(cA, cB)) {
			color = 0xffaaaa;//�s���N
			
			//�����߂��R�[�h
			//�܂���A��B�ւ̃x�N�g�����v�Z
			auto N=cB.center - cA.center;
			//�߂荞�ݗʂ̌v�Z
			auto overlap = cB.r + cA.r - N.Magnitude();
			N.Normalize();
			cA.center -= N * overlap * 0.5f;
			cB.center += N * overlap * 0.5f;

		}

		//�~A
		DrawCircle(cA.center.x, cA.center.y, cA.r, color);

		//�~B
		DrawCircle(cB.center.x, cB.center.y, cB.r, color);

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
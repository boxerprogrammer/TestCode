#include<DxLib.h>
#include<cmath>

bool IsHitCircle(int ax, int ay, int bx, int by,float ar,float br) {

	return hypot(bx-ax,by-ax)<=ar+br;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	SetWindowText("円の当たり判定サンプル");
	ChangeWindowMode(true);
	SetGraphMode(640, 480, 32);
	DxLib_Init();

	SetDrawScreen(DX_SCREEN_BACK);

	const auto color_normal = GetColor(255, 255, 255);
	const auto color_hit = GetColor(255, 192, 192);

	

	int ax=50;
	int ay=50;

	int bx = 320;
	int by = 240;

	const float radiusA = 50;
	const float radiusB = 50;
	auto col = color_normal;

	char keystate[256];

	while (ProcessMessage() == 0) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_RIGHT]) {
			ax += 2;
		}
		if (keystate[KEY_INPUT_LEFT]) {
			ax -= 2;
		}

		if (keystate[KEY_INPUT_UP]) {
			ay -= 2;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			ay += 2;
		}

		//hypot(bx - ax, by - ay) <= radiusA + radiusB
		if (IsHitCircle(ax,ay,bx,by,radiusA,radiusB)) {
			col = color_hit;
			//当たっているので押し戻す…だが、これが難しい。
			//当たるのははっきり言って児戯に等しいっ…！！
			//だが円を押し戻す…これが難しいっ…！
			//さて、どう押し戻すか。

			//まずうちさぁ、作用反作用あんだけど…押し戻してかない？
			//ああ～、いいっすねえ。どの方向に押し戻しますか？
			//法線方向って…知ってるかな？法線っていうのはね、3Dにおける
			//平面に垂直なベクトル…とか、2Dにおける曲線や特定の直線に対して
			//直交しているベクトルを「法線ベクトル」っていうんだよ。
			//難しいねえ。
			
			//では、円どうしの当たり判定の場合法線を求めるのは簡単である。
			//なぜなら中心同士を結ぶ線…それが法線になるからだ。
			
			//で、その法線とやらをどう使えばいいのだろうか？要は力の法線方向
			//成分が作用反作用によって失われてしまうため元のベクトルの
			//法線方向成分が失われると考えられる。法線方向への射影が欲しいのだ。
			//法線方向への射影は法線ベクトルを正規化し、それと元のベクトルの
			//内積をとることで得られるつまりV・Nだが、これが奪われる「量」
			//なので、計算式的にはV-(V・N)Nとなる。
			//さぁ、プログラムだこの野郎。ちなみに計算自体は上記の計算だが
			//実際に速度を補正すると同時に座標をめり込んでない状況にしないと
			//物理法則こわれちゃ↑ーうのである。気を付けよう。


			//

		}
		else {
			col = color_normal;
		}

		DrawCircle(ax, ay, radiusA, col, true);
		DrawCircle(bx, by, radiusB, col, true);

		ScreenFlip();
	}


	return 0;
}

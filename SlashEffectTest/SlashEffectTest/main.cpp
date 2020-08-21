#include<DxLib.h>
#include"Geometry.h"
#include<cmath>
#include<vector>
#include<algorithm>


using namespace std;
namespace {
	int bgH;
	VECTOR V2V(const Vector2& v) {
		return VGet(v.x, v.y, 0);
	}
	/// <summary>
	/// 割り切れるかどうかを返す
	/// </summary>
	/// <typeparam name="T">整数型に限る</typeparam>
	/// <param name="val">チェックしたい値</param>
	/// <param name="div">モジュロ値</param>
	/// <returns>true:割り切れる false:割り切れない</returns>
	template<typename T>
	inline bool IsDivisible(const T& val, const T& div) {
		static_assert(std::is_integral<T>::value, "整数型に限る");
		return val % div == 0;
	}

}
/// <summary>
/// 斬撃エフェクト構造体(扇形を改造)
/// 不変条件：v1→v2は時計回りである事
///          :v1の大きさとv2の大きさは同じである事
/// </summary>
struct Slash {
	Position2 center; //中心座標
	Vector2 v1; //扇形の端ベクトル①
	Vector2 v2; //扇形の端ベクトル②
	Slash() :center(0, 0), v1(0, 0), v2(0, 0) {}//無効です
	Slash(const Position2& p, const Vector2& inv1, const Vector2& inv2);
	Slash(const Position2& p, float r, float angle);
	Slash(const Position2& p, const Vector2& inv, float angle);
	void Draw(unsigned int color = 0xffffff);
	float Radius()const;
	void AddAngle(float angle);
	float GetAngle()const;
};
Slash::Slash(const Position2& p, const Vector2& inv1, const Vector2& inv2) :
	center(p), v1(inv1), v2(inv2) {

}
Slash::Slash(const Position2& p, float r, float angle) : center(p) {
	v1 = Vector2(r, 0);
	v2 = Vector2(r * cos(angle), r * sin(angle));
}
Slash::Slash(const Position2& p, const Vector2& inv, float angle) :
	center(p), v1(inv) {
	v2 = RotateMat(angle) * v1;
}
float
Slash::GetAngle()const {
	return GetAngle2Vector(v1, v2);
}

void
Slash::AddAngle(float angle) {
	float tmpAngle = GetAngle2Vector(v1, v2);

	v2 = RotateMat(tmpAngle + angle) * v1;
}


float
Slash::Radius()const {
	return v1.Magnitude();
}
int normalH=-1;
// ピクセルシェーダーを読み込む
int pshandle;// = LoadPixelShader("VertexShaderTestPS.pso");
void
Slash::Draw(unsigned int color) {
	// ２Ｄ描画に使用する頂点データ型
	//struct VERTEX2D
	//{
	//	VECTOR pos;
	//	float rhw;
	//	COLOR_U8 dif;
	//	float u, v;
	//};
	constexpr float min_angle = DX_PI_F / 36.0f;//だいたい5度くらい
	float angle = GetAngle2Vector(v1, v2);

	int triangles_num = (int)ceil(angle / min_angle);
	++triangles_num;
	vector<VERTEX2DSHADER> v(2 * triangles_num);

	for (auto& e : v) {
		e.rhw = 1.0f;
		e.dif = GetColorU8(color >> 16 & 0xff, color >> 8 & 0xff, color & 0xff, 255);
		e.u = 0;
		e.v = 0;
	}
	float r = v1.Magnitude();
	float dr = r / (float)triangles_num;
	int ver = GetUseDirect3DVersion();
	auto vstart = v1;
	for (size_t i = 0; i < triangles_num-1; ++i) {
		auto p = center + vstart.Normalized() * r;
		v[i * 2 + 0].pos = V2V(p);
		v[i * 2 + 0].u = (p.x / 800.0f)+cos((float)i/(float)triangles_num)/20.0f;
		v[i * 2 + 0].v = (p.y / 600.0f)+sin((float)i / (float)triangles_num) / 20.0f;
		v[i * 2 + 0].dif.r = 0;
		v[i * 2 + 0].dif.g = 0;
		v[i * 2 + 0].dif.b = 0;
		v[i * 2 + 0].dif.a = 0;
		p = center + vstart;
		v[i * 2 + 1].pos = V2V(p);
		v[i * 2 + 1].u = (p.x / 800.0f) + cos((float)(i+2) / (float)triangles_num) / 50.0f;
		v[i * 2 + 1].v = (p.y / 600.0f) + sin((float)(i+2) / (float)triangles_num) / 50.0f;
		v[i * 2 + 1].dif.r = 192;
		v[i * 2 + 1].dif.g = 192;
		v[i * 2 + 1].dif.b = 255;
		v[i * 2 + 1].dif.a = 255;
		vstart = RotateMat(min_angle) * vstart;
		r -= dr;
	}
	auto idx=(triangles_num - 1)*2;
	auto pos = center + v2.Normalized() * r;
	v[idx].pos = V2V(pos);
	v[idx].u = pos.x / 800.0f;
	v[idx].v = pos.y / 600.0f;
	v[idx].dif.r = 168;
	v[idx].dif.g = 168;
	v[idx].dif.b = 255;
	v[idx].dif.a = 192;
	pos = center + v2;
	v[idx + 1].pos = V2V(pos);
	v[idx + 1].u = pos.x / 800.0f;
	v[idx + 1].v = pos.y / 600.0f;
	v[idx + 1].dif.r = 255;
	v[idx + 1].dif.g = 255;
	v[idx + 1].dif.b = 255;
	v[idx + 1].dif.a = 255;

	// ２Ｄポリゴンを描画する
	// 使用するピクセルシェーダーをセット
	SetUseTextureToShader(0, bgH);
	SetUseTextureToShader(1, normalH);
	SetUsePixelShader(pshandle);
	DrawPrimitive2DToShader(v.data(), v.size(), DX_PRIMTYPE_TRIANGLESTRIP);

	//DrawPolygon2D(v.data(), v.size() / 3, DX_NONE_GRAPH, true);

	Position2 p = center + v1;
	DrawLine(center.x, center.y, p.x, p.y, 0xff8888);
	p = center + v2;
	DrawLine(center.x, center.y, p.x, p.y, 0xff8888);
	
}


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
	DxLib::SetGraphMode(800, 600,32);
	DxLib::SetWindowText(L"斬撃エフェクト");
	if (DxLib::DxLib_Init()) {
		return -1;
	}
	bgH=LoadGraph(L"ss.png");
	auto charaH = LoadGraph(L"chara.png");
	normalH= LoadGraph(L"normal.png");
	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	Rect rcA = { {0,0},100,100 };
	Rect rcB = { {300,300},80,80 };

	Circle cA = { { 250.0f,50.0f},50.0f };
	Circle cB = { { 300.0f,300.0f},50.0f };

	Slash slash(Position2(316, 459), Vector2(-125, -125), DX_PI_F*2.0f / 3.0f);
	pshandle = LoadPixelShader(L"testps.pso");

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
			slash.AddAngle(0.05f);
		}

		if (keystate[KEY_INPUT_X]) {
			slash.AddAngle(-0.05f);
		}

		unsigned int color = 0xffffff;//白
		if (IsHitCircles(cA, cB)) {
			color = 0xffaaaa;//ピンク
			

		}

		
		DrawGraph(0, 0, bgH, false);


		slash.Draw();
		DrawRotaGraph(300, 480, 3.0f, 0.0f, charaH, true);

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
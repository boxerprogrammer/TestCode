#include<DxLib.h>
#include<cassert>
#include<array>
#include<list>
#include<algorithm>
#include"Geometry.h"
using namespace std;

namespace mylib {
	void DrawExtendGraph(int x1, int y1, int x2,int y2,int handle, int psH, bool alpha, int secondH = -1) {
		std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
		//4頂点に必要な情報を代入していく
		//必要な情報→座標、頂点カラー、UV
		//ただし、rhwは必ず1.0にする必要があります。

		for (auto& v : vertices) {
			v.rhw = 1.0f;//これは必ず1
			v.dif = { 255,255,255,255 };//これは白色にしておく
		}

		int w, h;
		DxLib::GetGraphSize(handle, &w, &h);

		//並べ方はZの字になるように
		//頂点情報(左上)
		vertices[0].pos.x = x1 ;
		vertices[0].pos.y = y1;
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;
		vertices[0].su = (float)x1/(float)w;
		vertices[0].sv = (float)y1/(float)h;

		//頂点情報(右上)
		vertices[1].pos.x = x2;
		vertices[1].pos.y = y1 ;
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;
		vertices[1].su = (float)x2/(float)w;
		vertices[1].sv = (float)y1/(float)h;

		//頂点情報(左下)
		vertices[2].pos.x = x1;
		vertices[2].pos.y = y2;
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;
		vertices[2].su = (float)x1/(float)w;
		vertices[2].sv = (float)y2/(float)h;

		//頂点情報(右下)
		vertices[3].pos.x = x2;
		vertices[3].pos.y = y2;
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;
		vertices[3].su = (float)x2/(float)w;
		vertices[3].sv = (float)y2/(float)h;

		//テクスチャを指定
		DxLib::SetUseTextureToShader(0, handle);
		if (secondH >= 0) {
			DxLib::SetUseTextureToShader(1, secondH);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		//シェーダを指定
		DxLib::SetUsePixelShader(psH);

		DxLib::DrawPrimitive2DToShader(vertices.data(), vertices.size(),
			DX_PRIMTYPE_TRIANGLESTRIP);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	void DrawRotaGraph(int x, int y, float scale,float angle,int handle, int psH, bool alpha, int secondH = -1) {
		std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
		//4頂点に必要な情報を代入していく
		//必要な情報→座標、頂点カラー、UV
		//ただし、rhwは必ず1.0にする必要があります。

		for (auto& v : vertices) {
			v.rhw = 1.0f;//これは必ず1
			v.dif = { 255,255,255,255 };//これは白色にしておく
		}

		int w, h;
		DxLib::GetGraphSize(handle, &w, &h);

		//並べ方はZの字になるように
		//頂点情報(左上)
		vertices[0].pos.x = x - w * 0.5*scale;
		vertices[0].pos.y = y - h * 0.5 * scale;
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;

		//頂点情報(右上)
		vertices[1].pos.x = x + w * 0.5 * scale;
		vertices[1].pos.y = y - h * 0.5 * scale;
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;

		//頂点情報(左下)
		vertices[2].pos.x = x - w * 0.5 * scale;
		vertices[2].pos.y = y + h * 0.5 * scale;
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;

		//頂点情報(右下)
		vertices[3].pos.x = x + w * 0.5 * scale;
		vertices[3].pos.y = y + h * 0.5 * scale;
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;

		//テクスチャを指定
		DxLib::SetUseTextureToShader(0, handle);
		if (secondH >= 0) {
			DxLib::SetUseTextureToShader(1, secondH);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		//シェーダを指定
		DxLib::SetUsePixelShader(psH);

		DxLib::DrawPrimitive2DToShader(vertices.data(), vertices.size(),
			DX_PRIMTYPE_TRIANGLESTRIP);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	void DrawGraph(int x, int y, int handle,int psH, bool alpha,int secondH=-1) {
		std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
		//4頂点に必要な情報を代入していく
		//必要な情報→座標、頂点カラー、UV
		//ただし、rhwは必ず1.0にする必要があります。

		for (auto& v : vertices) {
			v.rhw = 1.0f;//これは必ず1
			v.dif = { 255,255,255,255 };//これは白色にしておく
		}

		int w, h;
		DxLib::GetGraphSize(handle, &w, &h);

		//並べ方はZの字になるように
		//頂点情報(左上)
		vertices[0].pos.x = x;
		vertices[0].pos.y = y;
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;

		//頂点情報(右上)
		vertices[1].pos.x = x+w;
		vertices[1].pos.y = y;
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;

		//頂点情報(左下)
		vertices[2].pos.x = x;
		vertices[2].pos.y = y+h;
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;

		//頂点情報(右下)
		vertices[3].pos.x = x+w;
		vertices[3].pos.y = y+h;
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;
		
		//テクスチャを指定
		DxLib::SetUseTextureToShader(0, handle);
		if (secondH >= 0) {
			DxLib::SetUseTextureToShader(1, secondH);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		//シェーダを指定
		DxLib::SetUsePixelShader(psH);

		DxLib::DrawPrimitive2DToShader(vertices.data(),vertices.size(),
										DX_PRIMTYPE_TRIANGLESTRIP);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() != 0) {
		return -1;
	}

	//SetDrawMode(DX_DRAWMODE_BILINEAR);
	SetDrawMode(DX_DRAWMODE_NEAREST);

	int crackH = LoadGraph(L"img/crack_nm.png");
	assert(crackH >= 0);

	int bgH = LoadGraph(L"img/bg.png");
	assert(bgH >= 0);

	int handle = LoadGraph(L"img/fukuro.png");
	assert(handle >= 0);

	int normalH = LoadGraph(L"img/fukuro_n.png");
	assert(normalH >= 0);

	int secondH = LoadGraph(L"img/pattern.png");
	assert(secondH >= 0);

	int dotTexH = LoadGraph(L"img/dotpic.png");
	assert(dotTexH >= 0);
	int dotTexNormH = LoadGraph(L"img/dotpic_n.png");
	assert(dotTexNormH >= 0);

	int psH = LoadPixelShader(L"firstps.pso");
	assert(psH >= 0);

	int normalMapPSH = LoadPixelShader(L"NormalMap.pso");
	assert(normalMapPSH >=0);

	int crackPSH = LoadPixelShader(L"CrackPS.pso");
	assert(crackPSH >= 0);

	int wavePSH = LoadPixelShader(L"WavePS.pso");
	assert(wavePSH >= 0);

	int normalMapTexH = LoadGraph(L"img/fukuro_n.png");
	assert(normalMapTexH >= 0);

	int explosionH = LoadGraph(L"img/explosion.png");
	assert(explosionH >= 0);
	int explosionSEH = LoadSoundMem(L"se/explode.wav");
	assert(explosionSEH >= 0);

	int liquidPSH = LoadPixelShader(L"LiquidGlass.pso");
	assert(liquidPSH >= 0);

	SetDrawScreen(DX_SCREEN_BACK);

	//定数バッファに使う用の型(16バイトアライメントになるように)
	struct CBufferStruct {
		float time;
		float x;
		float y;
		float r;
	};

	//定数バッファを作ります
	//①まずグラボ上に定数バッファ用のメモリを確保します
	//確保したらDxLibのハンドルが返されるので、それを受け取ります
	//ここで確保したのはあくまでもGPU上のメモリです
	int cbuffH = DxLib::CreateShaderConstantBuffer(sizeof(CBufferStruct));
	CBufferStruct* pCBufff = (CBufferStruct*)DxLib::GetBufferShaderConstantBuffer(cbuffH);

	//レンダーターゲット(Drawの書き込み先)
	//を作っておく
	auto RT = MakeScreen(640, 480);
	auto RT2 = MakeScreen(640, 480);
	auto RT3 = MakeScreen(640, 480);

	auto lastTime = GetTickCount64();
	int frame = 0;
	bool wave = false;
	int lastMouseInput = 0;
	
	struct Exploder {
		int frame = 0;
		int x = 0;
		int y = 0;
	};

	std::list<Exploder> exploders;
	constexpr int exp_one_size = 100;
	constexpr int exp_x_num = 10;
	constexpr int exp_y_num = 8;

	Rect liquid( 320, 240 ,100,100);

	while (ProcessMessage() != -1) {
		SetDrawScreen(RT);
		ClearDrawScreen();

		frame = (frame + 1) % 361;
		pCBufff->time = sin((static_cast<float>(frame) / 360.0f) * 2.0f * DX_PI_F);

		//値を変更したら忘れずに呼び出す
		DxLib::UpdateShaderConstantBuffer(cbuffH);

		DrawGraph(0, 0, bgH, true);
		//作った定数バッファをGPUに割り当てる
		DxLib::SetShaderConstantBuffer(cbuffH, DX_SHADERTYPE_PIXEL, 4);
		//mylib::DrawGraph(100, 50, handle,psH, true,secondH);
		//mylib::DrawGraph(100, 50, handle, normalMapPSH, true, normalMapTexH);
		mylib::DrawRotaGraph(320, 240,1.0f,0.0f, handle, normalMapPSH, true, normalH);

		for (auto& exploder : exploders) {
			++exploder.frame;
			int srcX = (exploder.frame % exp_x_num) * exp_one_size;
			int srcY = (exploder.frame / exp_x_num) * exp_one_size;
			DrawRectRotaGraph(exploder.x, exploder.y,srcX,srcY,
				exp_one_size,exp_one_size,3.0f,0.0f,explosionH,true);
		}
		exploders.remove_if([](const Exploder& e) {
			return e.frame > exp_x_num * exp_y_num;
			});

		

		SetDrawScreen(RT2);
		mylib::DrawGraph(0, 0, RT,crackPSH, false,crackH);

		SetDrawScreen(DX_SCREEN_BACK);
		
		auto minput = GetMouseInput();
		int mx, my;
		GetMousePoint(&mx, &my);
		if ((minput & MOUSE_INPUT_LEFT) && !(lastMouseInput&&MOUSE_INPUT_LEFT)) {

			pCBufff->x = mx;
			pCBufff->y = my;
			pCBufff->r = 1;
			wave = true;
			PlaySoundMem(explosionSEH, DX_PLAYTYPE_BACK);
			exploders.push_back({ 0, mx, my });
		}else{
			liquid.pos.x = mx;
			liquid.pos.y = my;
		}
		if (wave) {
			pCBufff->r += 10.0f;
			if (pCBufff->r > 2000.0f) {
				wave = false;
			}
		}
		lastMouseInput = minput;
		
		mylib::DrawGraph(0, 0, RT2,wavePSH, false);

		GraphFilterBlt( RT2,RT3, DX_GRAPH_FILTER_GAUSS, 16, 1400 ) ;

		mylib::DrawExtendGraph(liquid.Left(), liquid.Top(), liquid.Right(), liquid.Bottom(), RT3, liquidPSH,true, RT2);
		DrawFormatString(10, 10, 0xfffffff, L"FPS:%2.2f", GetFPS());

		auto diffTime = GetTickCount64() - lastTime;
		if (diffTime < 16) {
			Sleep(16 - diffTime);
		}
		lastTime = GetTickCount64();
		ScreenFlip();
	}
	DxLib::DeleteShaderConstantBuffer(cbuffH);
	DxLib_End();
	return 0;
}
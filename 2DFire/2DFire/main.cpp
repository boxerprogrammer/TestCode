#include<DxLib.h>
#include<array>
#include<algorithm>

using namespace std;


void MyDrawGraph(int x, int y, int width,int height,int imgHandle,int secondImg,int thirdImg,int psHandle) {
	//int width, height;
	//GetGraphSize(imgHandle, &width, &height);
	array<VERTEX2DSHADER, 4> verts;

	for (auto& v : verts) {
		v.rhw = 1.0;
		v.dif=DxLib::GetColorU8(255,255,255,255);//ディフューズ
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);//スペキュラ
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	//左上	
	verts[0].pos.x = x;
	verts[0].pos.y = y;
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;
	//右上
	verts[1].pos.x = x+width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;
	//左下
	verts[2].pos.x = x;
	verts[2].pos.y = y+height;
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;
	//右下
	verts[3].pos.x = x+width;
	verts[3].pos.y = y+height;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	//DxLib::SetDrawAlphaTest(true);
	int alphamode, alphaparam;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	GetDrawAlphaTest(&alphamode, &alphaparam);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);
	SetUsePixelShader(psHandle);
	SetUseTextureToShader(0, imgHandle);
	SetUseTextureToShader(1, secondImg);
	SetUseTextureToShader(2, thirdImg);
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib_Init();
	
	int img=LoadGraph(L"img.png");
	int fire = LoadGraph(L"fire.png");
	int fireNorm = LoadGraph(L"fire_normal.png");
	int fireStencil = LoadGraph(L"fire_stencil.png");
	int crackNorm = LoadGraph(L"crack_nm.png");
	int bg = LoadGraph(L"nightbg.jpg");
	int ps=LoadPixelShader(L"fire.pso");
	int psPP = LoadPixelShader(L"PostProcess.pso");
	
	//①定数バッファの確保(VRAM上)
	int cbuff=DxLib::CreateShaderConstantBuffer(sizeof(float) * 4);
	//グラボのメモリは実は直接いじれない。
	//RAM上のメモリでシミュレートしてる
	float* threshold=static_cast<float*>(DxLib::GetBufferShaderConstantBuffer(cbuff));
	float tekito = 0.0f;
	float angle = 0.0f;
	float voffset = 0.0f;
	float uscale = 1.0f;
	char keystate[256];
	int sw, sh;
	GetDrawScreenSize(&sw,&sh);
	int handleForPP=DxLib::MakeScreen(sw, sh,1);
	int overPP = DxLib::MakeScreen(sw, sh,1);
	int shakeTime = 0;
	bool lastLRKey = false;
	while (ProcessMessage() != -1) {
		SetDrawScreen(handleForPP);
		ClearDrawScreen();
		SetDrawScreen(overPP);
		ClearDrawScreen();
		SetDrawScreen(handleForPP);
		
		angle += 0.01f;
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_UP]) {
			tekito -= 0.01f;
		}
		if (keystate[KEY_INPUT_DOWN]) {
			tekito += 0.01f;
		}
		if (keystate[KEY_INPUT_SPACE]) {
			shakeTime = 60;
		}
		if (shakeTime > 0) {
			--shakeTime;
		}
		if (keystate[KEY_INPUT_RIGHT]) {
			if(!lastLRKey)
				uscale=std::clamp(uscale + 1.0, 1.0, 10.0);
			lastLRKey = true;
		}
		else if(keystate[KEY_INPUT_LEFT]){
			if (!lastLRKey)
				uscale = std::clamp(uscale - 1.0, 1.0, 10.0);
			lastLRKey = true;
		}
		else {
			lastLRKey = false;
		}
		voffset += 0.01f;
		
		DrawGraph(-50, -50, bg, true);
		threshold[0] = uscale;
		threshold[1] = angle;
		threshold[2] = voffset;
		threshold[3] = uscale;
		UpdateShaderConstantBuffer(cbuff);
		SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);
		auto result = SetRenderTargetToShader(0, handleForPP);
		result = SetRenderTargetToShader(1, overPP);
		MyDrawGraph(50, 250,200*uscale,200, fire,fireNorm,fireStencil, ps);
		SetRenderTargetToShader(1, -1);

		GraphFilter(overPP, DX_GRAPH_FILTER_GAUSS, 16, 1400);
		

		SetDrawScreen(DX_SCREEN_BACK);
		
		DrawGraph(0, 0, handleForPP,false);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawGraph(0, 0, overPP, false);
		DrawGraph(0, 0, overPP, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		//MyDrawGraph(0, 0, overPP, crackNorm, -1, psPP);
		ScreenFlip();
	}
	DxLib_End();
}
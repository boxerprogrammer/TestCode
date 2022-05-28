#include<DxLib.h>
#include<array>

using namespace std;


void MyDrawGraph(int x, int y, int imgHandle,int secondImg,int thirdImg,int psHandle) {
	int width, height;
	GetGraphSize(imgHandle, &width, &height);
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
	int ptn = LoadGraph(L"carbon_pattern.png");
	int normImg = LoadGraph(L"img_n2.png");
	int bg = LoadGraph(L"bg.png");
	int ps=LoadPixelShader(L"ReversePS.pso");
	int psPP = LoadPixelShader(L"PostProcess.pso");
	int imgPP = LoadGraph(L"crack_nm.png");
	//①定数バッファの確保(VRAM上)
	int cbuff=DxLib::CreateShaderConstantBuffer(sizeof(float) * 4);
	//グラボのメモリは実は直接いじれない。
	//RAM上のメモリでシミュレートしてる
	float* threshold=static_cast<float*>(DxLib::GetBufferShaderConstantBuffer(cbuff));
	float tekito = 0.0f;
	float angle = 0.0f;
	char keystate[256];
	int sw, sh;
	GetDrawScreenSize(&sw,&sh);
	int handleForPP=DxLib::MakeScreen(sw, sh);
	int shakeTime = 0;
	while (ProcessMessage() != -1) {
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
		ClearDrawScreen();
		DrawGraph(-50, -50, bg, true);
		threshold[0] = tekito;
		threshold[1] = angle;
		UpdateShaderConstantBuffer(cbuff);
		SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);
		MyDrawGraph(50, 50, img,ptn,normImg, ps);


		SetDrawScreen(DX_SCREEN_BACK);
		MyDrawGraph(0, 0, handleForPP, imgPP, -1, psPP);

		ScreenFlip();
	}
	DxLib_End();
}
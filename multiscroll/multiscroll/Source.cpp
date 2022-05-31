#include<DxLib.h>
#include<array>

using namespace std;


void MyDrawGraph(int x, int y, int imgHandle, int secondImg, int thirdImg, int psHandle) {
	int width, height;
	GetGraphSize(imgHandle, &width, &height);
	array<VERTEX2DSHADER, 4> verts;

	for (auto& v : verts) {
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);//ディフューズ
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
	verts[1].pos.x = x + width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;
	//左下
	verts[2].pos.x = x;
	verts[2].pos.y = y + height;
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;
	//右下
	verts[3].pos.x = x + width;
	verts[3].pos.y = y + height;
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

void MyDrawExtendGraph(int x1, int y1, int x2,int y2,int imgHandle, int secondImg, int thirdImg, int psHandle) {
	int width, height;
	GetGraphSize(imgHandle, &width, &height);
	array<VERTEX2DSHADER, 4> verts;

	for (auto& v : verts) {
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);//ディフューズ
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);//スペキュラ
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	//左上	
	verts[0].pos.x = x1;
	verts[0].pos.y = y1;
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;
	//右上
	verts[1].pos.x = x2;
	verts[1].pos.y = y1;
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;
	//左下
	verts[2].pos.x = x1;
	verts[2].pos.y = y2;
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;
	//右下
	verts[3].pos.x = x2;
	verts[3].pos.y = y2;
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

	SetGraphMode(640, 480, 32);



	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	auto scrH=MakeScreen(384, 288);
	auto normalH = LoadGraph("normal.png");
	auto ps = LoadPixelShader("waterreflection.pso");
	auto farBgH = LoadGraph("img/background.png");
	auto midBgH=LoadGraph("img/middleground.png");
	auto grdH = LoadGraph("img/sliced-tileset/ground.png");
	auto bgm= PlayMusic("chinke.mp3", DX_PLAYTYPE_BACK);
	
	auto jumpS=LoadSoundMem("jump.wav", DX_PLAYTYPE_BACK);
	int playerH[6];
	LoadDivGraph("img/bearded-walk.png", 6, 6, 1, 40, 47, playerH);

	int houseH[3];
	houseH[0] = LoadGraph("img/props-sliced/house-a.png");
	houseH[1] = LoadGraph("img/props-sliced/house-b.png");
	houseH[2] = LoadGraph("img/props-sliced/house-c.png");

	//①定数バッファの確保(VRAM上)
	int cbuff = DxLib::CreateShaderConstantBuffer(sizeof(float) * 4);
	//グラボのメモリは実は直接いじれない。
	//RAM上のメモリでシミュレートしてる
	float* cbuffer = static_cast<float*>(DxLib::GetBufferShaderConstantBuffer(cbuff));
	float tekito = 0.0f;
	float angle = 0.0f;
	SetTextureAddressModeUV(DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP);
	int scroll = 0;
	auto houseX = 384;
	auto rightX = 384;
	auto houseIdx = rand() % 3;
	constexpr int y_offset = -96;
	char keystate[256];
	bool turn = false;
	float vy=0.0f;
	
	constexpr int ground_line = 288 - 50;
	float y = ground_line;
	bool jumping = false;
	while (ProcessMessage() == 0) {
		SetDrawScreen(scrH);
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		DrawGraph(-(scroll/3%384), 0, farBgH,false);
		DrawGraph(-(scroll/3%384)+384, 0, farBgH, false);

		DrawGraph(-(scroll/2 % 384), 0, midBgH, true);
		DrawGraph(-(scroll/2 % 384) + 384, 0, midBgH, true);

		DrawTile(-(scroll*2%32), 288 - 32, 384, 288, 1, 1, 1.0f, 0.0f, grdH, true);

		houseX = rightX - scroll;

		int w, h;
		GetGraphSize(houseH[houseIdx], &w, &h);
		DrawGraph(houseX,288-32-h, houseH[houseIdx],true);

		if (houseX + w < 0) {
			houseIdx = rand() % 3;
			rightX = scroll + 384;
		}

		DrawRotaGraph(180, y, 1.0,0.0,playerH[scroll / 4 % 6] ,true,turn);
		if (keystate[KEY_INPUT_RIGHT]) {
			++scroll;
			cbuffer[0] += 0.001;
			turn = false;
		}
		if (keystate[KEY_INPUT_LEFT]) {
			--scroll;
			cbuffer[0] -= 0.001;
			turn = true;
		}
		if (scroll < 0) {
			scroll += 384;
		}
		if (keystate[KEY_INPUT_Z]) {
			if (!jumping) {
				vy = -10;
				jumping = true;
				PlaySoundMem(jumpS, DX_PLAYTYPE_BACK);
			}
		}
		vy += 0.5;
		y += vy;
		if (y > ground_line) {
			y = ground_line;
			vy = 0.0f;
			jumping = false;
		}
		angle += 0.025f;
		cbuffer[1] = angle;
		UpdateShaderConstantBuffer(cbuff);
		SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);

		SetDrawScreen(DX_SCREEN_BACK);
		MyDrawExtendGraph(0, 480+32, 640, 480 + y_offset-16, scrH, normalH, -1, ps);
		DrawExtendGraph(0, y_offset, 640, y_offset+480, scrH, false);
		
		ScreenFlip();
	}
	DxLib_End();
}
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

	int bloodDrop = LoadGraph("img/bloodDrop.png");

	//①定数バッファの確保(VRAM上)
	int cbuff = DxLib::CreateShaderConstantBuffer(sizeof(float) * 4);
	//グラボのメモリは実は直接いじれない。
	//RAM上のメモリでシミュレートしてる
	float* cbuffer = static_cast<float*>(DxLib::GetBufferShaderConstantBuffer(cbuff));
	float tekito = 0.0f;
	float time = 0.0f;
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

	auto rt = MakeScreen(640, 480);

	int frame = 0;//フレーム
	float amplitude = 0.0f;//振幅
	struct Vector2 {
		float x, y;
	};
	using Position2 = Vector2;
	struct Blood {
		Position2 pos = {};
		float g = 1.0f;
		float crate = 1.0f;
		float r = 8.0f;
	};
	array<Blood, 48> bloodPoints;
	auto bloodRT = MakeScreen(640, 480,1);
	for (auto& b : bloodPoints) {
		b.pos.y -= (rand() % 100);
		b.pos.x	=(float)(rand() % 640);
		b.g = 0.5f + (float)(rand() % 10) / 10.0f;
		b.crate = 0.6f + (float)(rand() % 10) / 10.0f;
		b.r = 16.0f*b.crate;
	}
	while (ProcessMessage() == 0) {
		SetDrawScreen(bloodRT);
		SetUsePixelShader(-1);
		SetUseTextureToShader(0, -1);
		SetUseTextureToShader(1, -1);
		SetUseTextureToShader(2, -1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		for (auto& b : bloodPoints) {
			b.pos.y += b.g;
			b.r = std::max(b.r-0.05f,0.1f);
			DrawCircleAA(b.pos.x, b.pos.y, b.r, 16, 0xaa0000, true);
		}
		


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
		int pw, ph;
		float a = -1.5;//傾き
		
		GetGraphSize(playerH[scroll / 4 % 6], &pw, &ph);
		int shadowLen = ph *0.75;
		DxLib::SetDrawBright(0, 0, 0);

		int diffY = ground_line - y;
		int diffY2 = diffY / 2;
		int shadowY = ground_line - diffY2+24;
		int shadowX = diffY * a;
		int toWall = 12;
		int sign = turn ? -1 : 1;
		//影①
		DrawRectModiGraph(
			//180-pw/2+ shadowLen*a, y+24-shadowLen,//左上
			//180 + pw / 2 + shadowLen * a, y + 24 - shadowLen,//右上
			shadowX +180 + toWall * a  - sign*pw / 2, shadowY- toWall,//左上
			shadowX + 180 + toWall * a + sign * pw / 2, shadowY- toWall,//右上
			shadowX + 180 + sign * pw / 2 , shadowY,//右下
			shadowX + 180 - sign * pw / 2 , shadowY,//左下
			0, ph / 2,pw,ph/2,
			playerH[scroll / 4 % 6], true);
		//影2
		DrawRectModiGraph(
			toWall * a + 180 + shadowX - sign * pw / 2, shadowY-toWall-24,//左上
			toWall * a + 180 + shadowX + sign * pw / 2, shadowY - toWall - 24,//右上
			toWall * a + 180 + shadowX + sign * pw / 2, shadowY - toWall,//右下
			toWall * a + 180 + shadowX - sign * pw / 2, shadowY - toWall,//左下
			0, 0, pw, ph / 2,
			playerH[scroll / 4 % 6], true);
		DxLib::SetDrawBright(0xff, 0xaa, 0xaa);


		//キャラ描画
		DrawRotaGraph(180, y, 1.0,0.0,playerH[scroll / 4 % 6] ,true,turn);
		DxLib::SetDrawBright(0xff, 0xff, 0xff);
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

		if (keystate[KEY_INPUT_SPACE]) {
			amplitude = 10.0f;
		}

		vy += 0.5;
		y += vy;
		if (y > ground_line) {
			y = ground_line;
			vy = 0.0f;
			jumping = false;
		}
		time += 0.025f;
		cbuffer[1] = time;
		UpdateShaderConstantBuffer(cbuff);
		SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 0);

		SetDrawScreen(rt);
		ClearDrawScreen();
		//MyDrawExtendGraph(0, 480+32, 640, 480 + y_offset-16, scrH, normalH, -1, ps);
		//↓元の絵
		DrawExtendGraph(0, y_offset, 640, y_offset+480, scrH, false);
		//水面の絵
		MyDrawExtendGraph(0, 480 + 32, 640, 480 + y_offset - 16, scrH, normalH, -1, ps);
		
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		DrawGraph(0, 0, bloodRT, true);
		for (auto& b : bloodPoints) {
			b.pos.y += b.g;
			b.crate *= 0.998f;
			//DrawCircleAA(b.pos.x, b.pos.y, 12*b.crate, 16, 0xaa0000, true);
			DrawRotaGraph(b.pos.x, b.pos.y, b.crate*0.35f, 0, bloodDrop, true);
		}


		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		int dx = 0, dy = 0;
		if (amplitude > 0.0f) {
			dx = ((frame % 3) - 1) * amplitude;
			dy = (((frame+rand()) % 3) - 1) * amplitude;
		}
		DrawGraph(dx, dy, rt, false);
		amplitude *= 0.9f;
		if (amplitude < 1.0f) {
			amplitude = 0.0f;
		}
		++frame;
		ScreenFlip();
	}
	DxLib_End();
}
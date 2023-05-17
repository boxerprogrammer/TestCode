#include<DxLib.h>
#include<array>
#include<list>
#include<vector>
#include<cmath>

using namespace std;

void DrawModel(int model,int vs,int ps,bool outline=false) {
	auto num=MV1GetMeshNum(model);
	for(int i=0;i<num;++i){
		MV1SetMeshBackCulling(model, i, DX_CULLING_LEFT);
	}
	DxLib::MV1SetUseOrigShader(false);
	MV1DrawModel(model);

	if(outline){
		DxLib::MV1SetUseOrigShader(true);
		SetUseVertexShader(vs);
		SetUsePixelShader(ps);
		for (int i = 0; i < num; ++i) {
			MV1SetMeshBackCulling(model, i, DX_CULLING_RIGHT);
		}
		MV1DrawModel(model);
	}
}

void MyDrawGraph(int x, int y, int width, int height) {
	array<VERTEX2DSHADER, 4> verts;
	for (auto& v : verts) {
		v.rhw = 1.0f;
		v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);
		v.u = 0.0f;
		v.v = 0.0f;
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
		v.spc = GetColorU8(0, 0, 0, 0);
	}
	verts[0].pos.x = x;
	verts[0].pos.y = y;
	verts[1].pos.x = x + width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	verts[2].pos.x = x;
	verts[2].pos.y = y + height;
	verts[2].v = 1.0f;
	verts[3].pos.x = x + width;
	verts[3].pos.y = y + height;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}


struct Position {
	float x, y;
};
std::vector<VERTEX2D> CreateVertices(const Position& center, const list<Position>& positions) {
	std::vector<VERTEX2D> ret(2*positions.size());
	float dC = 255.0f / static_cast<float>(positions.size());
	float alpha = 255.0f;
	auto it = positions.begin();
	for (int i = 0; i < positions.size(); ++i) {
		ret[i * 2 + 0].dif = GetColorU8(255, 255, 255, alpha);
		ret[i * 2 + 0].pos.x = center.x;
		ret[i * 2 + 0].pos.y = center.y;
		ret[i * 2 + 0].rhw = 1.0f;
		ret[i * 2 + 0].u = 0;
		ret[i * 2 + 0].v = 0;

		ret[i * 2 + 1].dif = GetColorU8(255, 255, 255, alpha);
		ret[i * 2 + 1].pos.x = center.x+it->x;
		ret[i * 2 + 1].pos.y = center.y+it->y;
		ret[i * 2 + 1].rhw = 1.0f;
		ret[i * 2 + 1].u = 0;
		ret[i * 2 + 1].v = 0;

		++it;
		alpha -= dC;
	}
	return ret;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetWindowSize(1280, 720);
	
	DxLib_Init();
	int res = SetGraphMode(1280, 720, 32);
	
	SetDrawScreen(DX_SCREEN_BACK);
	int model[3]={};
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 200, -400), VGet(0, 0, 0));
	auto vs = LoadVertexShader(L"threed_outline_vs.vso");
	auto ps = LoadPixelShader(L"threed_outline_ps.pso");
	auto noiseps=LoadPixelShader(L"noise_ps.pso");
	auto normalH=LoadGraph(L"normalmap.png");
	auto brickH = LoadGraph(L"brick.png");
	auto heightH = LoadSoftImage(L"cloud.png");
	auto brushH = LoadSoftImage(L"brush.png");
	float angle=0.0f;
	auto offscreen=MakeScreen(640,480);

	auto cbufferH = CreateShaderConstantBuffer(sizeof(float) * 4);
	float* time = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	time[0]=0.0f;

	float slashAngle = 0.0f;
	list<Position> positions;

	int sfw, sfh;
	GetSoftImageSize(heightH, &sfw, &sfh);
	
	vector<VERTEX3D> vertices(512 * 512);
	for (int j = 0; j < 512; ++j) {
		for (int i = 0; i < 512; ++i) {
			auto& v = vertices[i + j * 512];
			float r, g, b, a;
			GetPixelSoftImageF(heightH, i, j, &r, &g, &b, &a);

			v.pos = { (float)i-256.0f,r*100.0f,256.0f-(float)j };
			v.norm = {0.0f,1.0f,0.0f};
			v.dif = GetColorU8(255, 255, 255,255);
		}
	}

	int lx, ly;
	GetMousePoint(&lx, &ly);
	int mlast = GetMouseInput();
	while (ProcessMessage() != -1) {
		time[0]+=0.01f;
		SetDrawScreen(offscreen);
		//SetGraphMode(640, 480, 32);
		
		DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 200, -400), VGet(0, 0, 0));
		ClearDrawScreen();

		auto minput=GetMouseInput();
		if (minput & MOUSE_INPUT_LEFT) {
			int mx, my;
			GetMousePoint(&mx, &my);
			if (!(fabsf(mx - lx) < 5 && fabsf(my - ly)<5 && mlast == minput)) {
				BltSoftImageWithAlphaBlend(0, 0, 32, 32, brushH, mx - 640 - 16, my - 16, heightH, 64);
				lx = mx;
				ly = my;
			}
			
		}
		mlast = minput;
		
		for (int j = 0; j < 512; ++j) {
			for (int i = 0; i < 512; ++i) {
				auto& v = vertices[i + j * 512];
				float r, g, b, a;
				GetPixelSoftImageF(heightH, i, j, &r, &g, &b, &a);

				v.pos = { (float)i - 256.0f,r * 64.0f,256.0f - (float)j };
				v.norm = { 0.0f,1.0f,0.0f };
				v.dif = GetColorU8(255, 255, 255, 255);
			}
		}
		DrawPrimitive3D(vertices.data(), vertices.size(), DX_PRIMTYPE_POINTLIST, brickH, false);
		SetDrawScreen(DX_SCREEN_BACK);
		//SetGraphMode(640*2, 480, 32);
		DrawGraph(0, 0, offscreen, false);
		DrawSoftImage(640, 0, heightH); 
		ScreenFlip();
	}
	return 0;
}
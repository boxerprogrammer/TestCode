#include<DxLib.h>
#include<array>
#include<list>
#include<vector>

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
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	int model[3]={};
	model[0]=MV1LoadModel(L"model/torus.mqo");
	model[1] = MV1LoadModel(L"model/tekito.mqo");
	model[2] = MV1LoadModel(L"model/sphere.mqo");
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 200, -400), VGet(0, 0, 0));
	MV1SetPosition(model[1],VGet(-205,0,0));
	MV1SetPosition(model[2], VGet(205, 0, 0));
	auto vs = LoadVertexShader(L"threed_outline_vs.vso");
	auto ps = LoadPixelShader(L"threed_outline_ps.pso");
	auto noiseps=LoadPixelShader(L"noise_ps.pso");
	auto normalH=LoadGraph(L"normalmap.png");
	auto brickH = LoadGraph(L"brick.png");
	float angle=0.0f;
	auto offscreen=MakeScreen(640,480);

	auto cbufferH = CreateShaderConstantBuffer(sizeof(float) * 4);
	float* time = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	time[0]=0.0f;

	float slashAngle = 0.0f;
	list<Position> positions;
	while (ProcessMessage() != -1) {
		time[0]+=0.01f;

		SetDrawScreen(offscreen);
		DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 200, -400), VGet(0, 0, 0));
		ClearDrawScreen();
		DrawGraph(0, 0, brickH, false);
		angle+=0.01f;

		int mx,my;
		GetMousePoint(&mx,&my);


		auto from=DxLib::ConvScreenPosToWorldPos(VGet(mx,my,0));
		auto to = DxLib::ConvScreenPosToWorldPos(VGet(mx, my, 1));



		for(int i=0;i<3;++i){
			auto check=MV1CollCheck_Line(model[i],0,from,to);
			MV1SetRotationXYZ(model[i], VGet(0, angle, 0));
			DrawModel(model[i],vs,ps,check.HitFlag);
		}

		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		UpdateShaderConstantBuffer(cbufferH);
		SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 3);
		SetUsePixelShader(noiseps);
		SetUseTextureToShader(0, offscreen);
		SetUseTextureToShader(1,normalH);
		MyDrawGraph(0,0,640,480);

		//DrawPrimitive2D();
		float sx = cos(slashAngle)*200;
		float sy = sin(slashAngle)*200;
		DrawLineAA(320, 240, 320 + sx, 240 + sy, 0xffffff,5.0f);
		positions.push_front({ sx,sy });
		if (positions.size() >= 16) {
			positions.pop_back();
		}
		auto verts=CreateVertices({ 320,240 }, positions);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		DrawPrimitive2D(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP, DX_NONE_GRAPH,true);
		slashAngle += 0.1f;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);


		DrawFormatString(10,10,0xffffffff,L"FPS=%f",GetFPS());
		ScreenFlip();
	}
	return 0;
}
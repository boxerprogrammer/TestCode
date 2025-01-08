#include<DxLib.h>
#include<cassert>
#include<cmath>
#include<algorithm>
#include<vector>
#include<list>

#include"Geometry.h"
#include"Cuboid.h"

using namespace mgt_lib;



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	DxLib::ChangeWindowMode(true);
	if (DxLib::DxLib_Init() == -1) {
		return -1;
	}
	auto model= DxLib::MV1LoadModel(L"model/sphere.mv1");
	assert(model > 0);
	auto img = DxLib::LoadGraph(L"img/hole.png");
	assert(img > 0);
	auto vs = DxLib::LoadVertexShader(L"VertexShader.vso");
	assert(vs > 0);
	auto ps= DxLib::LoadPixelShader(L"PixelShader.pso");
	assert(ps > 0);

	VECTOR minpos = {10000.f,10000.f,10000.f};
	VECTOR maxpos = { -10000.f,-10000.f,-10000.f };
	int num = DxLib::MV1GetMeshNum(model);
	for (int i = 0; i < num; ++i) {
		auto tmin = DxLib::MV1GetMeshMinPosition(model, i);
		minpos = VGet(std::min(tmin.x, minpos.x), std::min(tmin.y, minpos.y), std::min(tmin.z, minpos.z));
		auto tmax = DxLib::MV1GetMeshMaxPosition(model, i);
		maxpos = VGet(std::max(tmax.x, maxpos.x), std::max(tmax.y, maxpos.y), std::max(tmax.z, maxpos.z));
	}

	

	DxLib::SetWriteZBuffer3D(true);
	DxLib::SetUseZBuffer3D(true);
	struct ForDecalCBuff {
		float minY;
		float maxY;
		float radius;
		float angle;
	};
	struct DecalBox {
		Position3 pos;
		int enable;
		//16byte
		Size3D size;//12byte;
		float dummy1;
		//Axis3D axis;//12byte*3=36byte
		Vector3 right;
		float dummy2;
		Vector3 up;
		float dummy3;
		Vector3 front;
		float dummy4;
	};
	size_t s = sizeof(DecalBox);
	int decalCBuffH= DxLib::CreateShaderConstantBuffer(sizeof(DecalBox)*8);
	auto decalCBuff= (DecalBox*)DxLib::GetBufferShaderConstantBuffer(decalCBuffH);
	for(int i=0;i<8;++i){
		decalCBuff[i] = {};
	}
	UpdateShaderConstantBuffer(decalCBuffH);

	int cbuffH= DxLib::CreateShaderConstantBuffer(sizeof(ForDecalCBuff));
	auto cbuff= (ForDecalCBuff*)DxLib::GetBufferShaderConstantBuffer(cbuffH);
	cbuff->radius = std::hypot(maxpos.x - minpos.x, 0.0f, maxpos.z - minpos.z);
	cbuff->minY = minpos.y;
	cbuff->maxY = maxpos.y;
	DxLib::UpdateShaderConstantBuffer(cbuffH);
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	float angle = 0.0f;
	int mouseInput = 0;
	int lastMouseInput = 0;
	VECTOR rayVec = {};
	VECTOR cameraPos = VGet(150.0f, 150.0f, -500.0f);

	//std::vector<Position3> hitPositions;
	std::list<mgt_lib::Cuboid> boxes;

	mgt_lib::Cuboid testc({0.0f,0.0f,0.f},{right_vec,up_vec,front_vec},{50.0f,50.0f,50.0f});
	float camAngle = 0.01f;
	MATRIX rot = {};
	
	DxLib::CreateRotationYMatrix(&rot, camAngle);
	cameraPos = VTransformSR(cameraPos, rot);
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		SetLightEnable(true);
		
		cameraPos = VTransformSR(cameraPos, rot);
		DxLib::DrawBox(0, 0, 640, 480, 0x0000aa,true);

		lastMouseInput = mouseInput;
		mouseInput=GetMouseInput();
		if (!(lastMouseInput & MOUSE_INPUT_LEFT) && mouseInput) {
			DxLib::SetCameraNearFar(1.0f, 1000.0f);
			SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
			int mx, my;
			GetMousePoint(&mx, &my);
			auto spos = ConvScreenPosToWorldPos(VGet(mx, my, 0.0f));
			rayVec = VSub(spos, cameraPos);
			auto epos = VAdd(spos, VScale(rayVec, 500.0f));
			auto collResult = DxLib::MV1CollCheck_Line(model,0,spos,epos);
			if (collResult.HitFlag) {
				auto hitpos = GetVector3(collResult.HitPosition);
				
				boxes.push_back(mgt_lib::Cuboid(hitpos, { right_vec,up_vec,front_vec }, Size3D(20.0f,20.0f,20.0f)));
				if(boxes.size()>8){
					boxes.pop_front();
				}
				int idx = 0;
				for(const auto& box : boxes){
					decalCBuff[idx].enable = 1;
					decalCBuff[idx].pos = box.GetPos();
					decalCBuff[idx].right = box.GetAxis().right;
					decalCBuff[idx].up = box.GetAxis().up;
					decalCBuff[idx].front = box.GetAxis().front;
					decalCBuff[idx].size = box.GetSize();
					++idx;
				}
				UpdateShaderConstantBuffer(decalCBuffH);
			}
		}
		//angle += 0.05f;
		MV1SetRotationXYZ(model,VGet(0.0f, angle, 0.0f));
		UpdateShaderConstantBuffer(cbuffH);
		MV1SetUseOrigShader(true);
		SetUseVertexShader(vs);
		SetUsePixelShader(ps);
		SetShaderConstantBuffer(cbuffH, DX_SHADERTYPE_PIXEL, 3);
		SetShaderConstantBuffer(decalCBuffH, DX_SHADERTYPE_PIXEL, 4);
		SetUseTextureToShader(3, img);
		DxLib::SetCameraNearFar(1.0f, 1000.0f);
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
		MV1DrawModel(model);
		MV1SetUseOrigShader(false);

		//for (const auto& hitPos : hitPositions) {
		//	DrawSphere3D(VGetFromVector3(hitPos), 10, 16, 0xff0000, 0xffffff, true);
		//}
		//for(const auto box : boxes){
		//	box.WireframeDraw();
		//}
		//SetLightEnable(false);
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
		DxLib::SetCameraNearFar(1.0f, 1000.0f);
		for(const auto& box : boxes){
			box.WireframeDraw();
		}

		DrawLine3D(VGet(minpos.x, minpos.y, minpos.z), VGet(minpos.x, maxpos.y, minpos.z), 0x00ff00);
		DrawLine3D(VGet(maxpos.x, minpos.y, minpos.z), VGet(maxpos.x, maxpos.y, minpos.z), 0x00ff00);
		DrawLine3D(VGet(minpos.x, minpos.y, maxpos.z), VGet(minpos.x, maxpos.y, maxpos.z), 0x00ff00);
		DrawLine3D(VGet(maxpos.x, minpos.y, maxpos.z), VGet(maxpos.x, maxpos.y, maxpos.z), 0x00ff00);
		DrawLine3D(VGet(minpos.x, maxpos.y, minpos.z), VGet(minpos.x, maxpos.y, maxpos.z), 0x00ff00);
		DrawLine3D(VGet(maxpos.x, maxpos.y, minpos.z), VGet(maxpos.x, maxpos.y, maxpos.z), 0x00ff00);
		DrawLine3D(VGet(minpos.x, maxpos.y, minpos.z), VGet(maxpos.x, maxpos.y, minpos.z), 0x00ff00);
		DrawLine3D(VGet(minpos.x, maxpos.y, maxpos.z), VGet(maxpos.x, maxpos.y, maxpos.z), 0x00ff00);
		DrawLine3D(VGet(minpos.x, minpos.y, minpos.z), VGet(minpos.x, minpos.y, maxpos.z), 0x00ff00);
		DrawLine3D(VGet(maxpos.x, minpos.y, minpos.z), VGet(maxpos.x, minpos.y, maxpos.z), 0x00ff00);
		DrawLine3D(VGet(minpos.x, minpos.y, minpos.z), VGet(maxpos.x, minpos.y, minpos.z), 0x00ff00);
		DrawLine3D(VGet(minpos.x, minpos.y, maxpos.z), VGet(maxpos.x, minpos.y, maxpos.z), 0x00ff00);

		//testc.WireframeDraw();

		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
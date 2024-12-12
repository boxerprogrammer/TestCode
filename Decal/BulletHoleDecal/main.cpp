#include<DxLib.h>
#include<cassert>
#include<cmath>
#include<algorithm>
#include<vector>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	auto model=MV1LoadModel(L"model/sphere.mv1");
	assert(model > 0);
	auto img = LoadGraph(L"img/hole.png");
	assert(img > 0);
	auto vs = LoadVertexShader(L"VertexShader.vso");
	assert(vs > 0);
	auto ps=LoadPixelShader(L"PixelShader.pso");
	assert(ps > 0);

	VECTOR minpos = {10000.f,10000.f,10000.f};
	VECTOR maxpos = { -10000.f,-10000.f,-10000.f };
	int num = MV1GetMeshNum(model);
	for (int i = 0; i < num; ++i) {
		auto tmin = MV1GetMeshMinPosition(model, i);
		minpos = VGet(std::min(tmin.x, minpos.x), std::min(tmin.y, minpos.y), std::min(tmin.z, minpos.z));
		auto tmax = MV1GetMeshMaxPosition(model, i);
		maxpos = VGet(std::max(tmax.x, maxpos.x), std::max(tmax.y, maxpos.y), std::max(tmax.z, maxpos.z));
	}

	

	SetWriteZBuffer3D(true);
	SetUseZBuffer3D(true);
	struct ForDecalCBuff {
		float minY;
		float maxY;
		float radius;
		float angle;
	};
	int cbuffH=CreateShaderConstantBuffer(sizeof(ForDecalCBuff));
	auto cbuff= (ForDecalCBuff*)GetBufferShaderConstantBuffer(cbuffH);
	cbuff->radius = std::hypot(maxpos.x - minpos.x, 0.0f, maxpos.z - minpos.z);
	cbuff->minY = minpos.y;
	cbuff->maxY = maxpos.y;
	UpdateShaderConstantBuffer(cbuffH);
	SetDrawScreen(DX_SCREEN_BACK);
	float angle = 0.0f;
	int mouseInput = 0;
	int lastMouseInput = 0;
	VECTOR rayVec = {};
	VECTOR cameraPos = VGet(150, 150, -500);
	while (ProcessMessage() != -1) {
		ClearDrawScreen();

		lastMouseInput = mouseInput;
		mouseInput=GetMouseInput();
		if (!(lastMouseInput & MOUSE_INPUT_LEFT) && mouseInput) {
			int mx, my;
			GetMousePoint(&mx, &my);
			auto spos = ConvScreenPosToWorldPos(VGet(mx, my, 0.0f));
			rayVec = VSub(spos, cameraPos);
			MV1Intersect
		}
		//angle += 0.05f;
		MV1SetRotationXYZ(model,VGet(0.0f, angle, 0.0f));
		UpdateShaderConstantBuffer(cbuffH);
		MV1SetUseOrigShader(true);
		SetUseVertexShader(vs);
		SetUsePixelShader(ps);
		SetShaderConstantBuffer(cbuffH, DX_SHADERTYPE_PIXEL, 3);
		SetUseTextureToShader(3, img);
		DxLib::SetCameraNearFar(1.0f, 1000.0f);
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
		MV1DrawModel(model);
		MV1SetUseOrigShader(false);

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

		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
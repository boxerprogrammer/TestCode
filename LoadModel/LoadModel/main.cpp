#include<DxLib.h>


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	auto cameraPos = VGet(0, 200, -300);
	DxLib::SetCameraNearFar(1.0, 500.0f);
	DxLib::SetCameraPositionAndTargetAndUpVec(cameraPos, VGet(0, 0, 0), VGet(0, 1, 0));
	int model=MV1LoadModel(L"model/bodyeater.mv1");
	int roundbox = MV1LoadModel(L"pbrmodel/roundbox.mv1");
	int sphere = MV1LoadModel(L"pbrmodel/sphere.mv1");
	int rough = LoadGraph(L"pbrmodel/steel/roughness.png");
	int metallic = LoadGraph(L"pbrmodel/steel/metallic.png");
	int sphmap = LoadGraph(L"pbrmodel/sph.png");
	int pso = LoadPixelShader(L"ps.pso");
	int vso = LoadVertexShader(L"vs.vso");
	auto tlNum=MV1GetTriangleListNum(model);
	int tlVertType = -1;
	for (int i = 0; i < tlNum; ++i) {
		tlVertType=MV1GetTriangleListVertexType(model,i);
		break;
	}
	DxLib::SetUseZBuffer3D(true);
	DxLib::SetWriteZBuffer3D(true);
	DxLib::SetWriteZBufferFlag(true);
	DxLib::SetZBufferCmpType(DX_CMP_LESS);
	DxLib::MV1SetUseZBuffer(model, true);
	DxLib::MV1SetWriteZBuffer(model, true);
	DxLib::MV1SetZBufferCmpType(model, DX_CMP_LESS);
	
	auto startTime = GetTickCount64();
	float angle = 0.0f;
	auto rt = MakeScreen(640, 480);
	float amplitude = 0.0f;
	int frame=0;
	char keystate[256];
	
	auto cbufferH = CreateShaderConstantBuffer(sizeof(float) * (4+16));
	float* cpos = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	MV1SetUseOrigShader(true);
	while (ProcessMessage() != -1) {
		SetDrawScreen(rt);
		DxLib::SetCameraNearFar(1.0, 500.0f);
		DxLib::SetCameraPositionAndTargetAndUpVec(cameraPos, VGet(0, 0, 0), VGet(0, 1, 0));
		auto vmat=GetCameraViewMatrix();
		cpos[0] = cameraPos.x;
		cpos[1] = cameraPos.y;
		cpos[2] = cameraPos.z;
		int idx = 4;
		for (int j = 0; j < 4; ++j) {
			for (int i = 0; i < 4; ++i) {
				cpos[idx] = vmat.m[i][j];
				++idx;
			}
		}

		UpdateShaderConstantBuffer(cbufferH);
		SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 0);

		ClearDrawScreen();
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_SPACE]) {
			amplitude = 10.0f;
		}
		if (tlVertType == DX_MV1_VERTEX_TYPE_1FRAME) {
			DrawString(10, 10, L"not normal not skinning",0xffffff);
		}
		else if (tlVertType == DX_MV1_VERTEX_TYPE_4FRAME) {
			DrawString(10, 10, L"not normal use skinning", 0xffffff);
		}
		else if (tlVertType == DX_MV1_VERTEX_TYPE_NMAP_1FRAME) {
			DrawString(10, 10, L"use normal not skinning", 0xffffff);
		}
		else if (tlVertType == DX_MV1_VERTEX_TYPE_NMAP_4FRAME) {
			DrawString(10, 10, L"use normal use skinning", 0xffffff);
		}
		angle = static_cast<float>(GetTickCount64() - startTime)/1000.0f;
		//MV1SetUseOrigShader(false);
		//MV1DrawModel(sphere);
		
		
		DxLib::SetUseVertexShader(vso);
		DxLib::SetUsePixelShader(pso);
		DxLib::SetUseTextureToShader(2, rough);
		DxLib::SetUseTextureToShader(3, metallic);
		DxLib::SetUseTextureToShader(4, sphmap);
		//MV1SetScale(model, VGet(10,10,10));
		MV1SetRotationXYZ(roundbox, VGet(0, angle, angle/2));
		MV1SetScale(roundbox, VGet(1.0,1.0,1.0));
		MV1SetPosition(roundbox, VGet(-100, -30.0, 0));
		MV1DrawModel(roundbox);

		MV1SetRotationXYZ(sphere, VGet(0, -angle/2, -angle ));
		MV1SetScale(sphere, VGet(1.0, 1.0, 1.0));
		MV1SetPosition(sphere, VGet(100, -30.0, 0));
		MV1DrawModel(sphere);
		
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		int dx = 0, dy = 0;
		if (amplitude > 0.0f) {
			dx = ((frame % 3) - 1) * amplitude;
			dy = (((frame + rand()) % 3) - 1) * amplitude;
		}
		DrawGraph(dx, dy, rt, false);
		amplitude *= 0.9f;
		if (amplitude < 1.0f) {
			amplitude = 0.0f;
		}
		++frame;

		auto fps = GetFPS();
		DrawFormatString(50, 50, 0xffffff, L"FPS=%2.2f", fps);
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
#include<DxLib.h>


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	if (DxLib_Init() == -1) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	DxLib::SetCameraNearFar(1.0, 500.0f);
	DxLib::SetCameraPositionAndTargetAndUpVec(VGet(0, 0, -300), VGet(0, 0, 0), VGet(0, 1, 0));
	int model=MV1LoadModel(L"model/bodyeater.mv1");
	int sphere = MV1LoadModel(L"pbrmodel/sphere.mv1");
	int rough = LoadGraph(L"pbrmodel/roughness.png");
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
	
	float angle = 0.0f;
	auto rt = MakeScreen(640, 480);
	float amplitude = 0.0f;
	int frame=0;
	char keystate[256];
	while (ProcessMessage() != -1) {
		SetDrawScreen(rt);
		DxLib::SetCameraNearFar(1.0, 500.0f);
		DxLib::SetCameraPositionAndTargetAndUpVec(VGet(0, 0, -300), VGet(0, 0, 0), VGet(0, 1, 0));

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
		angle += 0.01f;
		//MV1SetUseOrigShader(false);
		//MV1DrawModel(sphere);
		MV1SetUseOrigShader(true);
		
		DxLib::SetUseVertexShader(vso);
		DxLib::SetUsePixelShader(pso);
		DxLib::SetUseTextureToShader(2, rough);
		DxLib::SetUseTextureToShader(3, sphmap);
		//MV1SetScale(model, VGet(10,10,10));
		MV1SetRotationXYZ(sphere, VGet(0, angle, 0));
		MV1SetScale(sphere, VGet(1.0,1.0,1.0));
		MV1SetPosition(sphere, VGet(0, -50.0, 0));
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


		ScreenFlip();
	}
	DxLib_End();
	return 0;
}
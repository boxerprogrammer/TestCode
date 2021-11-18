#include<DxLib.h>

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
	auto vs = LoadVertexShader(L"threed_outline.vso");
	auto ps = LoadPixelShader(L"threed_outline.pso");
	float angle=0.0f;
	while (ProcessMessage() != -1) {
		ClearDrawScreen();
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
		ScreenFlip();
	}
	return 0;
}
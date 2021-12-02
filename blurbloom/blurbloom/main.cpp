#include<DxLib.h>


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	auto offscreen=MakeScreen(640,480);
	auto model=MV1LoadModel(L"model/cube.mqo");
	SetDrawScreen(offscreen);
	SetCameraPositionAndTarget_UpVecY(VGet(0,200,-400),VGet(0,0,0));
	float angle=0.0f;
	auto copydst=MakeScreen(640,480,false);
	auto highb = MakeScreen(640, 480, false);
	auto downscale = MakeScreen(640/8, 480/8, false);
	auto gauss = MakeScreen(640 / 8, 480 / 8, false);
	auto downscale2 = MakeScreen(640 / 4, 480 / 4, false);
	auto gauss2 = MakeScreen(640 / 4, 480 / 4, false);
	while (ProcessMessage() != -1) {
		SetDrawScreen(offscreen);
		ClearDrawScreen();
		SetCameraPositionAndTarget_UpVecY(VGet(0, 200, -400), VGet(0, 0, 0));
		MV1SetRotationXYZ(model,VGet(angle,angle*2,angle*3));
		angle+=0.01f;
		MV1DrawModel(model);
		SetDrawScreen(copydst);
		ClearDrawScreen();
		SetCameraPositionAndTarget_UpVecY(VGet(0, 200, -400), VGet(0, 0, 0));
		MV1DrawModel(model);

		DxLib::FillGraph(highb,0,0,0);
		DxLib::FillGraph(downscale, 0, 0, 0);
		DxLib::FillGraph(gauss, 0, 0, 0);
		DxLib::FillGraph(downscale2, 0, 0, 0);
		DxLib::FillGraph(gauss2, 0, 0, 0);

		//高輝度成分を抜き出す
		GraphFilterBlt(offscreen,highb,DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, 200,false);
		
		//ダウンスケール(縮小バッファにコピー)
		GraphFilterBlt(highb,downscale, DX_GRAPH_FILTER_DOWN_SCALE, 8);
		GraphFilterBlt(downscale,gauss, DX_GRAPH_FILTER_GAUSS, 8,800);

		GraphFilterBlt(highb,copydst,DX_GRAPH_FILTER_REPLACEMENT,255,255,255,255, 0,0,255,255);
		GraphFilterBlt(copydst, downscale2, DX_GRAPH_FILTER_DOWN_SCALE, 4);
		GraphFilterBlt(downscale2, gauss2, DX_GRAPH_FILTER_GAUSS, 8, 400);
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		DrawGraph(0, 0, offscreen, false);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		SetDrawMode(DX_DRAWMODE_BILINEAR);
		DrawExtendGraph(0, 0, 640, 480, gauss, false);
		//DrawExtendGraph(0, 0,640,480, gauss, false);
		//DrawExtendGraph(0, 0, 640, 480, gauss, false);
		DrawExtendGraph(0, 0, 640, 480, gauss2, false);
		DrawExtendGraph(0, 0, 640, 480, gauss2, false);
		SetDrawMode(DX_DRAWMODE_NEAREST);


		DrawFormatString(10,10,0xffffff,L"FPS=%f",GetFPS());
		ScreenFlip();
		
	}
	DxLib_End();
	return 0;
}
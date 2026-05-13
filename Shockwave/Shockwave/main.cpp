#include<DxLib.h>
#include<cassert>
#include<array>
#include<d3dcompiler.h>
#include<string>
#include<vector>
#include"DrawUtil.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace std;

bool CheckShaderCompile(HRESULT result, ID3DBlob* errBlob) {
	if (result != S_OK) {
		auto errSize = errBlob->GetBufferSize();
		std::string errMsg;
		errMsg.resize(errSize);
		memcpy_s(errMsg.data(), errMsg.size(),
			errBlob->GetBufferPointer(), errBlob->GetBufferSize());
		OutputDebugStringA(errMsg.c_str());
		assert(0);
		return false;
	}
	return true;
}

bool CompilePixelShader(const std::wstring& path,const std::vector<D3D_SHADER_MACRO>& defines,ID3DBlob*& psBlob) {
	ID3DBlob* errBlob = nullptr;
	auto result = D3DCompileFromFile(
		path.c_str(),//hlslのパス
		defines.empty()?nullptr:defines.data(), //defineを設定したいならここを設定する。なければnullptrを入れる
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルードするかどうか？
		"main", //エントリポイント(DXライブラリのシェーダはエントリポイントがmain固定なので"main"にしとく)
		"ps_5_0",//シェーダのバージョン ピクセルシェーダなら"ps_"と書いてそのあとにバージョン番号が_区切りで入る
		0,//フラグ1(デバッグやリリースなどの指定を行う) 
		0,//フラグ2(0でいいです)
		&psBlob,
		&errBlob);
	if (!CheckShaderCompile(result, errBlob)) {
		return false;
	}
	if (errBlob != nullptr) {
		errBlob->Release();
	}
	return true;
}

bool CompileVertexShader(const std::wstring& path, const std::vector<D3D_SHADER_MACRO>& defines, ID3DBlob*& vsBlob) {
	ID3DBlob* errBlob = nullptr;
	auto result = D3DCompileFromFile(
		path.c_str(),//hlslのパス
		defines.empty()?nullptr:defines.data(), //defineを設定したいならここを設定する。なければnullptrを入れる
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルードするかどうか？
		"main", //エントリポイント(DXライブラリのシェーダはエントリポイントがmain固定なので"main"にしとく)
		"vs_5_0",//シェーダのバージョン ピクセルシェーダなら"ps_"と書いてそのあとにバージョン番号が_区切りで入る
		0,//フラグ1(デバッグやリリースなどの指定を行う) 
		0,//フラグ2(0でいいです)
		&vsBlob,
		&errBlob);
	if (!CheckShaderCompile(result, errBlob)) {
		return false;
	}
	if (errBlob != nullptr) {
		errBlob->Release();
	}
	return true;
}

int 
LoadDefinedVertexShader(const wstring& path, const vector<D3D_SHADER_MACRO>& defines) {
	ID3DBlob* vsBlob = nullptr;
	if (!CompileVertexShader(path, defines, vsBlob)) {
		return -1;
	}
	int vsH = LoadVertexShaderFromMem(vsBlob->GetBufferPointer(),
									vsBlob->GetBufferSize());
	assert(vsH >= 0);
	vsBlob->Release();
	return vsH;
}

int
LoadDefinedPixelShader(const wstring& path, const vector<D3D_SHADER_MACRO>& defines) {
	ID3DBlob* psBlob = nullptr;
	if (!CompilePixelShader(path, defines, psBlob)) {
		return -1;
	}
	int psH = LoadPixelShaderFromMem(psBlob->GetBufferPointer(),
		psBlob->GetBufferSize());
	assert(psH >= 0);
	psBlob->Release();
	return psH;
}



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetGraphMode(640, 480, 32);
	if (DxLib_Init() != 0) {
		return -1;
	}

	SetDrawMode(DX_DRAWMODE_BILINEAR);

	int stelthPatternH = LoadGraph(L"img/stelthpattern.jpg");
	assert(stelthPatternH >= 0);

	int rengaH = LoadGraph(L"img/renga.png");
	assert(rengaH >= 0);

	int patternH = LoadGraph(L"img/pattern.png");
	assert(patternH >= 0);

	int matcapH = LoadGraph(L"matcap/dia.png");

	int modelH = MV1LoadModel(L"model/bodyeater.mv1");
	assert(modelH >= 0);
	std::vector<int> models;
	for(int i=0;i<10;++i) {//ボーンの表示をOFFにする
		models.push_back(MV1DuplicateModel(modelH));
		auto tmpModelH = models.back();
		MV1SetScale(tmpModelH,VGet(0.25f,0.25f,0.25f));
		MV1SetPosition(tmpModelH, VGet(-70+(float)(i%3)*30.0f, 0.0f, -(float)(i/3)*30.0f));
		MV1SetRotationXYZ(tmpModelH, VGet(0.0f, DX_PI_F/4.0f,0.0f));
	}
	MV1SetScale(modelH,VGet(0.5f,0.5f,0.5f));
	MV1SetPosition(modelH, VGet(0.0f, 0.0f, -100.0f));
	MV1SetRotationXYZ(modelH, VGet(0.0f, DX_PI_F/3.0f,0.0f));

	int fieldH = MV1LoadModel(L"model/city/city3.mv1");
	
	assert(fieldH >= 0);

	std::vector<D3D_SHADER_MACRO> defines;
	int psNoneH = LoadDefinedPixelShader(L"Shader/MV1PixelShader.hlsl",defines);
	int vsNoneH = LoadDefinedVertexShader(L"Shader/MV1VetexShader.hlsl", defines);
	defines = {
		{"BUMPMAP","1"},
		{nullptr,nullptr}
	};
	int psNormalH = LoadDefinedPixelShader(L"Shader/MV1PixelShader.hlsl", defines);
	int vsNormalH = LoadDefinedVertexShader(L"Shader/MV1VetexShader.hlsl", defines);
	defines.clear();
	int psGhostH = LoadDefinedPixelShader(L"shader/ghostShaderPS.hlsl", defines);

	int psDepthOnlyH = LoadPixelShader(L"depthOnlyPS.pso");
	assert(psDepthOnlyH >= 0);

	int psOutlineH = LoadPixelShader(L"OutlinePS.pso");
	assert(psOutlineH >= 0);

	int psPixelEffect = LoadPixelShader(L"PixelEffect.pso");
	assert(psPixelEffect >= 0);

	SetDrawScreen(DX_SCREEN_BACK);
	int sw, sh, sd;
	GetScreenState(&sw, &sh, &sd);

	uint64_t lastTime = GetTickCount64();
	SetupCamera_Perspective(DX_PI_F / 3.0f);//画角
	VECTOR cpos = VGet(10, 20, -200);
	SetCameraPositionAndTarget_UpVecY(cpos, VGet(0, 0, 0));
	SetCameraNearFar(0.1f, 1000.0f);
	SetUseLighting(true);
	SetLightEnable(true);
	SetGlobalAmbientLight(GetColorF(0.2, 0.2, 0.2, 1));
	SetLightDirection(VGet(1, -1, 1));
	SetUseZBuffer3D(true);
	SetUseCullingFlag(true);

	SetUseBackCulling(DX_CULLING_NONE);

	auto tlistNum = MV1GetTriangleListNum(fieldH);
	int vType = -1;
	std::vector<int> vertexTypes;
	for (int i = 0;i < tlistNum; ++i) {
		vertexTypes.push_back(MV1GetTriangleListVertexType(fieldH, i));//頂点タイプ
	}

	auto modelListNum = MV1GetTriangleListNum(modelH);
	std::vector<int> modelVertexTypes;
	for (int i = 0;i < modelListNum; ++i) {
		modelVertexTypes.push_back(MV1GetTriangleListVertexType(modelH, i));//頂点タイプ
	}

	int meshNum = MV1GetMeshNum(fieldH);
	//バックフェースカリングOFF
	for (int i = 0;i < meshNum;++i) {
		MV1SetMeshBackCulling(fieldH, i, DX_CULLING_NONE);
	}
	//バウンディングボックスの作成
	auto vmin = MV1GetFrameMinVertexLocalPosition(fieldH, 0);
	auto vmax = MV1GetFrameMaxVertexLocalPosition(fieldH, 0);
	auto frameNum = MV1GetFrameNum(fieldH);
	for (int i = 0; i < frameNum; ++i) {
		auto tmpMin=MV1GetFrameMinVertexLocalPosition(fieldH, i);
		auto tmpMax= MV1GetFrameMaxVertexLocalPosition(fieldH, i);
		vmin = { std::min(vmin.x,tmpMin.x),std::min(vmin.y,tmpMin.y),std::min(vmin.z,tmpMin.z) };
		vmax = { std::max(vmax.x,tmpMax.x),std::max(vmax.y,tmpMax.y),std::max(vmax.z,tmpMax.z) };
	}
	char keystate[256];
	float angle = 0.0f;
	constexpr float m_scale = 0.5f;
	std::array<VECTOR, 8> bbVertices;
	vmax = VScale(vmax, m_scale);
	vmin = VScale(vmin, m_scale);

	bbVertices[0] = vmin;//手前左下
	bbVertices[1] = { vmax.x,vmin.y,vmin.z };//手前右下
	bbVertices[2] = { vmax.x,vmax.y,vmin.z };//手前右上
	bbVertices[3] = { vmin.x,vmax.y,vmin.z };//手前左上

	bbVertices[4] = { vmin.x,vmin.y,vmax.z };//奥左下
	bbVertices[5] = { vmax.x,vmin.y,vmax.z };//奥右下
	bbVertices[6] = vmax;//奥右上
	bbVertices[7] = { vmin.x,vmax.y,vmax.z };//奥左上

	struct GBoundingBox {
		VECTOR vmin;//4*3=12バイト
		float padding0;
		VECTOR vmax;//4*3=12バイト
		float threshold;//ディゾルヴ閾値
	};
	int cbuffH = CreateShaderConstantBuffer(sizeof(GBoundingBox));
	assert(cbuffH >= 0);
	auto pBB=(GBoundingBox*)GetBufferShaderConstantBuffer(cbuffH);
	pBB->vmin = vmin;
	pBB->vmax = vmax;
	pBB->threshold=1.0f;
	UpdateShaderConstantBuffer(cbuffH);

	struct ClickedPos {//画面クリックした座標を送る
		float x, y, z, w;
	};
	int cbuffClickedPosH = CreateShaderConstantBuffer(sizeof(ClickedPos));
	assert(cbuffClickedPosH >= 0);

	auto pClickedPos = (ClickedPos*)GetBufferShaderConstantBuffer(cbuffClickedPosH);
	pClickedPos->z = 0.0f;
	UpdateShaderConstantBuffer(cbuffClickedPosH);

	auto RTBlur = MakeScreen(sw/2, sh/2, false);//ぼかし画像をかきこむ画面
	auto RTColor = MakeScreen(sw, sh, false);//カラー値を書き込む画面
	auto RTNormal = MakeScreen(sw,sh,false);//法線ベクトルを書き込む画面

	//深度値だけは、Rのみにして、かつ32ビットにします。
	SetDrawValidFloatTypeGraphCreateFlag(true);//記録形式をfloatに
	//SetUsePlatformTextureFormat(DX_TEXTUREFORMAT_DIRECT3D9_R32F);
	SetCreateGraphChannelBitDepth(32);//１チャンネル当たりのビット数を32bitに
	SetCreateDrawValidGraphChannelNum(1);//チャンネル数を１に
	auto RTDepth = MakeScreen(sw, sh);//深度値を書き込む画面

	auto RTDepthForStelth = MakeScreen(sw, sh);//深度値を書き込む

	auto RTDistance = MakeScreen(sw, sh);

	//元に戻す
	SetCreateGraphColorBitDepth(0);
	SetCreateDrawValidGraphChannelNum(0);
	SetDrawValidFloatTypeGraphCreateFlag(false);

	auto ps2DH = LoadPixelShader(L"DepthVisualizePS.pso");
	assert(ps2DH >= 0);
	ID3DBlob* psBlob = nullptr;
	std::vector< D3D_SHADER_MACRO> defines2={
		{"USE_DOF",""},//#define USE_DOFが定義されている状態
		{nullptr,nullptr}//D3D_SHADER_MACRO配列は最後を必ずnullptr,nullptrにしてください
	};
	auto psDofH = LoadDefinedPixelShader(L"DepthOfField.hlsl", defines2);
	defines2[0].Name = "USE_FOG";//#define USE_FOGが定義されている状態
	auto psFogH = LoadDefinedPixelShader(L"DepthOfField.hlsl", defines2);
	defines2.clear();
	auto psNoneEffectH = LoadDefinedPixelShader(L"DepthOfField.hlsl", defines2);
	auto RTForPostEffect = MakeScreen(sw, sh);
	bool isWave = false;
	constexpr int stelth_interval = 120;
	int stelthFrame = 0;
;	while (ProcessMessage() != -1) {
		angle += 0.1f;
		SetDrawScreen(RTColor);
		ClearDrawScreen();

		DrawExtendGraph(0, 0, 640, 480, rengaH, false);


		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_W]) {
			cpos.y += 0.5f;
		}
		if (keystate[KEY_INPUT_S]) {
			cpos.y -= 0.5f;
		}
		if (keystate[KEY_INPUT_A]) {
			cpos.x -= 0.5f;
		}
		if (keystate[KEY_INPUT_D]) {
			cpos.x += 0.5f;
		}
		if (keystate[KEY_INPUT_Q]) {
			cpos.z += 0.5f;
		}
		if (keystate[KEY_INPUT_E]) {
			cpos.z -= 0.5f;
		}
		if (keystate[KEY_INPUT_UP]) {
			pBB->threshold = std::min(1.1f, pBB->threshold + 0.01f);
		}
		else if(keystate[KEY_INPUT_DOWN]) {
			pBB->threshold = std::max(-0.25f, pBB->threshold - 0.01f);
		}

		if (keystate[KEY_INPUT_SPACE]) {
			stelthFrame = stelth_interval;
		}

		UpdateShaderConstantBuffer(cbuffH);

		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			int mx, my;
			GetMousePoint(&mx, &my);
			pClickedPos->x = static_cast<float>(mx);
			pClickedPos->y = static_cast<float>(my);
			pClickedPos->z = 1.0f;//被写界深度を行うかどうか
			pClickedPos->w = 0.0f;//被写界深度の範囲

			pClickedPos->z = 0.0f;
			isWave = true;

		}
		

		UpdateShaderConstantBuffer(cbuffClickedPosH);

		

		SetDrawScreen(RTDepthForStelth);
		SetCameraPositionAndTarget_UpVecY(cpos, VGet(0, 0, 0));
		ClearDrawScreen();
		MV1SetUseOrigShader(true);
		SetUseVertexShader(vsNoneH);
		SetUsePixelShader(psDepthOnlyH);
		SetZBufferCmpType3D(DX_CMP_GREATER);
		MV1DrawModel(modelH);//蛇の描画(ステルス用)
		for(auto& tmpModelH : models) {
			MV1DrawModel(tmpModelH);
		}

		SetDrawScreen(RTColor);
		SetCameraPositionAndTarget_UpVecY(cpos, VGet(0, 0, 0));
		SetZBufferCmpType3D(DX_CMP_LESSEQUAL);
		SetUseTextureToShader(4, patternH);

		MV1SetUseOrigShader(true);
		SetUsePixelShader(psNoneH);
		SetUseVertexShader(vsNoneH);
		//
		MV1SetScale(fieldH,VGet(m_scale, m_scale, m_scale));
		auto center = VScale(VAdd(vmax, vmin), 0.5f);
		

		for (auto& v : bbVertices) {
			DrawSphere3D(v, 1.0f, 16, 0xffffff, 0, false);
		}
		//ボックス辺の描画
		//手前
		for (int i = 0;i < 4;++i) {
			DrawLine3D(bbVertices[i], bbVertices[(i + 1) % 4], 0xaaffff);//手前
			DrawLine3D(bbVertices[i+4], bbVertices[(i + 1) % 4+4], 0xaaffff);//奥
			DrawLine3D(bbVertices[i], bbVertices[i+4], 0xaaffff);//手前
		}
		FillGraph(RTDepth, 255, 255, 255, 255);
		FillGraph(RTDistance, 255, 255, 255, 255);
		FillGraph(RTNormal, 0, 0, 0, 0);
		SetShaderConstantBuffer(cbuffH, DX_SHADERTYPE_PIXEL, 4);
		SetShaderConstantBuffer(cbuffClickedPosH, DX_SHADERTYPE_PIXEL, 5);
		SetUseTextureToShader(5, matcapH);
		int result = SetRenderTargetToShader(1, RTNormal);
		result = SetRenderTargetToShader(2, RTDepth);
		result = SetRenderTargetToShader(3, RTDistance);
		for (int i = 0; i < tlistNum; ++i) {
			switch (vertexTypes[i]) {
			case DX_MV1_VERTEX_TYPE_1FRAME:
				SetUsePixelShader(psNoneH);
				SetUseVertexShader(vsNoneH);
				break;
			case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
				SetUsePixelShader(psNormalH);
				SetUseVertexShader(vsNormalH);
				break;
			}
			MV1DrawTriangleList(fieldH, i);
		}
		for(auto& tmpModelH : models) {
			MV1DrawModel(tmpModelH);
		}
		
		//SetUsePixelShader(psGhostH);
		//MV1DrawModel(modelH);
		for (int i = 0; i < modelListNum; ++i) {
			switch (modelVertexTypes[i]) {
			case DX_MV1_VERTEX_TYPE_1FRAME:
				SetUsePixelShader(psNoneH);
				SetUseVertexShader(vsNoneH);
				break;
			case DX_MV1_VERTEX_TYPE_NMAP_1FRAME:
				SetUsePixelShader(psNormalH);
				SetUseVertexShader(vsNormalH);
				break;
			}
			MV1DrawTriangleList(modelH, i);
		}



		SetUsePixelShader(-1);
		SetUseVertexShader(-1);
		MV1SetUseOrigShader(false);
		SetRenderTargetToShader(1, -1);
		SetRenderTargetToShader(2, -1);
		SetRenderTargetToShader(3, -1);
		SetUseTextureToShader(4, -1);//テクスチャもリセット
		SetUseTextureToShader(5, -1);

		SetDrawScreen(RTForPostEffect);
		GraphFilterBlt(RTColor, RTBlur,DX_GRAPH_FILTER_DOWN_SCALE, 2);
		GraphFilter(RTBlur, DX_GRAPH_FILTER_GAUSS, 16, 1400);
		//DrawGraph(0, 0, RTBlur,false);
		//mylib::DrawUtil::DrawGraph(0, 0,RTColor, psNoneEffectH, false,RTDepth,RTBlur);
		mylib::DrawUtil::DrawGraph(0, 0, RTColor, psOutlineH, false,RTDepth,RTNormal);
		SetDrawScreen(DX_SCREEN_BACK);

		if (isWave) {
			pClickedPos->z += 10.0f;
			if (pClickedPos->z > 1000.0f) {
				isWave = false;
			}
		}
		if (stelthFrame > 0) {
			pClickedPos->w = 1.0 - static_cast<float>(stelthFrame) / static_cast<float>(stelth_interval);
		}
		else {
			pClickedPos->w = 0.0f;
		}
		mylib::DrawUtil::DrawGraph(0, 0, RTForPostEffect, psPixelEffect, false, RTDepth,RTDepthForStelth,stelthPatternH,RTDistance);
		SetUseTextureToShader(0, -1);//テクスチャもリセット
		SetUseTextureToShader(1, -1);//テクスチャもリセット
		SetUseTextureToShader(2, -1);//テクスチャもリセット
		SetUseTextureToShader(3, -1);//テクスチャもリセット
		SetUseTextureToShader(4, -1);//テクスチャもリセット


		DrawFormatString(10, 10, 0xffffff, L"camerapos=%2.2f,%2.2f,%2.2f", cpos.x, cpos.y, cpos.z);
		DrawFormatString(10, 30, 0xffffff, L"FPS=%2.2f", GetFPS());

		if (stelthFrame > 0) {
			--stelthFrame;
		}

		auto diffTime = GetTickCount64() - lastTime;
		if (diffTime < 16) {
			Sleep(16 - diffTime);
		}
		lastTime = GetTickCount64();

		

		ScreenFlip();
	}
	
	DxLib_End();
	return 0;
}
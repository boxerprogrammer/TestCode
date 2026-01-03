#include<DxLib.h>
#include<vector>
#include<cmath>
#include<algorithm>
#include<string>
#include<map>
#include<array>
#include<cassert>
using namespace std;

/// <summary>
/// 3Dグラフを描画します。
/// </summary>
/// <param name="handle">描画するグラフのハンドル。</param>
/// <param name="scale">グラフのスケール値。</param>
void Draw3DGraph(int handle, int scale,int vs ,int ps, vector<int> textures){
	int gw, gh;
	GetGraphSize(handle, &gw, &gh);
	float aspect = (float)gw / (float)gh;
	// ここに3Dグラフ描画のコードを追加
	float w = aspect * scale;
	float h = scale;
	// 例: グラフの四隅を描画
	std::array<VERTEX3DSHADER,4> vertices = {
		VERTEX3DSHADER{{-w/2, h/2, 0}, {}, {0,0,1}, {}, {}, {255,255,255,255}, {0,0,0,255}, 0.0f, 0.0f, 0.0f, 0.0f},
		VERTEX3DSHADER{{ w/2, h/2, 0}, {}, {0,0,1}, {}, {}, {255,255,255,255}, {0,0,0,255}, 1.0f, 0.0f, 0.0f, 0.0f},
		VERTEX3DSHADER{{-w/2, -h/2, 0}, {}, {0,0,1}, {}, {}, {255,255,255,255}, {0,0,0,255}, 0.0f, 1.0f, 0.0f, 0.0f},
		VERTEX3DSHADER{{ w/2, -h/2, 0}, {}, {0,0,1}, {}, {}, {255,255,255,255}, {0,0,0,255}, 1.0f, 1.0f, 0.0f, 0.0f}
	};
	SetUsePixelShader(ps);
	SetUseVertexShader(vs);
	for (int i = 0; i < textures.size();++i) {
		SetUseTextureToShader(i,textures[i]);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	// 描画処理（仮）
	DrawPrimitive3DToShader(vertices.data(), 4, DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	for (int i = 0; i < textures.size();++i) {
		SetUseTextureToShader(i,-1); // シェーダーの使用を解除
	}
}

int LoadTexture(const std::wstring& filepath) {
	int handle = LoadGraph(filepath.c_str());
	assert(handle >= 0 && "Failed to load texture");
	return handle;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(TRUE);  // ウィンドウモードに設定
	// DXライブラリの初期化
	if (DxLib_Init() == -1) {
		return -1; // 初期化に失敗した場合は終了
	}
	vector<int> textures;
	textures.push_back(LoadTexture(L"img/card_frame.png"));
	//textures.push_back(LoadTexture(L"img/normal.png"));
	textures.push_back(LoadTexture(L"img/wavenormal.png"));
	//textures.push_back(LoadTexture(L"img/gradation.png"));
	textures.push_back(LoadTexture(L"img/base_grad.png"));
	textures.push_back(LoadTexture(L"img/monster.png"));
	textures.push_back(LoadTexture(L"img/marble_base.png"));
	
	int ps = LoadPixelShader(L"hologramPS.pso");
	assert(ps >= 0);
	int vs = LoadVertexShader(L"hologramVS.vso");
	assert(vs >= 0);
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);
	SetCameraNearFar(1.0f, 100.0f);
	SetupCamera_Perspective(DX_PI_F / 2.0f);
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 0.0f, -8.0f), VGet(0.0f, 0.0f, 0.0f));
	struct BaseCBuffer{
		MATRIX world;
		MATRIX view;
		MATRIX proj;
	};
	int cbuffH = CreateShaderConstantBuffer(sizeof(BaseCBuffer));
	BaseCBuffer* pCBuffer = (BaseCBuffer*)GetBufferShaderConstantBuffer(cbuffH);
	pCBuffer->world=DxLib::MGetIdent();
	pCBuffer->view = GetCameraViewMatrix();
	pCBuffer->proj = GetCameraProjectionMatrix();

	struct CardStateBuffer{
		float time;
		float padding[3];
	};
	int stateCbuffH = CreateShaderConstantBuffer(sizeof(CardStateBuffer));
	CardStateBuffer* pStateCBuffer = (CardStateBuffer*)GetBufferShaderConstantBuffer(stateCbuffH);
	pStateCBuffer->time = 0.0f;
	UpdateShaderConstantBuffer(stateCbuffH);
	VECTOR eular = VGet(0.0f, 0.0f, 0.0f);
	char keystate[256];
	// メインループ
	while (ProcessMessage() == 0) {
		pStateCBuffer->time += 0.025f;
		UpdateShaderConstantBuffer(stateCbuffH);

		GetHitKeyStateAll(keystate);
		if(keystate[KEY_INPUT_LEFT]){
			eular.y -= 0.05f;
		}
		if(keystate[KEY_INPUT_RIGHT]){
			eular.y += 0.05f;
		}
		if(keystate[KEY_INPUT_UP]){
			eular.x += 0.05f;
		}
		if(keystate[KEY_INPUT_DOWN]){
			eular.x -= 0.05f;
		}
		eular.x = std::clamp(eular.x, -DX_PI_F / 6.0f, DX_PI_F / 6.0f);
		eular.y = std::clamp(eular.y, -DX_PI_F / 6.0f, DX_PI_F / 6.0f);
		
		// 画面をクリア
		ClearDrawScreen();
		auto rot = MMult( MGetRotX(eular.x), MGetRotY(eular.y));
		pCBuffer->world = rot;
		
		UpdateShaderConstantBuffer(cbuffH);
		SetShaderConstantBuffer(cbuffH, DX_SHADERTYPE_VERTEX, 0);
		SetShaderConstantBuffer(stateCbuffH, DX_SHADERTYPE_PIXEL, 1);
		// ここに描画処理を追加
		Draw3DGraph(textures[0], 10, vs,ps, textures);
		SetShaderConstantBuffer(-1, DX_SHADERTYPE_VERTEX, 0);
		// 画面を更新
		ScreenFlip();
	}
	// DXライブラリの終了処理
	DxLib_End();
	return 0;
}
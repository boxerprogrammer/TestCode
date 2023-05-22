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


constexpr int game_width = 640;
constexpr int game_height = 480;

constexpr int terrain_width = 512;
constexpr int terrain_height = 512;


using Vertices_t = vector<VERTEX3D>;
using Indices_t = vector<unsigned short>;

void CreateBaseVertices(Vertices_t& vertices,int m,int n) {
	vertices.resize(m * n);
	const float stride_x = 512.0f/static_cast<float>(m);
	const float stride_y = 512.0f/static_cast<float>(n);
	const float stride_u = 1.0f / static_cast<float>(m);
	const float stride_v = 1.0f / static_cast<float>(n);
	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < m; ++i) {
			auto& v = vertices[i + j * m];
			v.pos = { (float)i*stride_x - 256.0f,0,256.0f- (float)j*stride_y };
			v.norm = { 0.0f,1.0f,0.0f };
			v.dif = GetColorU8(255, 255, 255, 255);
			v.u = (float)i * stride_u;
			v.v = (float)j * stride_v;
		}
	}
	return;
}

void CreateLineIndices(Indices_t& indices,int m, int n) {
	//LINELIST�̏ꍇ�A�C���f�b�N�X�̐��́A�_�̐���m,n�Ƃ����
	//((m-1)*n*2)+(m*(n-1)*2)�ł���B
	//�܂��A�������ɓ_��m����ꍇ�A���̐���m-1�B�C���f�b�N�X���̂�(m-1)*2�K�v��
	//���ꂪn�i����킯������A(m-1)*n*2�ł���B���ꂪ�c��������m��n���t�]����B

	indices.resize(((m - 1) * n * 2) + (m * (n - 1) * 2));

	//������
	int idx = 0;
	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < m - 1; ++i) {
			indices[idx] = i + j*m;
			++idx;
			indices[idx] = (i + 1) + j * m;
			++idx;
		}
	}
	//�c����
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n-1; ++j) {
			indices[idx] = i + j * m;
			++idx;
			indices[idx] = i + (j+1) * m;
			++idx;
		}
	}
}
void CreateTriangleIndices(Indices_t& indices, int m, int n) {
	//�܂��A�ЂƂ̃N�A�b�h�̂��߂ɃC���f�b�N�X��6�K�v�B
	//�ŁA���ꂪ(m-1)*(m-1)���݂���
	indices.resize((m - 1) * (n - 1) * 6);
	int idx = 0;
	for (int i = 0; i < m-1; ++i) {
		for (int j = 0; j < n - 1; ++j) {
			//�Ƃ肠�����O�p�`�͎��v���
			//����O�p�`
			indices[idx] = i + j * m;//����
			++idx;
			indices[idx] = (i+1) + j * m;//�E��
			++idx;
			indices[idx] = i + (j+1) * m;//����
			++idx;
			//�E���O�p�`
			indices[idx] = (i+1) + j * m;//�E��
			++idx;
			indices[idx] = (i + 1) + (j+1) * m;//�E��
			++idx;
			indices[idx] = i + (j + 1) * m;//����
			++idx;

		}
	}
}





void CalculateHeightFromHeightMap( Vertices_t& vertices, int heightH,int m, int n) {
	const float stride_x = 512.0f / static_cast<float>(m);
	const float stride_y = 512.0f / static_cast<float>(n);
	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < m; ++i) {
			auto& v = vertices[i + j * m];
			float r, g, b, a;
			GetPixelSoftImageF(heightH, i*stride_x, j*stride_y, &r, &g, &b, &a);

			v.pos.y = r * 100.0f;
		}
	}
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);

	const int height = std::max(terrain_height, game_height);

	SetWindowSize(1280, 720);
	//SetWindowSize(game_width + terrain_width, height);
	
	DxLib_Init();
	//int res = SetGraphMode(game_width+ terrain_width, height, 32);
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
	auto offscreen=MakeScreen(game_width,game_height);

	auto cbufferH = CreateShaderConstantBuffer(sizeof(float) * 4);
	float* time = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
	time[0]=0.0f;

	float slashAngle = 0.0f;
	list<Position> positions;

	int sfw, sfh;
	GetSoftImageSize(heightH, &sfw, &sfh);
	
	constexpr int lattice_num_x = 256;
	constexpr int lattice_num_y = 256;

	Vertices_t vertices;
	CreateBaseVertices(vertices,lattice_num_x,lattice_num_y);
	Indices_t indices;
	//CreateLineIndices(indices, lattice_num_x, lattice_num_y);
	CreateTriangleIndices(indices, lattice_num_x, lattice_num_y);
	CalculateHeightFromHeightMap(vertices, heightH, lattice_num_x, lattice_num_y);


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
				BltSoftImageWithAlphaBlend(0, 0, 32, 32, brushH, mx - 640 - 16, my - 16, heightH, 32);
				lx = mx;
				ly = my;
			}
			
		}
		mlast = minput;
		
		CalculateHeightFromHeightMap(vertices, heightH, lattice_num_x, lattice_num_y);
		//DrawPrimitive3D(vertices.data(), vertices.size(), DX_PRIMTYPE_POINTLIST, brickH, false);
		DrawPrimitiveIndexed3D(vertices.data(), vertices.size(),indices.data(),indices.size(), DX_PRIMTYPE_TRIANGLELIST, brickH, false);
		SetDrawScreen(DX_SCREEN_BACK);
		//SetGraphMode(640*2, 480, 32);
		DrawGraph(0, 0, offscreen, false);
		DrawSoftImage(640, 0, heightH); 
		ScreenFlip();
	}
	return 0;
}
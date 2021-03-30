#include<DxLib.h>
#include<vector>
#include<array>

using namespace std;

struct Position2D {
	float x;
	float y;
};

struct Cube {
	float width;
	float height;
	float depth;
	VECTOR axis[3];//x,y,z軸
	VECTOR center;
	VECTOR calcedCenter;
	VECTOR rotatedCenter;
	VECTOR dispAxis[3];
	std::array<VECTOR,8> vertices;
	std::array<VECTOR, 8> calcedVertices;
	Cube(const VECTOR& c,float w, float h, float d) :center(c),width(w), height(h), depth(d) {
		CreateVertices();
	}
	void CreateVertices() {
		vertices[0].x = center.x - width / 2;
		vertices[0].y = center.y + height / 2;
		vertices[0].z = center.z - depth / 2;

		vertices[1].x = center.x + width / 2;
		vertices[1].y = center.y + height / 2;
		vertices[1].z = center.z - depth / 2;

		vertices[2].x = center.x - width / 2;
		vertices[2].y = center.y - height / 2;
		vertices[2].z = center.z - depth / 2;

		vertices[3].x = center.x + width / 2;
		vertices[3].y = center.y - height / 2;
		vertices[3].z = center.z - depth / 2;

		vertices[4].x = center.x - width / 2;
		vertices[4].y = center.y + height / 2;
		vertices[4].z = center.z + depth / 2;

		vertices[5].x = center.x + width / 2;
		vertices[5].y = center.y + height / 2;
		vertices[5].z = center.z + depth / 2;

		vertices[6].x = center.x - width / 2;
		vertices[6].y = center.y - height / 2;
		vertices[6].z = center.z + depth / 2;

		vertices[7].x = center.x + width / 2;
		vertices[7].y = center.y - height / 2;
		vertices[7].z = center.z + depth / 2;

	}

	void Update(const MATRIX& viewproj, const MATRIX& rot) {
		MATRIX m = MMult(rot, viewproj);
		for (int i = 0; i < 8; ++i) {
			calcedVertices[i] = VTransform(vertices[i],m);
		}
		rotatedCenter= VTransform(center, rot);
		calcedCenter = VTransform(center, m);
		for (int i = 0; i < 3; ++i) {
			axis[i].x = rot.m[i][0];
			axis[i].y = rot.m[i][1];
			axis[i].z = rot.m[i][2];
			axis[i] = VNorm(axis[i]);
			dispAxis[i] = VAdd(rotatedCenter,VScale(axis[i],5.0f));
			dispAxis[i] = VTransform(dispAxis[i], viewproj);
		}
		
		
	}

	Position2D GetPosition2D(const VECTOR& v,float vw,float vh) {
		Position2D pos;
		float cx = vw / 2.0f;
		float cy = vh / 2.0f;
		pos.x = (v.x/v.z+1.0f)*cx;
		pos.y = (1.0f-v.y/v.z)* cy;
		return pos;
	}
	

	void DrawVertices(float vw,float vh,unsigned int color,float thick=3.0f) {
		array<Position2D, 8> p;
		for (int i = 0; i < 8; ++i) {
			p[i]= GetPosition2D(calcedVertices[i], vw, vh);
		}


		DrawLineAA(p[0].x, p[0].y, p[1].x, p[1].y, color, thick);
		DrawLineAA(p[0].x, p[0].y, p[2].x, p[2].y, color, thick);
		DrawLineAA(p[0].x, p[0].y, p[4].x, p[4].y, color, thick);

		DrawLineAA(p[7].x, p[7].y, p[5].x, p[5].y, color, thick);
		DrawLineAA(p[7].x, p[7].y, p[6].x, p[6].y, color, thick);
		DrawLineAA(p[7].x, p[7].y, p[3].x, p[3].y, color, thick);

		DrawLineAA(p[1].x, p[1].y, p[5].x, p[5].y, color, thick);
		DrawLineAA(p[1].x, p[1].y, p[3].x, p[3].y, color, thick);

		DrawLineAA(p[4].x, p[4].y, p[5].x, p[5].y, color, thick);
		DrawLineAA(p[2].x, p[2].y, p[3].x, p[3].y, color, thick);

		DrawLineAA(p[6].x, p[6].y, p[2].x, p[2].y, color, thick);
		DrawLineAA(p[6].x, p[6].y, p[4].x, p[4].y, color, thick);

		for (auto& v : calcedVertices) {
			auto pos = GetPosition2D(v, vw, vh);
			DrawCircleAA(pos.x, pos.y, 5, 8, 0xffaaaa);
		}

		auto c2d=GetPosition2D(calcedCenter, vw, vh);
		auto vx2d = GetPosition2D(dispAxis[0], vw, vh);
		auto vy2d = GetPosition2D(dispAxis[1], vw, vh);
		auto vz2d = GetPosition2D(dispAxis[2], vw, vh);
		DrawLineAA(c2d.x, c2d.y,vx2d.x,  vx2d.y,  0xff0000, 5);
		DrawLineAA(c2d.x, c2d.y, vy2d.x, vy2d.y, 0x8888ff, 5);
		DrawLineAA(c2d.x, c2d.y, vz2d.x, vz2d.y, 0x00ff00, 5);

	}
};

struct RAY
{
	VECTOR org;
	VECTOR dir;
};

bool RaycastCube(RAY* ray, Cube* cube)
{
	const float EPSILON = 1.175494e-37;//このイプシロンが適切かどうかは要検証

	VECTOR m = VScale(VAdd(ray->org , ray->dir) , 0.5f);//線分の中点
	VECTOR d = VSub(ray->dir , m);//線分の長さの半分のベクトル
	m = VSub(m , cube->rotatedCenter);//ボックスと線分を原点まで平行移動

	//↓現在の軸に合わせる(これによってAABBがOBB対応になる)
	m = VGet(VDot(cube->axis[0], m), VDot(cube->axis[1], m), VDot(cube->axis[2], m));
	d = VGet(VDot(cube->axis[0], d), VDot(cube->axis[1], d), VDot(cube->axis[2], d));

	//ワールド座標軸が分離軸かどうかチェック
	float adx = fabsf(d.x);
	if (fabsf(m.x) > cube->width/2.0f + adx) return false;
	float ady = fabsf(d.y);
	if (fabsf(m.y) > cube->height/2.0f + ady) return false;
	float adz = fabsf(d.z);
	if (fabsf(m.z) > cube->depth/2.0f + adz) return false;
	adx += EPSILON;
	ady += EPSILON;
	adz += EPSILON;

	//方向ベクトルの外積を座標軸に対してチェックする
	if (fabsf(m.y * d.z - m.z * d.y) > cube->height / 2.0f * adz + cube->depth / 2.0f * ady) return false;
	if (fabsf(m.z * d.x - m.x * d.z) > cube->width / 2.0f * adz + cube->depth / 2.0f * adx) return false;
	if (fabsf(m.x * d.y - m.y * d.x) > cube->width / 2.0f * ady + cube->height/2.0f * adx) return false;

	return true;
}

VECTOR Get3DRay(const Position2D& spos, float vw, float vh) {
	VECTOR pos;
	float cx = vw / 2.0f;
	float cy = vh / 2.0f;
	pos.x = spos.x/cx-1.0f;
	pos.y = 1.0f-spos.y/cy;
	auto fov=GetCameraFov();
	pos.z = 0;
	auto invproj = MInverse(GetCameraProjectionMatrix());
	pos = VTransform(pos,invproj);
	auto inview=MInverse(GetCameraViewMatrix());
	pos = VTransform(pos, inview);
	return pos;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib_Init();
	
	SetDrawScreen(DX_SCREEN_BACK);
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetCameraNearFar(1.0, 300.0f);
	SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -80), VGet(0, 0, 0));
	SetupCamera_Perspective(DX_PI_F / 4.0f);
	Cube cube(VGet(20,0,0),20,30,6);
	auto view = GetCameraViewMatrix();
	auto proj = GetCameraProjectionMatrix();
	auto viewproj = MMult(view, proj);
	VECTOR rot = VGet(0, 0, 0);
	RAY ray;
	ray.org = VGet(0, 0, -80);
	ray.dir= VGet(0, 0, 80);
	while (DxLib::ProcessMessage() != -1) {
		ClearDrawScreen();
		int mposx, mposy;
		GetMousePoint(&mposx,&mposy);
		Position2D mp;
		mp.x = mposx;
		mp.y = mposy;
		
		ray.dir = VScale(VSub(Get3DRay(mp, 640, 480),ray.org), 100.0f);
		char keystate[256];
		GetHitKeyStateAll(keystate);
		if (keystate[KEY_INPUT_ESCAPE]) {
			break;
		}
		rot.x += 0.01f;
		rot.y += 0.02f;
		rot.z += 0.005f;
		auto m = MGetIdent();
		auto mrot=MGetRotX(rot.x);
		mrot = MMult(mrot, MGetRotY(rot.y));
		mrot = MMult(mrot, MGetRotZ(rot.z));
		m = MMult(mrot, viewproj);
		cube.Update(viewproj,mrot);
		if (RaycastCube(&ray, &cube)) {
			cube.DrawVertices(640, 480, 0xffaaff,8.0f);
		}
		else {
			cube.DrawVertices(640, 480, 0xffffff, 2.0f);
		}
		DrawCircleAA(mposx, mposy, 5, 16, 0xff0000);
		ScreenFlip();
	}

	DxLib_End();
}
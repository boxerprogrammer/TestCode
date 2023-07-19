#include "DrawHelper.h"
#include<DxLib.h>
#include<sstream>
#include<iomanip>
using namespace std;
void MyDrawGraph(int x, int y, int imgHandle, int secondImg, int thirdImg, int psHandle) {
	int width, height;
	GetGraphSize(imgHandle, &width, &height);
	array<VERTEX2DSHADER, 4> verts;

	for (auto& v : verts) {
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);//ディフューズ
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);//スペキュラ
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	//左上	
	verts[0].pos.x = x;
	verts[0].pos.y = y;
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;
	//右上
	verts[1].pos.x = x + width;
	verts[1].pos.y = y;
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;
	//左下
	verts[2].pos.x = x;
	verts[2].pos.y = y + height;
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;
	//右下
	verts[3].pos.x = x + width;
	verts[3].pos.y = y + height;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	//DxLib::SetDrawAlphaTest(true);
	int alphamode, alphaparam;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	GetDrawAlphaTest(&alphamode, &alphaparam);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);
	SetUsePixelShader(psHandle);
	SetUseTextureToShader(0, imgHandle);
	SetUseTextureToShader(1, secondImg);
	SetUseTextureToShader(2, thirdImg);
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}

void MyDrawRotaGraph(int x, int y, float scale, float angle, int imgHandle, int secondImg, int thirdImg, int psHandle) {
	int width, height;
	GetGraphSize(imgHandle, &width, &height);
	
	int w = (width*scale) / 2;
	int h = (height*scale) / 2;

	array<VERTEX2DSHADER, 4> verts;

	for (auto& v : verts) {
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);//ディフューズ
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);//スペキュラ
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	//左上	
	verts[0].pos.x = x-w;
	verts[0].pos.y = y-h;
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;
	//右上
	verts[1].pos.x = x + w;
	verts[1].pos.y = y - h;
	verts[1].u = 1.0f;
	verts[1].v = 0.0f;
	//左下
	verts[2].pos.x = x - w;
	verts[2].pos.y = y + h;
	verts[2].u = 0.0f;
	verts[2].v = 1.0f;
	//右下
	verts[3].pos.x = x + w;
	verts[3].pos.y = y + h;
	verts[3].u = 1.0f;
	verts[3].v = 1.0f;
	
	int alphamode, alphaparam;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	GetDrawAlphaTest(&alphamode, &alphaparam);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);
	SetUsePixelShader(psHandle);
	SetUseTextureToShader(0, imgHandle);
	SetUseTextureToShader(1, secondImg);
	SetUseTextureToShader(2, thirdImg);
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);

}

void MyDrawExtendGraph(int x1, int y1, int x2, int y2, int imgHandle, int secondImg, int thirdImg, int psHandle,
	float repeatX , float repeatY) {
	int width, height;
	GetGraphSize(imgHandle, &width, &height);
	array<VERTEX2DSHADER, 4> verts;

	for (auto& v : verts) {
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);//ディフューズ
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);//スペキュラ
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	//左上	
	verts[0].pos.x = x1;
	verts[0].pos.y = y1;
	verts[0].u = 0.0f;
	verts[0].v = 0.0f;
	//右上
	verts[1].pos.x = x2;
	verts[1].pos.y = y1;
	verts[1].u = 1.0f*repeatX;
	verts[1].v = 0.0f;
	//左下
	verts[2].pos.x = x1;
	verts[2].pos.y = y2;
	verts[2].u = 0.0f;
	verts[2].v = 1.0f*repeatY;
	//右下
	verts[3].pos.x = x2;
	verts[3].pos.y = y2;
	verts[3].u = 1.0f*repeatX;
	verts[3].v = 1.0f*repeatY;
	//DxLib::SetDrawAlphaTest(true);
	int alphamode, alphaparam;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	GetDrawAlphaTest(&alphamode, &alphaparam);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);
	SetUsePixelShader(psHandle);
	SetUseTextureToShader(0, imgHandle);
	SetUseTextureToShader(1, secondImg);
	SetUseTextureToShader(2, thirdImg);
	DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
}

std::vector<int> 
LoadSequentialGraph(const wchar_t* baseFolderPath, const wchar_t* baseName, int pictNum) {
	std::vector<int> ret(pictNum);
	std::fill(ret.begin(), ret.end(), -1);
	for (int i = 0; i < pictNum; ++i) {
		wstringstream wss;
		wss << baseFolderPath;
		wss << baseName;
		wss << setfill(L'0') << setw(2);
		wss << i << L".png";
		if (ret[i] == -1) {
			ret[i] = DxLib::LoadGraph(wss.str().c_str());
		}
	}
	return ret;
}
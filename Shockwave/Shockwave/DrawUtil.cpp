#include "DrawUtil.h"
#include<DxLib.h>
#include<array>

namespace mylib {
	void DrawUtil::DrawRotaGraph(int x, int y, float scale, float angle, int handle, int psH, bool alpha, int secondH,
		int vsH) {
		std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
		//4頂点に必要な情報を代入していく
		//必要な情報→座標、頂点カラー、UV
		//ただし、rhwは必ず1.0にする必要があります。

		for (auto& v : vertices) {
			v.rhw = 1.0f;//これは必ず1
			v.dif = { 255,255,255,255 };//これは白色にしておく
		}

		int w, h;
		DxLib::GetGraphSize(handle, &w, &h);

		//並べ方はZの字になるように
		//頂点情報(左上)
		vertices[0].pos.x = x - w * 0.5 * scale;
		vertices[0].pos.y = y - h * 0.5 * scale;
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;

		//頂点情報(右上)
		vertices[1].pos.x = x + w * 0.5 * scale;
		vertices[1].pos.y = y - h * 0.5 * scale;
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;

		//頂点情報(左下)
		vertices[2].pos.x = x - w * 0.5 * scale;
		vertices[2].pos.y = y + h * 0.5 * scale;
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;

		//頂点情報(右下)
		vertices[3].pos.x = x + w * 0.5 * scale;
		vertices[3].pos.y = y + h * 0.5 * scale;
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;

		//テクスチャを指定
		DxLib::SetUseTextureToShader(0, handle);
		if (secondH >= 0) {
			DxLib::SetUseTextureToShader(1, secondH);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		//シェーダを指定
		DxLib::SetUsePixelShader(psH);

		DxLib::DrawPrimitive2DToShader(vertices.data(), vertices.size(),
			DX_PRIMTYPE_TRIANGLESTRIP);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	void DrawUtil::DrawGraph(int x, int y, int handle, int psH, bool alpha, int secondH,int thirdH,int forthH,int fifthH) {
		std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
		//4頂点に必要な情報を代入していく
		//必要な情報→座標、頂点カラー、UV
		//ただし、rhwは必ず1.0にする必要があります。

		for (auto& v : vertices) {
			v.rhw = 1.0f;//これは必ず1
			v.dif = { 255,255,255,255 };//これは白色にしておく
		}

		int w, h;
		DxLib::GetGraphSize(handle, &w, &h);

		//並べ方はZの字になるように
		//頂点情報(左上)
		vertices[0].pos.x = x;
		vertices[0].pos.y = y;
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;

		//頂点情報(右上)
		vertices[1].pos.x = x + w;
		vertices[1].pos.y = y;
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;

		//頂点情報(左下)
		vertices[2].pos.x = x;
		vertices[2].pos.y = y + h;
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;

		//頂点情報(右下)
		vertices[3].pos.x = x + w;
		vertices[3].pos.y = y + h;
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;

		//テクスチャを指定
		DxLib::SetUseTextureToShader(0, handle);
		if (secondH >= 0) {
			DxLib::SetUseTextureToShader(1, secondH);
		}
		if (thirdH >= 0) {
			DxLib::SetUseTextureToShader(2, thirdH);
		}
		if (forthH >= 0) {
			DxLib::SetUseTextureToShader(3, forthH);
		}
		if (fifthH >= 0) {
			DxLib::SetUseTextureToShader(4, fifthH);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		//シェーダを指定
		DxLib::SetUsePixelShader(psH);

		DxLib::DrawPrimitive2DToShader(vertices.data(), vertices.size(),
			DX_PRIMTYPE_TRIANGLESTRIP);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void DrawUtil::DrawCube(const Vector3& pos, const Vector3& size,
							int vs,int ps,int texHandle)
	{
		std::array<DxLib::VERTEX3DSHADER, 8> vertices = {};
		for (auto& v : vertices) {
			v.spos = {};
			v.norm = {};
			v.tan = {};
			v.binorm = {};
			v.dif = { 255,255,255,255 };//これは白色にしておく
			v.spc = {};
			v.su = v.sv = 0.0f;
		}
		//左上手前:0
		vertices[0].pos = { pos.x - size.x / 2.0f,
							pos.y + size.y / 2.0f,
							pos.z - size.z / 2.0f };
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;

		//右上手前:1
		vertices[1].pos = { pos.x + size.x / 2.0f,
							pos.y + size.y / 2.0f,
							pos.z - size.z / 2.0f };
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;

		//左下手前:2
		vertices[2].pos = { pos.x - size.x / 2.0f,
							pos.y - size.y / 2.0f,
							pos.z - size.z / 2.0f };
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;

		//右下手前:3
		vertices[3].pos = { pos.x + size.x / 2.0f,
							pos.y - size.y / 2.0f,
							pos.z - size.z / 2.0f };
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;

		//奥側
		//左上奥:4
		vertices[4].pos = { pos.x - size.x / 2.0f,
							pos.y + size.y / 2.0f,
							pos.z + size.z / 2.0f };
		vertices[4].u = 0.0f;
		vertices[4].v = 0.0f;

		//右上奥:5
		vertices[5].pos = { pos.x + size.x / 2.0f,
							pos.y + size.y / 2.0f,
							pos.z + size.z / 2.0f };
		vertices[5].u = 1.0f;
		vertices[5].v = 0.0f;

		//左下奥:6
		vertices[6].pos = { pos.x - size.x / 2.0f,
							pos.y - size.y / 2.0f,
							pos.z + size.z / 2.0f };
		vertices[6].u = 0.0f;
		vertices[6].v = 1.0f;

		//右下奥:7
		vertices[7].pos = { pos.x + size.x / 2.0f,
							pos.y - size.y / 2.0f,
							pos.z + size.z / 2.0f };
		vertices[7].u = 1.0f;
		vertices[7].v = 1.0f;

		if (vs != -1) {
			SetUseVertexShader(vs);
		}
		if (ps != -1) {
			SetUsePixelShader(ps);
		}

		std::array<uint16_t, 36> indices = {
				0,1,2, 1,3,2,
				1,5,3, 5,7,3,
				5,4,7, 4,6,7,
				4,0,6, 0,2,6,
				4,5,0, 0,5,1,
				2,3,6, 3,7,6,
		};
		//テクスチャを指定
		DxLib::SetUseTextureToShader(0, texHandle);
		//次の授業でインデックスを指定していく
		DxLib::DrawPrimitiveIndexed3DToShader(vertices.data(), vertices.size(),
			indices.data(),
			indices.size(),
			DX_PRIMTYPE_TRIANGLELIST);

	}

	void DrawUtil::DrawCubeNS(const Vector3& pos, const Vector3& size,int texHandle)
	{
		std::array<DxLib::VERTEX3D, 8> vertices = {};
		for (auto& v : vertices) {
			v.norm = {};
			v.dif = { 255,255,255,255 };//これは白色にしておく
			v.spc = {};
			v.su = v.sv = 0.0f;
		}
		//左上手前:0
		vertices[0].pos = { pos.x - size.x / 2.0f,
							pos.y + size.y / 2.0f,
							pos.z - size.z / 2.0f };
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;

		//右上手前:1
		vertices[1].pos = { pos.x + size.x / 2.0f,
							pos.y + size.y / 2.0f,
							pos.z - size.z / 2.0f };
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;

		//左下手前:2
		vertices[2].pos = { pos.x - size.x / 2.0f,
							pos.y - size.y / 2.0f,
							pos.z - size.z / 2.0f };
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;

		//右下手前:3
		vertices[3].pos = { pos.x + size.x / 2.0f,
							pos.y - size.y / 2.0f,
							pos.z - size.z / 2.0f };
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;

		//奥側
		//左上奥:4
		vertices[4].pos = { pos.x - size.x / 2.0f,
							pos.y + size.y / 2.0f,
							pos.z + size.z / 2.0f };
		vertices[4].u = 0.0f;
		vertices[4].v = 0.0f;

		//右上奥:5
		vertices[5].pos = { pos.x + size.x / 2.0f,
							pos.y + size.y / 2.0f,
							pos.z + size.z / 2.0f };
		vertices[5].u = 1.0f;
		vertices[5].v = 0.0f;

		//左下奥:6
		vertices[6].pos = { pos.x - size.x / 2.0f,
							pos.y - size.y / 2.0f,
							pos.z + size.z / 2.0f };
		vertices[6].u = 0.0f;
		vertices[6].v = 1.0f;

		//右下奥:7
		vertices[7].pos = { pos.x + size.x / 2.0f,
							pos.y - size.y / 2.0f,
							pos.z + size.z / 2.0f };
		vertices[7].u = 1.0f;
		vertices[7].v = 1.0f;


		std::array<uint16_t, 36> indices = {
				0,1,2, 1,3,2,
				1,5,3, 5,7,3,
				5,4,7, 4,6,7,
				4,0,6, 0,2,6,
				4,5,0, 0,5,1,
				2,3,6, 3,7,6,
		};
		DxLib::GetUseBackCulling();
		//次の授業でインデックスを指定していく
		DxLib::DrawPrimitiveIndexed3D(vertices.data(), vertices.size(),
			indices.data(),
			indices.size(),
			DX_PRIMTYPE_TRIANGLELIST,texHandle,false);
	}


}

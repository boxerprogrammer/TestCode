#pragma once
namespace mylib {
	struct Vector3 {
		float x;
		float y;
		float z;
	};
class DrawUtil
{
public:
	static void DrawRotaGraph(int x, int y, float scale, float angle, int handle, int psH, bool alpha, int secondH = -1,int vsH=-1);
	static void DrawGraph(int x, int y, int handle, int psH, bool alpha, int secondH = -1,int thirdH=-1,int forthH=-1,int fifthH=-1);
	static void DrawCube(const Vector3& pos, const Vector3& size, int vs = -1, int ps = -1,int texHandle=-1);
	static void DrawCubeNS(const Vector3& pos, const Vector3& size,int texHandle=-1);
};
}


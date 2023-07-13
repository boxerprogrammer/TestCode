#include<DxLib.h>
#include<string>
#include<vector>

enum class ObjectType
{
	Mesh=0,
	Camera=1,
	Light=2
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	auto file=DxLib::FileRead_open("./Data.pos");
	struct Vector3 {
		float x, y, z;
	};
	struct Object {
		ObjectType objType;
		std::string name;
		Vector3 pos;
		Vector3 rot;
		float FOV;
		float near_, far_;
	};
	std::vector<Object> data;
	char sig[4];
	float version;
	int dataNum;
	DxLib::FileRead_read(sig, sizeof(sig), file);
	DxLib::FileRead_read(&version, sizeof(version), file);
	DxLib::FileRead_read(&dataNum, sizeof(dataNum), file);

	data.resize(dataNum);
	for (int i = 0; i < dataNum; ++i) {
		auto& obj = data[i];
		byte bType;
		DxLib::FileRead_read(&bType, sizeof(bType), file);
		obj.objType = (ObjectType)bType;
		byte strLen;
		DxLib::FileRead_read(&strLen, sizeof(strLen), file);
		obj.name.resize(strLen);
		DxLib::FileRead_read(&obj.name[0], strLen, file);
		DxLib::FileRead_read(&obj.pos, sizeof(obj.pos), file);
		DxLib::FileRead_read(&obj.rot, sizeof(obj.rot), file);
		if (obj.objType == ObjectType::Camera) {
			DxLib::FileRead_read(&obj.FOV, sizeof(obj.FOV), file);
			DxLib::FileRead_read(&obj.near_, sizeof(obj.near_), file);
			DxLib::FileRead_read(&obj.far_, sizeof(obj.far_), file);
		}
	}

	while (ProcessMessage() != -1) {
		ClearDrawScreen();
		int y = 10;
		for (auto& d : data) {
			DrawFormatString(10, y, 0xffffff, "%s.pos=[%2.2f,%2.2f,%2.2f] .rot=[%2.2f,%2.2f,%2.2f]",
				d.name.c_str(), d.pos.x, d.pos.y, d.pos.z,
				d.rot.x, d.rot.y, d.rot.z);
			if (d.objType == ObjectType::Camera) {
				y += 16;
				DrawFormatString(30, y, 0xffffff, "FOV=%2.2f, Near=%2.2f, Far=%2.2f",
					d.FOV, d.near_, d.far_);
			}
			y += 16;
		}
		ScreenFlip();
	}
	DxLib_End();
}